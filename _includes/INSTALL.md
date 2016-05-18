Installation 
---

To build and install **RestFrames** simply cd to the *RestFrames* home
directory and do:

	>$ ./configure
	>$ make
	>$ make install

Note: the above commands will attempt to install **RestFrames** in
your */usr/local* area. If you do not have write permission for this
area, or would like to install **RestFrames** somewhere else, you can
replace the *./configure* step above with:

	>$ ./configure --prefix=your_path_here

For example, you can simply install **RestFrames** in your current
directory:

	>$ ./configure --prefix=$PWD

See the *INSTALL* file for more details and options about configuring
and installing **RestFrames**.

Examples 
---

The RestFrames distribution includes a collection of examples
that can be run from the ROOT command line as macros. In order
to run these example, you must first set the RestFrames environmental
variables and library path:

    >$ source setup_RestFrames.sh(csh)

Example macros can then be run from the ROOT command line by typing:

    root [0] .x examples/example_macro.C++

A description of each of the example macros can be found in
the [examples section](/examples/).
