.data
n: .word 6

.text
main:
    la t0, n
    lw a0, 0(t0)
    jal ra, factorial
    
    addi a1, a0, 0
    addi a0, x0, 1
    ecall # Print Result
    
    addi a0, x0, 10
    ecall # Exit

factorial:
    # YOUR CODE HERE
    addi t2 x0 1
    beq a0 t2 exit
    addi sp sp -8
    sw ra 0(sp)
    sw a0 4(sp)
    addi a0 a0 -1
    jal ra, factorial
    addi t1 a0 0
    lw ra 0(sp)
    lw a0 4(sp)
    addi sp sp 8
    mul a0 t1 a0
mark:
    j end
    
exit:
    j mark

end:
    jr ra
    
