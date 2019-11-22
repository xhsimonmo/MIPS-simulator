#result is -4
addi $3, $0, 0
addi $2, $0, 4
addi $4, $0, 4
des: sub $2, $2, $4
bgez $2, des
add $2, $2, $3
jr $0
nop
nop
