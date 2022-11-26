.text

main:
    addi $sp, $sp, -4 #save ra
    sw $ra, 0($sp)

    
    li $t0, 1 #load immediates
    li $t1, 2

    move $a0, $t0  #move stuff into a registers
    move $a1, $t1

    addi $sp, $sp, -8 # save t registers
    sw $t0, 0($sp)
    sw $t1, 4($sp)

    jal foo

    lw $t1, 4($sp)
    lw $t0, 0($sp)
    addi $sp, $sp, 8

    add $t5, $v0, $t1  #adds return from foo, t0, and t1 into t5
    add $t5, $t5, $t0 
    
    li $v0, 1     #print int
    move $a0, $t5
    syscall

    lw $ra, 0($sp)
    addi $sp, $sp, 4 #restore ra

    jr $ra #end

foo:  
    li $t0, 0   #mess with t registers in foo
    li $t1, 0
    add $v0, $a0, $a1   #sum a0 and a1
    jr $ra


.data