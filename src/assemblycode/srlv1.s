#result is 255(8 1 bits)
lui $3, 0xffff
addi $4, $0, 16
srlv $2, $3, $4
jr $0