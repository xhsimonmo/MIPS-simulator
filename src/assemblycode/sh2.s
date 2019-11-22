#result is 5
lui $3, 0x2000
addi $2, $0, 5
sh $2, 4($3)
addi $2, $0, 9
lw $2, 4($3)
srl $2, $2, 16
jr  $0
