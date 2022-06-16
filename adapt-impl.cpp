/*
 * Copyright (c) 2019, Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 *
 * Written by:  Michael Lam, lam26@llnl.gov
 *              Harshitha Menon, harshitha@llnl.gov
 *
 * LLNL-CODE-762758. All rights reserved.
 *
 * This file is part of ADAPT. For details, see https://github.com/LLNL/adapt-fp
 *
 * Please also see the LICENSE file for our notice and the GPL.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the IMPLIED WARRANTY OF MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the terms and conditions of the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef __ADAPT_IMPL_CPP
#define __ADAPT_IMPL_CPP

#include <adapt.h>

//JSON format library
#include "ToolConfig.cpp"

// "tape" for recording operations
static AD_real::Tape& tape = AD_real::getTape();

// gradient index data type
typedef AD_real::Identifier AD_index;

// total number of independent/intermediate variables
static unsigned long indCount = 0;

// flag indicating whether source info should be integrated into variable labels
static bool useSourceInfo = false;

// flag indicating whether error should be absolute or signed
static bool useAbsoluteValueError = false;

// maps for storing independent/dependent variables
// (and tolerable errors for the latter)
static std::unordered_map<unsigned long, AD_index>      indIdxs;
static std::unordered_map<unsigned long, double>        indVals;
static std::unordered_map<unsigned long, std::string>   indLabels;
static std::unordered_map<std::string, AD_real*>        depVars;
static std::unordered_map<std::string, double>          depErrs;
static std::unordered_map<std::string, bool>          accVars;

void AD_begin()
{
    // clear internal data structures
    indIdxs.clear();
    indVals.clear();
    indLabels.clear();
    depVars.clear();
    depErrs.clear();
    tape.clearAdjoints();
    tape.reset();

    // start tape if not running
    if (!tape.isActive()) {
        tape.setActive();
    }
}

void AD_end()
{
    // stop tape if still running
    if (tape.isActive()) {
        tape.setPassive();
    }
}

double AD_value(const AD_real &var)
{
    return var.value();
}

double AD_value(double var)
{
    return var;
}

void AD_enable_source_aggregation()
{
    useSourceInfo = true;
}

void AD_enable_absolute_value_error()
{
    useAbsoluteValueError = true;
}

void AD_independent(AD_real &var, std::string label)
{
    // make sure we're recording
    if (!tape.isActive()) {
        tape.setActive();
    }

    tape.registerInput(var);
    indIdxs[indCount] = var.getIdentifier();
    indLabels[indCount] = label;
    indVals[indCount++] = var.value();
}

void AD_independent(AD_real &var, std::string label, std::string source)
{
    if (useSourceInfo) {
        label = label + ":" + source;
    }
    AD_independent(var, label);
}

void AD_intermediate(AD_real &var, std::string label)
{
    indIdxs[indCount] = var.getIdentifier();
    indLabels[indCount] = label;
    indVals[indCount++] = var.value();
}

void AD_intermediate(AD_real &var, std::string label, std::string source, bool isAccVar)
{
    if (useSourceInfo) {
        label = label + ":" + source;
    }
    if (isAccVar) {
      accVars[label] = true;
    }
    AD_intermediate(var, label);
}

void AD_intermediate_iter(AD_real &var, std::string label, std::string source, int iter)
{
    if (useSourceInfo) {
        label = label + ":" + source;
    }
    label = label + ": " + std::to_string(iter);
    AD_intermediate(var, label);
}


void AD_dependent(AD_real &var, std::string label, double toleratedError)
{
    tape.registerOutput(var);
    depVars[label] = &var;
    depErrs[label] = toleratedError;
}

void AD_dependent(AD_real &var, std::string label, int numDigits)
{
    // to calculate error tolerance in the output, scale the output variable by
    // 5 x 10^(-numDigits)
    double scale = 5.0 / pow(10.0, (double)numDigits);
    AD_dependent(var, label, (double)(scale * var.value()));
}

bool pair_comp_inc(std::pair<std::string,double> a, std::pair<std::string,double> b)
{
    return fabs(a.second) < fabs(b.second);
}
bool pair_comp_dec(std::pair<std::string,double> a, std::pair<std::string,double> b)
{
    return fabs(a.second) > fabs(b.second);
}

void AD_report()
{
    size_t numIndVars = indLabels.size();
    size_t numDepVars = depVars.size();

    // check for sufficient variable counts
    if (numIndVars == 0 || numDepVars == 0) {
        std::cerr << "ERROR: Must have at least one independent/intermediate "
                  << "and one dependent variable." << std::endl;
        return;
    }

    std::cout << "=== BEGIN ADAPT REPORT ===" << std::endl;
    std::cout << numIndVars << " total independent/intermediate variables" << std::endl;
    std::cout << numDepVars << " dependent variables" << std::endl;

    // aggregate program variable info by label; this is sufficiently general
    // to handle both per-definition (default) or per-assignment/operation
    // approaches as long as the instrumentation provides source info when
    // registering the independent/intermediate variables; the per-operation
    // behavior can be enabled using AD_enable_source_aggregation()
    //
    std::unordered_map<std::string, long>        varCount;
    std::unordered_map<std::string, double> varMetric;
    std::unordered_map<std::string, std::vector<double> > varOutputError;

    // dependent labels and tolerated errors
    std::vector<std::string> depLabels;
    std::vector<double> tolError;
    std::vector<double> totalError;

    // loop over all registered dependent (output) variables
    for (auto& dep : depVars) {

        // save label
        depLabels.push_back(dep.first);
        tolError.push_back(fabs(depErrs[dep.first]));
        totalError.push_back(0.0);

        // perform a clean analysis with the given output variable
        tape.clearAdjoints();
        dep.second->setGradient(1.0);
        tape.evaluate();

        // loop over all registered independent (input) variables
        // and update assignment-based aggregation data structures
        for (auto& ind : indIdxs) {
            unsigned long input = ind.first;
            std::string inputLabel = indLabels[input];
            double partial = tape.getGradient(ind.second);
            double value = indVals[input];
            double varInputError = value - (float) value;

            // instance count (aggregated by variable) -- only necessary once
            if (depLabels.size() == 1) {
                if (varCount.find(inputLabel) == varCount.end()) {
                    varCount[inputLabel] = 0;
                }
                varCount[inputLabel]++;
            }

            // output sensitivity (aggregated by variable)
            // TODO: change this for multiple dependent variables?
            if (varMetric.find(inputLabel) == varMetric.end()) {
                varMetric[inputLabel] = 0.0;
            }
            varMetric[inputLabel] += partial * value;

            // output error if converted to single precision (aggregated by
            // variable, stored separately for each dependent variable)
            if (varOutputError.find(inputLabel) == varOutputError.end()) {
                varOutputError[inputLabel] = std::vector<double>();
            }
            while (varOutputError[inputLabel].size() < depLabels.size()) {
                varOutputError[inputLabel].push_back(0.0);
            }
            if (useAbsoluteValueError) {
            	varOutputError[inputLabel][depLabels.size()-1] += fabs(partial * varInputError);
	    } else if (accVars.find(inputLabel) == accVars.end()) {
            	varOutputError[inputLabel][depLabels.size()-1] += partial * varInputError;
	    } else {
            	varOutputError[inputLabel][depLabels.size()-1] += fabs(partial * varInputError);
	    }
        }
    }

    // get variables and error contributions, then sort by increasing max error
    std::vector<std::pair<std::string, double>> vars;
    for (auto &var: varOutputError) {
        double maxError = varOutputError[var.first][0];
        for (size_t i = 1; i < numDepVars; i++) {
            double err = fabs(varOutputError[var.first][i]);
            if (err > maxError) {
                maxError = err;
            }
        }
        vars.push_back(std::make_pair(var.first, maxError));
    }
    std::sort(vars.begin(), vars.end(), pair_comp_inc);

    ToolConfig config;
    config.setToolID("ADAPT");

    // find maximum label length for prettier output
    size_t maxLabelLen = 1;
    for (auto& var: vars) {
        if (var.first.size() > maxLabelLen) {
            maxLabelLen = var.first.size();
        }
    }

    // calculate and emit mixed-precision configuration
    std::cout << "Mixed-precision recommendation:" << std::endl;
    for (auto& var: vars) {

        // update tolerated error for each dependent and determine whether
        // there is enough error budget left to replace this var
        bool replace = true;
        for (size_t i = 0; i < numDepVars; i++) {
            tolError[i] -= fabs(varOutputError[var.first][i]);
            totalError[i] += fabs(varOutputError[var.first][i]);
            if (tolError[i] < 0.0) {
                replace = false;
            }
        }

        // emit output
        if (replace) {
            std::cout << "  Replace variable ";
            config.addReplaceVarBaseType(var.first, var.first, var.second,
                    varCount[var.first]);
        } else {
            std::cout << "  DO NOT replace   ";
        }
        std::cout << std::left << std::setw(maxLabelLen+4) << var.first
                  << std::scientific
                  << "  max error introduced: " << fabs(var.second)
                  << "  count: " << std::setw(10) << varCount[var.first]
                  << "  totalerr: ";
        for (size_t i = 0; i < numDepVars; i++) {
            if (i > 0) { std::cout << " "; }
            std::cout << totalError[i];           // total error contribution 
        }
        std::cout << std::endl;
    }

    //save json config
    config.saveConfig("adapt_recommend.json");

#define DUMP_VEC(X)     std::cout << #X << std::endl; \
                        for (auto& p: X) { \
                            std::cout << "  " << p << std::endl; \
                        }

#define DUMP_MAP(X)     std::cout << #X << std::endl; \
                for (auto& p: X) { \
                            std::cout << "  " << p.first \
                                    << " : " << p.second << std::endl; \
                        }

#define DUMP_MAPVEC(X)  std::cout << #X << std::endl; \
                        for (auto& p: X) { \
                            std::cout << "  " << p.first \
                                    << " : "; \
                            for (auto& q: p.second) { \
                                std::cout << q << " "; \
                            } \
                            std::cout << std::endl; \
                        }

    // DEBUG OUTPUT (TODO: remove)
    //DUMP_VEC(depLabels);
    //DUMP_MAP(depErrs);
    //DUMP_MAP(varCount);
    //DUMP_MAP(varMetric);
    //DUMP_MAPVEC(varOutputError);
    //DUMP_MAP(vars);

    std::cout << "=== END ADAPT REPORT ===" << std::endl;
}

#endif // __ADAPT_IMPL_CPP

