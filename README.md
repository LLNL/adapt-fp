# ADAPT: Algorithmic Differentiation for Floating-Point Precision Tuning

This repository contains the source code for the ADAPT tool [1], which allows
you to instrument your own C++ code and analyze it with the aim of finding a
mixed-precision version (i.e., some double-precision variables have been changed
to single precision).

ADAPT performs algorithmic (or "automatic") differentiation [2] and then uses
the partial derivatives ("adjoints") to estimate the error introduced by
changing each variable's type. It then recommends variables that should be
replaced to maximize the amount of conversions under an error threshold that you
provide.

[1] Harshitha Menon, Michael O. Lam, Daniel Osei-Kuffuor, Markus Schordan, Scott
Lloyd, Kathryn Mohror, and Jeffrey Hittinger. 2018. ADAPT: algorithmic
differentiation applied to floating-point precision tuning. In Proceedings of
the International Conference for High Performance Computing, Networking,
Storage, and Analysis (SC '18). IEEE Press, Piscataway, NJ, USA, Article 48, 13
pages. [ACM DL Link](https://dl.acm.org/citation.cfm?id=3291720)

[2] Wikipedia article: "[Automatic
Differentiation](https://en.wikipedia.org/wiki/Automatic_differentiation)"

## Prerequisites

ADAPT depends on [CoDiPack](https://github.com/SciCompKL/CoDiPack) [3], a template
expression-based library for C++11. This library must be accessible to the
compiler, possibly by adding `-I/path/to/CoDiPack/folder` to `CXXFLAGS`.
You will also need to use the `-DCODI_ZeroAdjointReverse=0` preprocessor flag
to allow proper access to intermediate adjoints.

You should also consider using `-DCODI_EnableImplicitConversion
-DCODI_DisableImplicitConversionWarning` to enable implicit casts from `AD_real`
to `double`. This allows you to avoid some `AD_value()` calls (see Using ADAPT
below) but it could also hide unintended conversions (causing a loss of adjoint
information).

[3] Max Sagebaum, Tim Albring, and Nicolas R. Gauger. 2017. High-Performance
Derivative Computations using CoDiPack. arXiv preprint arXiv:1709.07229.
[arXiv Link](https://arxiv.org/abs/1709.07229)

## Getting Started

To build your project with ADAPT, include the ADAPT folder as an include path by
adding `-I/path/to/ADAPT/folder` to `CXXFLAGS`. Also, you must compile with the
`-std=c++11` option.

Add the following to each source code file that needs to reference ADAPT:

```c
#include <adapt.h>
```

Add the following to exactly one source code file (usually the one with your
`main`):

```c
#include <adapt-impl.cpp>
```

## Using ADAPT

Change any variables that you wish to analyze from `float` or `double` to
`AD_real`. Tag any independent variables using the `AD_INDEPENDENT` macro,
intermediate results (usually any assignment to the variables of interest)
using the `AD_INTERMEDIATE` macro, and any output variables
(and their error threshold) using the `AD_DEPENDENT` macro. Call `AD_begin()`
before any code that you wish to analyze, and call `AD_report()` after all of
the code that you wish to analyze. Call `AD_value()` to convert `AD_real`
variables to a `double` where necessary (e.g., for output).

There are also a couple of options that you can set to change the behavior of
the analysis. These options are enabled using function calls:

* `AD_enable_absolute_value_error()` - Enable taking the absolute value of
  errors during variable analysis. Useful when the program contains long chains
  of self-assignments (i.e., reads and writes to the same variable). Must be
  called before `AD_report`.

* `AD_enable_source_aggregation()` - Enable aggregation based on source info of
  assignments rather than variable definitions. Must be called before any of the
  `AD_INDEPENDENT`/`AD_INTERMEDIATE` macros.

The included demos (in the `sanity`, `sum2pi_x`, and `arclength` folders)
provide examples of how to use ADAPT. Consult the README files in those folders
for more information about the individual demos.

## Getting Involved

To get involved, submit an issue or email the authors directly.

## Contributing

To contribute, submit a pull request or email the authors directly.

## Release

ADAPT is released under an GPL license. For more details see the NOTICE and
LICENSE files.

LLNL-CODE-762758

ADAPT uses the "JSON for Modern C++" library, distributed under the MIT license:

```
    __ _____ _____ _____
 __|  |   __|     |   | |  JSON for Modern C++
|  |  |__   |  |  | | | |  version 3.1.2
|_____|_____|_____|_|___|  https://github.com/nlohmann/json

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2013-2018 Niels Lohmann <http://nlohmann.me>.
```

