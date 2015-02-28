---
RestFrames: Particle Physics Event Analysis Library {#mainpage}
=================================
Developed by Christopher Rogan (crogan@cern.ch)

Copyright (c) 2014-2015, Christopher Rogan

Installation 
---

To make the shared library _libRestFrames.so_ simply do:

    >$ make

which will create _lib/libRestFrames.so_

Examples 
---

The RestFrames distribution includes a collection of examples
that can be run from the ROOT command line as macros. In order
to run these example, you must first set the RestFrames environmental
variables and library path:

    >$ source setup.sh

Example macros can then be run from the ROOT command line by typing:

    root [0] .x macros/example_macro.C++
