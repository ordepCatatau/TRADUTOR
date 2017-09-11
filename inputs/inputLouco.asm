section data



;lollolol


vetor: space 15
n1: space
n2: const 21
label: equ 0
aux: equ n1

section text
Macro_1: macro
add n1
sub n2
input n1

end


jmpp rot2
Macro_1
if label
rot2: sub n1 - 10
load n2 - 13
add aux

stop





