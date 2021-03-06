// -*- mode: c++ -*-
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

#ifndef cnf_configuration_model_h__
#define cnf_configuration_model_h__

#include "RsfReader.h" // for 'StringList'
#include "ConfigurationModel.h"

#include <string>
#include <set>
#include <list>
#include <boost/regex.hpp>

namespace kconfig {
    class PicosatCNF;
}


class CnfConfigurationModel: public ConfigurationModel {
public:
    CnfConfigurationModel(const std::string &filename);

    //! destructor
    virtual ~CnfConfigurationModel();

    //! add feature to whitelist ('ALWAYS_ON')
    virtual void addFeatureToWhitelist(const std::string feature)        final override;

    //! gets the current feature whitelist ('ALWAYS_ON')
    /*!
     * NB: The referenced list gets invalidated by addFeatureToWhitelist!
     *
     * The referenced object must not be freed, the model class manages it.
     */
    virtual const StringList *getWhitelist()                       const final override;

    //! add feature to blacklist ('ALWAYS_OFF')
    /*!
     * NB: This invalidates possibly returned StringList objects
     * referenced by getWhitelist(). Be sure to call getWhitelist()
     * again after using this method.
     */
    virtual void addFeatureToBlacklist(const std::string feature)        final override;

    //! gets the current feature blacklist ('ALWAYS_OFF')
    virtual const StringList *getBlacklist()                       const final override;


    virtual int doIntersect(const std::string exp,
                    const ConfigurationModel::Checker *c,
                    std::set<std::string> &missing,
                    std::string &intersected)                      const final override;

    virtual int doIntersect(const std::set<std::string> exp,
                    const ConfigurationModel::Checker *c,
                    std::set<std::string> &missing,
                    std::string &intersected)                      const final override;

    virtual std::set<std::string> findSetOfInterestingItems(const std::set<std::string> &)
                                                                   const final override;

    //! checks if a given item should be in the model space
    virtual bool inConfigurationSpace(const std::string &symbol)   const final override;

    //! checks if we can assume that the configuration space is complete
    virtual bool isComplete()                                      const final override;

    //@{
    //! checks the type of a given symbol.
    //! @return false if not found
    virtual bool isBoolean(const std::string&)                     const final override;
    virtual bool isTristate(const std::string&)                    const final override;
    //@}

    //! returns the version identifier for the current model
    virtual const std::string getModelVersionIdentifier()          const final override {
        return "cnf";
    }

    //! returns the type of the given symbol
    /*!
     * Normalizes the given item so that passing with and without
     * CONFIG_ prefix works.
     */
    virtual std::string getType(const std::string &feature_name)   const final override;

    virtual bool containsSymbol(const std::string &symbol)         const final override;

    virtual const StringList *getMetaValue(const std::string &key) const final override;

    const kconfig::PicosatCNF *getCNF(void) { return _cnf; }

private:
    std::string _name;
    boost::regex _inConfigurationSpace_regexp;
    kconfig::PicosatCNF *_cnf;
};
#endif
