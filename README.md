# avro-C-intro
A simple Avro C program to familiar myself with Avro.

# Goal
Implement from scratch a simple Avro C hello world that does the following:
- Reads a schema (pick a thing to implement, like a person with a name, phone number, birthday, and address)
- Creates a value using that schema
- Fill out the value with information
- Writes the value to file
- Reads value from file
- Prints data from the schema to screen

# Installation
Avro's documentation for installation: https://github.com/apache/avro/blob/master/lang/c/INSTALL
1. I set up Ubuntu environment using WSL
2. Before installing Avro, we need to install GCC `gcc`, CMake `cmake`, Jansson `libjansson-dev`, and `pkg-config` (for Jansson)
3. I also installed `plocate`, `valgrind`, and `make`
4. Download the Avro repo and follow installation steps in the Avro installation documentation linked above (the build folder you create should be in ```avro/lang/c```)
5. After successfully installing Avro, the Avro header files should be in ```/include``` and ```libavro.so``` should be in ```/lib``` or ```/lib64```

# Compilation
1. If on WSL, you may have linking errors like me. To fix, link the correct paths while compiling, add something like this: ```-I/include -L/lib```
2. You also need to link this: ```-lavro```, AFTER you link the object files (after the ```-o```), or just at the very end of your compilation command

# Makefile
`make hello_world`: compiles source code<br>
`make mem`: runs executable with valgrind<br>
`make clean`: removes all object files and executables<br>

# Program
1. writes to `people.test.avro`
2. reads from `people.test.avro`
3. prints to screen (`stderr`)

# Tests
Created Avro and text files with different number of people to compare the file sizes
