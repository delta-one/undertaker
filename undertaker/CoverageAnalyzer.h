/*
 *   undertaker - analyze preprocessor blocks in code
 *
 * Copyright (C) 2011 Christian Dietrich <christian.dietrich@informatik.uni-erlangen.de>
 * Copyright (C) 2011-2012 Reinhard Tartler <tartler@informatik.uni-erlangen.de>
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

#ifndef _COVERAGEANALYZER_H_
#define _COVERAGEANALYZER_H_

#include "SatChecker.h"

#include <list>
#include <set>
#include <string>

class ConditionalBlock;
class ConfigurationModel;


/************************************************************************/
/* CoverageAnalyzer                                                     */
/************************************************************************/

class CoverageAnalyzer {
public:
    virtual std::list<SatChecker::AssignmentMap> blockCoverage(ConfigurationModel *) = 0;

    // NB: missingSet is filled during blockCoverage run
    MissingSet getMissingSet() const { return missingSet; }

protected:
    /* c'tor */
    CoverageAnalyzer(const CppFile *file) : file(file) {};

    std::string baseFileExpression(const ConfigurationModel *model);

    const CppFile * file;
    MissingSet missingSet; // set of strings
};

/************************************************************************/
/* SimpleCoverageAnalyzer                                               */
/************************************************************************/

class SimpleCoverageAnalyzer : public CoverageAnalyzer {
public:
    SimpleCoverageAnalyzer(CppFile *f) : CoverageAnalyzer(f) {};
    virtual std::list<SatChecker::AssignmentMap> blockCoverage(ConfigurationModel *) final override;
};

/************************************************************************/
/* MinimizeCoverageAnalyzer                                             */
/************************************************************************/

class MinimizeCoverageAnalyzer : public CoverageAnalyzer {
public:
    MinimizeCoverageAnalyzer(CppFile *f) : CoverageAnalyzer(f) {};
    virtual std::list<SatChecker::AssignmentMap> blockCoverage(ConfigurationModel *) final override;
};

#endif /* _COVERAGEANALYZER_H_ */
