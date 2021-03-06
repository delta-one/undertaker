/*
 *   undertaker - analyze preprocessor blocks in code
 *
 * Copyright (C) 2009-2012 Reinhard Tartler <tartler@informatik.uni-erlangen.de>
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

#include "ConfigurationModel.h"
#include "StringJoiner.h"


std::string ConfigurationModel::getMissingItemsConstraints(const std::set<std::string> &missing) {
    StringJoiner sj;

    for (const std::string &str : missing)
        sj.push_back(str);

    std::stringstream ss;
    if (sj.size() > 0)
        ss << "( ! ( " <<  sj.join(" || ") << " ) )";
    return ss.str();
};
