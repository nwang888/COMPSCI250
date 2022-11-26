.text

main:
    addi $sp, $sp, -4 #save ra
    sw $ra, 0($sp)

    
    li $a0, 1 #load immediates
    li $a1, 2

    jal foo

    move $t0, $v0 #retrieve foo's result from $v0
    
    li $v0, 1     #print int
    move $a0, $t0
    syscall

    lw $ra, 0($sp)
    addi $sp, $sp, 4 #restore ra

    jr $ra #end

foo:  
    add $v0, $a0, $a1 #sum and return in $v0
    jr $ra


.data