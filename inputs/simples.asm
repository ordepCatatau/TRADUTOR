section data
n1: const -10
n2: space 10
section text
jmp rot
rot: add n1
load n1
add n2 + 7
add n2 - 1
stop
