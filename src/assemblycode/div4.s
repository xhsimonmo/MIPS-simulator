#result is 14
addiu $3, $0, 66
addiu $4, $0, 5
div $3, $4
mfhi $5
mflo $6
addu $2, $5, $6
jr $0
