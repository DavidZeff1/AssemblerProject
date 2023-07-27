Assembler Project:

This repository contains a program for processing raw data entered by the user, parsing it for macros, empty lines, and comments, and then converting it to base64 machine code. The program outputs several files based on the successful execution or any errors encountered during the process.

The main purpose of this program is to process the raw data entered by the user and perform the following steps:

unParsedProg: This folder holds the raw data entered by the user before being parsed for macros, empty lines, and comments.

macroparser.c: This C file contains the code to parse the data from the unParsedProg folder. It processes the input and creates a new file called prog.as. The parsing includes handling comments, newlines, and macros in the code.

firstRun.c: This C file takes the data from prog.as and converts it to base64 machine code. If no errors occur during the conversion process, it will output three files: prog.ob, prog.ent, and prog.ext.
