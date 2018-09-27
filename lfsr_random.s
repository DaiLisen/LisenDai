.data

lfsr:
	.align 4
	.word 0x1

.text

# Implements a 16-bit lfsr
#
# Arguments: None
lfsr_random:
	la t0 lfsr
	lhu a0 0(t0)

	# Your Code Here
addi s0 s0 16
LOOP:
beq t0 s0 exit
srli t1 a0 0
srli t2 a0 2
srli t3 a0 3
srli t4 a0 5
xor t5 t1 t2
xor t5 t5 t3
xor t5 t5 t4
slli t5 t5 15
srli t6 a0 1
add t6 t6 t5
addi a0 t6 0
addi t0 t0 1
j LOOP
exit:
addi s0 s0 0

	la t0 lfsr
	sh a0 0(t0)
	jr ra
