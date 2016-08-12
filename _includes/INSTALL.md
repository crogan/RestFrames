Installation 
---

To build and install **RestFrames** simply cd to the `RestFrames` home
directory and do:

	>$ ./configure
	>$ make
	>$ make install

Note: the above commands will attempt to install **RestFrames** in
your `/usr/local` area. If you do not have write permission for this
area, or would like to install **RestFrames** somewhere else, you can
replace the `>$ ./configure` step above with:

	>$ ./configure --prefix=your_path_here

For example, you can simply install **RestFrames** in your current
directory:

	>$ ./configure --prefix=$PWD

See the `RestFrames/INSTALL` file for more details and options about configuring
and installing **RestFrames**.

Examples 
---

The RestFrames distribution includes a collection of examples
that can be run from the ROOT command line as macros, or as compiled
executables. To run the compiled examples one can type:

    >$ ./examples_example_macro.x

In order to run these examples as ROOT macros, you must first set the RestFrames environmental
variables and library paths for auto-loading of the RestFrames shared library:

    >$ source setup_RestFrames.sh(csh)

Example macros can then be run from the ROOT command line by typing:

    root [0] .x examples/example_macro.C+

NOTE: Running example macros without the `+` option (without ROOT
ACLiC compilation) is currently not supported in ROOT5. In ROOT6,
users can run the example macros uncompiled with the CLING
interpreter:

    root [0] .x examples/example_macro.C

or with ACLiC compilation. In this latter case, due to an unresolved
issue with automatic loading of shared libraries, users must first
declare an instance of a **RestFrames** class in the ROOT command line
environment before running the macro:

	root [0] RestFrames::RFKey load_libRestFrames(1);
    root [1] .x examples/example_macro.C+

With respect to this last limitation, if users open ROOT from the
`RestFrames` or `RestFrames/examples` directory the existing
`rootlogon.C` files contained therein with automatically ensure a
class instance is declared (assuming the user is not using an
alternatice `rootlogon.C` file).

A description of each of the available examples can be found in
the [examples](/examples/) page.
