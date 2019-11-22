#result is 110
addiu $3, $0, 20
addiu $2, $0, 100
sub $2, $2, $3
bgezal $2, branch
addi $2, 10
addi $2, 10
jr $0
branch: addi $2, 10
jr $31