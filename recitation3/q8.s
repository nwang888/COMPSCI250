.text

main:
    addi $sp, $sp, -4 #save ra
    sw $ra, 0($sp)

    
    li $s0, 1 #load immediates into s
    li $s1, 2

    move $a0, $s0  #move stuff into a registers
    move $a1, $s1

    jal foo

    add $t5, $v0, $s1  #adds return from foo, s0, and s1 into t5
    add $t5, $s5, $t0 
    
    li $v0, 1     #print int
    move $a0, $t5
    syscall

    lw $ra, 0($sp)
    addi $sp, $sp, 4 #restore ra

    jr $ra #end

foo:  
    addi $sp, $sp, -8 # save t registers
    sw $s0, 0($sp)
    sw $s1, 4($sp)

    li $s0, 0   #mess with s registers in foo
    li $s1, 0
    add $v0, $a0, $a1   #sum a0 and a1

    lw $s1, 4($sp)
    lw $s0, 0($sp)
    addi $sp, $sp, 8

    jr $ra
.data