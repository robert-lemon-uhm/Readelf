# ReadELF
A replication of linux's readelf program, completed for EE 491 (Software Reverse Engineering). This project implements the -h, -t, and -x options. 

## Setup
In order to run this project, simply open the repository in a linux-like environment and type `make` to build the program. This generates an executable that can be run 
via `./Readelf`. 

## File Header Option
`./Readelf -h <file>`


Including the `-h` option will print the file header for an ELF file. This metadata provides information about the file such as its endian-ness and addressing types. 
More information about ELF headers, including the byte-by-byte format, can be found [here](https://en.wikipedia.org/wiki/Executable_and_Linkable_Format).

## Section Details Option
`./Readelf -t <file>`


Including the `-t` option prints the section headers for each section within the file. Section headers include section metadata like dthe type, size, and offset 
(section location with the file).

## Hex Dump Option
`./Readelf -x <name> <file>`


Including the `-x` prints a hexdump of the section specified by `<name>`. This hex dump includes the hex values of the section as well their ASCII equivalent. 

TODO: Allow users to provide the section number instead of the section name. 
