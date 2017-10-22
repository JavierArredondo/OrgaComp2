addi $t0, $zero, 2
addi $t1, $zero, 2
beq $t0, $t1, NEXT
addi $t1, $t0, 3
subi $t2, $t1, 1
mul $t2, $t2, $t2
add $t7, $t2, $t2
addi $sp, $sp, -8
sw $t7, 0($sp)
NEXT:
add $t1, $t0, 1