#cin 0x20, cout 0x22, result is 34
lui $14, 0x3000
lw  $2,0($14) #load into putc
addiu $2,$2,2
addiu $14,$14,4
sb  $2,0($14)
jr  $0
