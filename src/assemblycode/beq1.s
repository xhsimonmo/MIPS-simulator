#result is 20
addiu $3, $0, 8
addiu $2, $0, 4
des: addiu $2, $2, 4
beq $3, $2, des
addu $2, $2, $3
jr $0
nop
nop