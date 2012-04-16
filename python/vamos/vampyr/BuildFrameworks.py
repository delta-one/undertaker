#
#   BuildFrameworks - utility classes for working in source trees
#
# Copyright (C) 2011 Christian Dietrich <christian.dietrich@informatik.uni-erlangen.de>
# Copyright (C) 2011-2012 Reinhard Tartler <tartler@informatik.uni-erlangen.de>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

from vamos.vampyr.Configuration import BareConfiguration, LinuxConfiguration
from vamos.vampyr.utils import find_configurations
from vamos.golem.kbuild import file_in_current_configuration, \
    guess_arch_from_filename, guess_subarch_from_arch, \
    get_linux_version, NotALinuxTree
from vamos.tools import execute

import os.path
import logging

class BuildFramework:
    """ Base class for all Build System Frameworks"""

    def __init__(self, options=None):
        """
        options: Dictonary that control the used helper tools. The
        following keys are supported:

        args: Dictionary with extra arguments for various tools (see note below)

        exclude_others: boolean, if set, #include statements will be ignored.

        loglevel: sets the loglevel, see the logging package.

        Note: in the args dictionary, the 'undertaker' key can be used
        to pass extra arguments to undertaker when calculating the
        configurations. For instance, pass "-C simple" to select the
        non-greedy algorithm for caluculating configurations.

        For the tools 'sparse', 'gcc' and 'clang', this field allows you
        to enable additional warnings (e.g., -Wextra).
        """

        if options:
            assert(isinstance(options, dict))
            if options.has_key('args'):
                assert(isinstance(options['args'], dict))
            self.options = options
        else:
            self.options = None

    def calculate_configurations(self, filename):
        raise NotImplementedError


class BareBuildFramework(BuildFramework):
    """ For use without Makefiles, works directly on source files """

    def __init__(self, options=None):
        BuildFramework.__init__(self, options)

    def calculate_configurations(self, filename):
        undertaker = "undertaker -q -j coverage -C min -O combined"
        if 'undertaker' in self.options['args']:
            undertaker += " " + self.options['args']['undertaker']
        undertaker += " '" + filename + "'"
        execute(undertaker, failok=False)

        configs = list()
        for cfgfile in find_configurations(filename):
            assert '.config' in cfgfile
            cppflagsfile = cfgfile.replace('.config', '.cppflags')
            with open(cppflagsfile) as f:
                flags = " ".join([s.rstrip() for s in f.readlines()])
            configs.append(BareConfiguration(self, cfgfile, flags))
        return configs


class KbuildBuildFramework(BuildFramework):
    """ For use with Linux, considers Kconfig constraints """

    def __init__(self, options=None):
        if not options:
            options=dict()
        BuildFramework.__init__(self, options)
        if not options.has_key('expansion_strategy'):
            options['expansion_strategy'] = 'alldefconfig'
        if not options.has_key('coverage_strategy'):
            options['coverage_strategy'] = 'min'
        if not options.has_key('arch'):
            options['arch'] = None
        if not options.has_key('subarch'):
            options['subarch'] = None

    def guess_arch_from_filename(self, filename):
        """
        Tries to guess the architecture from a filename. Uses golem's
        guess_arch_from_filename() method.
        """
        (arch, subarch) = guess_arch_from_filename(filename)
        self.options['arch'] = arch
        self.options['subarch'] = subarch
        return (arch, subarch)

    def calculate_configurations(self, filename):
        """Calculate configurations for a given file with Kconfig output mode"""

        if self.options['arch'] is None:
            oldsubarch = self.options['subarch']
            self.guess_arch_from_filename(filename)
            if oldsubarch is not None:
                # special case: if only subarch is set, restore it
                self.options['subarch'] = oldsubarch

        cmd = "undertaker -q -j coverage -C %s -O combined" % self.options['coverage_strategy']
        if os.path.isdir("models"):
            cmd += " -m models/%s.model" % self.options['arch']
        else:
            logging.info("No models directory found, running without models")

        logging.info("Calculating configurations for '%s'", filename)
        if self.options and self.options.has_key('args'):
            if 'undertaker' in self.options['args']:
                cmd += " " + self.options['args']['undertaker']

        cmd += " '%s'" % filename.replace("'", "\\'")
        (output, statuscode) = execute(cmd, failok=True)
        if statuscode != 0 or any([l.startswith("E:") for l in output]):
            logging.error("Running undertaker failed: %s", cmd)
            print "--"
            for i in output:
                logging.error(i)
            return set()

        logging.info("Testing which configurations are actually being compiled")
        configs = list()
        for cfgfile in find_configurations(filename):
            config_obj = LinuxConfiguration(self, cfgfile,
                                            arch=self.options['arch'], subarch=self.options['subarch'],
                                            expansion_strategy=self.options['expansion_strategy'])
            config_obj.switch_to()
            if file_in_current_configuration(filename, config_obj.arch, config_obj.subarch) != "n":
                logging.info("Configuration '%s' is actually compiled", cfgfile)
                configs.append(config_obj)
            else:
                logging.info("Configuration '%s' is *not* compiled", cfgfile)

        return configs


def select_framework(identifier, options):

    frameworks = {'kbuild' : KbuildBuildFramework,
                  'linux'  : KbuildBuildFramework,
                  'bare'   : BareBuildFramework}

    if identifier is None:
        identifier='bare'
        try:
            logging.info("Detected Linux version %s, selecting kbuild framework",
                         get_linux_version())
            identifier='linux'
        except NotALinuxTree:
            pass

    if os.environ.has_key('ARCH'):
        options['arch'] = os.environ['ARCH']

        if os.environ.has_key('SUBARCH'):
            options['subarch'] = os.environ['SUBARCH']
        else:
            options['subarch'] = guess_subarch_from_arch(options['arch'])

    if identifier in frameworks:
        bf = frameworks[identifier](options)
    else:
        raise RuntimeError("Build framework '%s' not found" % \
                               options['framework'])
    return bf
