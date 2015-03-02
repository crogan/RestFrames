Installation 
---

To make the shared library **libRestFrames.so** simply do:

    >$ make

which will create **lib/libRestFrames.so**

Examples 
---

The RestFrames distribution includes a collection of examples
that can be run from the ROOT command line as macros. In order
to run these example, you must first set the RestFrames environmental
variables and library path:

    >$ source setup.sh

Example macros can then be run from the ROOT command line by typing:

    root [0] .x examples/example_macro.C++

A description of each of the example macros can be found in
the [examples section](/examples/).
