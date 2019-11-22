#result is 0
addiu $3, $0, 8
addiu $2, $0, 4
addi $4, $0, 12
des: sub $2, $2, $4
bgez $2, des
add $2, $2, $3
jr $0
nop
nop
