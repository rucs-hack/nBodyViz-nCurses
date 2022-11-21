This is a Header based Cross Platform library that can be dropped into any program that needs to use the mop file format for n-body model time series recording and playback.
The only external dependancy it has is zlib, thereare compression and decompresion methods that are as yet unused, but will be used to compress the mopfile as much as is possible given the reader requirements.
There are compile time options - MOPFILE and MOPFILELIGHT, which determine what class members MopItem has. These defines go in the Makefile 
of your project.

The included Main.cpp and Makefile are for CI purposes only. You can see how to use the defines by reading the Makefile, and the Main.cpp file.
will expand in test capability over time.

When using Mopfile, ignore these files.