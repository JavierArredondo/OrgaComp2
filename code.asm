ENTRY:
addi $t1, $zero, 0
addi $t0, $zero, 0
j EXIT
beq $t1, $zero, ENTRY
add $t0, $t7, $t2
EXIT:
add $t5, $t7, $t2
add $t3, $t7, $t2
add $t6, $t7, $t2