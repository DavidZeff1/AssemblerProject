this project will take input from an assembly-like langauge(from unparsedProg file) and will do the following:

First, the input will be parsed, including getting rid of empty new lines and comments(any lines that begin with';'), and will also identify macros(
macros will be defined like this:
mcro m1
  sub @r1, @r4
  bne END
endmcro

Anywhere in the program where there is a m1 initialized it will be replaced with 
  sub @r1, @r4
  bne END


) and parse them through the program, the file which does these tasks is called "macroParser".

all the parsed input will be written into a new file(Prog.as), and then we will convert our newly parsed information into binary and base 64. We will also keep track of entry and external variables and write them to separate files(Prog.ent,prog.ext) while the converted input will be written into  a "prog.ob" file. if there are errors they will be written into an error.txt file, all the above will be done in the "firstRun.c" file.

there are 15 type of operations:
ה
code , operation
0 mov
1 cmp
2 add
3 sub
4 not
5 clr
6 lea
7 inc
8 dec
9 jmp
10 bne
11 red
12 prn
13 jsr
14 rts
15 stop

and 7 registers 
@r1 - @r7

variables are defined when succeeded by a colon :
MAIN: mov @r3 ,LENGTH


entry and external variables are defined when preceded by '.extern' or'.entry':
.entry LENGTH
.extern W

there are also variables of type .data and .string which essentially are an array of numbers or an array of characters:
STR: .string “abcdef”
LENGTH: .data 6,-9,15
K: .data 22


