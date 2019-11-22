lui $3, 0x3000
addi $4,$0,12
sw $4,4($3) #should output/cout 12
addi $5,$0,5
lui $6,0x2000
sw $5,0($6)
lw $2,0($6)
jr $0
# $2 should be 5
