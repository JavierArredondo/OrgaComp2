addi $t0, $zero, 5
addi $t1, $zero, 1

iteracion:
mul $t1, $t1, $t0
subi $t0, $t0, 1
beq $t0, $zero, exit
j iteracion

exit: