#result is -3(253)
addi $3, $0, 1
addi $2, $0, 4
addi $4, $0, 4
simon: sub $2, $2, $4
bgez $2, simon
add $2, $2, $3
jr $0
nop
nop
