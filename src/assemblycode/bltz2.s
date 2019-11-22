#result is 21
addi $2, $0, -3
addi $4, $0, 20
branch: addi $2, $2, 2
bltz $2, branch
add $2, $2, $4
jr $0
