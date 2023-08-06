this project will take input from an assembly-like langauge(from unparsedProg file) and will do the following:

first the iput will be parsed, included getting rid of empty new lined and comments(any lines the begins with';'), and will also identify macros and parse them through the program, the file which does these tasks is called "macroParser".

all the parsed input will be written into a new file(Prog.as), and then we will convert our newly parsed input into binary and then into base 64 and we will also keep track of entry and external variables and write them to seperate files(Prog.ent,prog.ext) while the converted input will be written into  a "prog.ob" file. if there are errors they will be written into a error.txt file, all the above will be done in the "firstRun.c" file.