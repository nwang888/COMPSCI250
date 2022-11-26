.text
.align 2
my_strlen:

    li $t0, 0
    move $t1, $a0

_loop:
    lb		$t2, 0($s1)		# dereference string pointer
    beqz    $t2, _endloop	# if object pointed to by pointer == 0, i.e. '\0' end
    addi    $t1, $t1
    
    
    jr $ra

main:
    addiu 

.data