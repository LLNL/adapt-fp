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

#ifndef __ADAPT_H
#define __ADAPT_H

/*
 * USAGE: Include this header file anywhere that real numbers are used in your
 * program, and convert their definitions to use the "AD_real" type. You may
 * need to change some of your memory allocation routines to accomodate a class
 * type instead of an intrinsic type. For exactly *one* of your code modules
 * (the one with "main" is recommended), you must also add the following line
 * to enable the actual implementation routines:
 *
 *      #include <adapt-impl.cpp>
 */

#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>

#include "codi.hpp"
#include "ToolConfig.hpp"

/*
 * Optional macros to use for including source information for independent and
 * intermediate variables without having to manually change AD calls. Source
 * info is not useful for dependent variables so it is omitted.
 */

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define SOURCE_INFO "" __FILE__ ":" TOSTRING(__LINE__) ""

#define AD_INDEPENDENT(x,y)  AD_independent (x, y, SOURCE_INFO)
#define AD_INTERMEDIATE(x,y) AD_intermediate(x, y, SOURCE_INFO)
#define AD_INTERMEDIATE_ACC(x,y) AD_intermediate(x, y, SOURCE_INFO, true)
#define AD_INTERMEDIATE_ITER(x,y,z) AD_intermediate_iter(x, y, SOURCE_INFO, z)
#define AD_DEPENDENT(x,y,z)  AD_dependent   (x, y, z)

/*
 * Active real number type definition; use this instead of "double" for all
 * variables that are part of the important computation.
 *
 * The latter definitions use long doubles (80 bits) instead of regular doubles
 * to store gradients; this reduces the roundoff error in gradient calculations
 * at a small CPU and memory cost. If your compiler or architecture does not
 * support long doubles or if you don't want this additional overhead, re-enable
 * the original RealReverse active types.
 */

//typedef codi::RealReverse AD_real;
typedef codi::RealReverseGen<double, long double> AD_real;

/*
 * Start recording of computation for AD
 */
void AD_begin();

/*
 * Stop recording of computation for AD
 */
void AD_end();

/*
 * Extract the true ("primal") value of an active real number. Useful for
 * passing to output routines or other places where the use of the AD variable
 * isn't appropriate. A passthrough double version is also provided to help
 * automated tools insert ADAPT instrumentation more uniformly.
 */
double AD_value(const AD_real &var);
double AD_value(double var);

/*
 * Enable aggregation based on source info. Must be called before any of the
 * AD_independent/AD_intermediate/AD_dependent registration functions.
 */
void AD_enable_source_aggregation();

/*
 * Enable taking the absolute value of errors during variable analysis. Useful
 * when the program contains long chains of self-assignments (i.e., reads and
 * writes to the same variable). Must be called before AD_report.
 */
void AD_enable_absolute_value_error();

/*
 * Register an independent (input) variale.
 */
void AD_independent(AD_real &var, std::string label);
void AD_independent(AD_real &var, std::string label, std::string source);

/*
 * Register an intermediate variable.
 */
void AD_intermediate(AD_real &var, std::string label);
void AD_intermediate(AD_real &var, std::string label, std::string source, bool isAccVar=false);
void AD_intermediate_iter(AD_real &var, std::string label, std::string source, int iter);

/*
 * Register a dependent (output) variable and a tolerable error bound.
 */
void AD_dependent(AD_real &var, std::string label, double toleratedError);

/*
 * Register a dependent (output) variable with a requested number of significant
 * digits.
 */
void AD_dependent(AD_real &var, std::string label, int numDigits);

/*
 * Prints mixed-precision allocation recommendation.
 */
void AD_report();

#endif  // __ADAPT_H

