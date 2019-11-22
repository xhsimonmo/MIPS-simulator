#result is 18
addi $4, $0, 20
addi $2, $0, 4
addi $3, $0, 6
branch: sub $2, $2, $3
bgtz $2, branch
add $2, $2, $4
jr $0
