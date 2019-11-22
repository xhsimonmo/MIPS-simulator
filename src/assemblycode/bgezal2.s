#result is 20
addiu $3, $0, 20
addiu $2, $0, 100
add $2, $3, $2
bgezal $2, branch
addi $2, 10
addu $2, $0, $31
jr $0
branch: addi $2, 10
addi $2, 10
jr $31
