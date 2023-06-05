# avro-C-intro
A little Avro C hello world to familiar myself with Avro.


# Installation
Avro's documentation for installation: https://github.com/apache/avro/blob/master/lang/c/INSTALL
1. I set up Ubuntu environment using WSL
2. Before installing Avro, we need to install GCC *gcc*, CMake *cmake*, Jansson *libjansson-dev*, and *pkg-config* (for Jansson)
3. I also installed *plocate*, *valgrind*, and *make*
4. Download the Avro repo and follow installation steps in the Avro installation documentation linked above (the build folder you create should be in avro/lang/c)
5. After successfully installing Avro, the Avro header files should be in /include and libavro.so should be in /lib or /lib64

# Compilation
1. If on WSL, you may have linking errors like me. To fix, link the correct paths while compiling, something like this: -I/include -L/lib
2. You also need to link this: -lavro, AFTER you link the object files (after thr -o), or just at the very end of your compilation command
