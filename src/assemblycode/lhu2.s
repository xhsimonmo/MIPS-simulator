addi $3,$3,0xfff0
lui $10,0x2345
sw $3,0($10)
lhu $2,2($10)
jr $0

#$2 is 240
