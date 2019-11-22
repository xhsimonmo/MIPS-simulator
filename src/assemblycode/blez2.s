#result is 8
addi $3, $0, 4
addi $2, $0, -4
branch: add $2, $2, $3
blez $2, branch 
add $2, $2, $3
nop
nop
jr $0
