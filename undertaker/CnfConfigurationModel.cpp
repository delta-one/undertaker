/*
 *   undertaker - analyze preprocessor blocks in code
 *
 * Copyright (C) 2012 Ralf Hackner <rh@ralf-hackner.de>
 * Copyright (C) 2013-2014 Stefan Hengelein <stefan.hengelein@fau.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef DEBUG
#define BOOST_FILESYSTEM_NO_DEPRECATED
#endif

#include "CnfConfigurationModel.h"
#include "Tools.h"
#include "StringJoiner.h"
#include "Logging.h"
#include "PicosatCNF.h"

#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>


CnfConfigurationModel::CnfConfigurationModel(const std::string &filename) {
    const StringList *configuration_space_regex = nullptr;
    boost::filesystem::path filepath(filename);
    _name = filepath.stem().string();

    _cnf = new kconfig::PicosatCNF();
    _cnf->readFromFile(filename);
    configuration_space_regex = _cnf->getMetaValue("CONFIGURATION_SPACE_REGEX");

    if (configuration_space_regex != nullptr && configuration_space_regex->size() > 0) {
        Logging::info("Set configuration space regex to '", configuration_space_regex->front(),
                      "'");
        _inConfigurationSpace_regexp = boost::regex(configuration_space_regex->front());
    } else {
        _inConfigurationSpace_regexp = boost::regex("^CONFIG_[^ ]+$");
    }
    if (_cnf->getVarCount() == 0) {
        // if the model is empty (e.g., if /dev/null was loaded), it cannot possibly be complete
        _cnf->addMetaValue("CONFIGURATION_SPACE_INCOMPLETE", "1");
    }
}

CnfConfigurationModel::~CnfConfigurationModel() {
    delete _cnf;
}

void CnfConfigurationModel::addFeatureToWhitelist(const std::string feature) {
    const std::string magic("ALWAYS_ON");
    _cnf->addMetaValue(magic, feature);
}

const StringList *CnfConfigurationModel::getWhitelist() const {
    const std::string magic("ALWAYS_ON");
    return _cnf->getMetaValue(magic);
}

void CnfConfigurationModel::addFeatureToBlacklist(const std::string feature) {
    const std::string magic("ALWAYS_OFF");
    _cnf->addMetaValue(magic, feature);
}

const StringList *CnfConfigurationModel::getBlacklist() const {
    const std::string magic("ALWAYS_OFF");
    return _cnf->getMetaValue(magic);
}

const StringList *CnfConfigurationModel::getMetaValue(const std::string &key) const {
    return _cnf->getMetaValue(key);
}

std::set<std::string> CnfConfigurationModel::findSetOfInterestingItems(
                                    const std::set<std::string> &) const {
    return {};
}

int CnfConfigurationModel::doIntersect(const std::string exp,
                                    const ConfigurationModel::Checker *c,
                                    std::set<std::string> &missing,
                                    std::string &intersected) const {
    const std::set<std::string> start_items = undertaker::itemsOfString(exp);
    return doIntersect(start_items, c, missing, intersected);
}


int CnfConfigurationModel::doIntersect(const std::set<std::string> start_items,
                                    const ConfigurationModel::Checker *c,
                                    std::set<std::string> &missing,
                                    std::string &intersected) const {
    StringJoiner sj;
    int valid_items = 0;


    const std::string magic_on("ALWAYS_ON");
    const std::string magic_off("ALWAYS_OFF");
    const StringList *always_on = _cnf->getMetaValue(magic_on);
    const StringList *always_off = _cnf->getMetaValue(magic_off);

    for (const std::string &str : start_items) {
        if (containsSymbol(str)) {
            valid_items++;
            if (always_on) {
                const auto &cit = std::find(always_on->begin(), always_on->end(), str);
                if (cit != always_on->end()) // str is found
                    sj.push_back(str);
            }
            if (always_off) {
                const auto &cit = std::find(always_off->begin(), always_off->end(), str);
                if (cit != always_off->end()) // str is found
                    sj.push_back("!" + str);
            }
        } else {
            // check if the symbol might be in the model space.
            // if not it can't be missing!
            Logging::debug(str);
            if (!inConfigurationSpace(str))
                continue;
            // iff we are given a checker for items, skip if it doesn't pass the test
            if (c && ! (*c)(str)) {
                continue;
            }
            /* free variables are never missing -> check if str starts with __FREE__ */
            if (str.size() > 1 && !boost::starts_with(str, "__FREE__"))
                missing.insert(str);
        }
    }
    sj.push_back("._." + _name + "._.");
    intersected = sj.join("\n&& ");
    Logging::debug("Out of ", start_items.size(), " items ", missing.size(),
                   " have been put in the MissingSet using ", _name);
    return valid_items;
}

bool CnfConfigurationModel::inConfigurationSpace(const std::string &symbol) const {
    if (boost::regex_match(symbol, _inConfigurationSpace_regexp))
        return true;
    return false;
}

bool CnfConfigurationModel::isComplete() const {
    const StringList *configuration_space_complete = _cnf->getMetaValue("CONFIGURATION_SPACE_INCOMPLETE");
    // Reverse logic at this point to ensure Legacy models for kconfig to work
    return !(configuration_space_complete != nullptr);
}

bool CnfConfigurationModel::isBoolean(const std::string &item) const {
    return _cnf->getSymbolType(item) == 1;
}

bool CnfConfigurationModel::isTristate(const std::string &item) const {
    return _cnf->getSymbolType(item) == 2;
}

std::string CnfConfigurationModel::getType(const std::string &feature_name) const {
    static const boost::regex item_regexp("^(CONFIG_)?([0-9A-Za-z_]+)(_MODULE)?$");
    boost::smatch what;

    if (boost::regex_match(feature_name, what, item_regexp)) {
        std::string item = what[2];
        int type = _cnf->getSymbolType(item);
        static const std::string types[] = { "MISSING", "BOOLEAN", "TRISTATE", "INTEGER", "HEX", "STRING", "other"} ;
        return types[type];
    }
    return "#ERROR";
}

bool CnfConfigurationModel::containsSymbol(const std::string &symbol) const {
    if (symbol.substr(0, 5) == "FILE_") {
        return true;
    }
    if (_cnf->getAssociatedSymbol(symbol) != nullptr) {
        return true;
    }
    return false;
}
