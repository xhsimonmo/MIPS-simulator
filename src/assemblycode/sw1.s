addi $3,$3,0xfffe
lui $4,0x2000
sw $3,0($4)
lb $2,0($4)
srl $2,$2,16 #$2 should be 0x0000ffff
jr $0

#should be 255 
