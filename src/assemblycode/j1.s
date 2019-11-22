#result is 10
addi $2, $0, 5
addi $3, $0, 10
branch: addi $2, $2, 1
beq $2, $3,end
j branch
end: jr $0
