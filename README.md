# ADAPT: Algorithmic Differentiation for Floating-Point Precision Tuning

This repository contains the source code for the ADAPT tool [1].

[1] Harshitha Menon, Michael O. Lam, Daniel Osei-Kuffuor, Markus Schordan, Scott
Lloyd, Kathryn Mohror, and Jeffrey Hittinger. 2018. ADAPT: algorithmic
differentiation applied to floating-point precision tuning. In Proceedings of
the International Conference for High Performance Computing, Networking,
Storage, and Analysis (SC '18). IEEE Press, Piscataway, NJ, USA, Article 48, 13
pages. [ACM DL Link](https://dl.acm.org/citation.cfm?id=3291720)

## Prerequisites

ADAPT depends on [CoDiPack](https://github.com/SciCompKL/CoDiPack) [2], a template
expression-based library for C++11. This library must be accessible to the
compiler, possibly by adding `-I/path/to/CoDiPack/folder` to `CXXFLAGS`.

[2] Max Sagebaum, Tim Albring, and Nicolas R. Gauger. 2017. High-Performance
Derivative Computations using CoDiPack. arXiv preprint arXiv:1709.07229.

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
intermediate results using the `AD_INTERMEDIATE` macro, and any output variables
(and their error threshold) using the `AD_DEPENDENT` macro. Call `AD_begin()`
before any code that you wish to analyze, and call `AD_report()` after all of
the code that you wish to analyze.

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

