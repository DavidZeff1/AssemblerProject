.entry LENGTH
.extern W
MAIN: mov @r3 ,LENGTH
LOOP: jmp L1
prn -5
  bne W
sub @r1, @r4
bne L3
L1: inc K
.entry LOOP
not @r1, @r4
clr @r1, @r4
inc @r1, @r4
dec @r1, @r4
jmp @r1, @r4
jmp W
END: stop
STR: .string “abcdef”
LENGTH: .data 6,-9,15
K: .data 22
.extern L3
