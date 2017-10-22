ENTRY:
addi $t0, $zero, 0
addi $t1, $zero, 0
beq $t0, $zero, EXIT
j ENTRY
add $t0, $t7, $t2
EXIT:
add $t6, $t7, $t2
