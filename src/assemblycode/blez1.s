#result is 210
addi $2,$0,206
blez $2,branch
addi $2,$0,1
addi $2,$0,2
branch: addi $2,$2,1
jr $0
