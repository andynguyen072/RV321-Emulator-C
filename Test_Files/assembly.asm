.text
addi t5, t0, 5
addi t6, t5, -10
addi t7, t6, 3
add t28, t5, t6
sub t29, t6, t7
sll t5, t5, t5
srl t31, t7, t29
sra t30, t29, t28
or t29, t7, t7
and t28, t30, t31
xor t7, t29, t6
slt t30, t28, t7
sltu t30, t7, t28
