RestFrames {#mainpage}
=============


written by C. Rogan (crogan@cern.ch) Jan '15

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
