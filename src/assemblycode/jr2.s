#result is 10
addi $2, $0, 5
addi $3, $0, 10
addi $4, $0, 12
branch: addi $2, $2, 1
beq $2, $3, finish
jr $4
finish: jr $0
