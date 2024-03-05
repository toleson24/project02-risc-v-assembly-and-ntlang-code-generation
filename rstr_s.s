.global rstr_s
.global strlen
.global swap_s

# Swap chars at indices a1 & a2 in array a0
#
# a0 - char arr[]
# a1 - int i
# a1 - int j
# t0-t5 - char temp

swap_s:
 	add t1, a1, a0
	add t2, a2, a0 
	#lb t3, (t1)		# arr[i]
	#lb t4, (t2)		# arr[j]

	#mv t5, t3
	#mv t3, t4
	#mv t4, t5

	#sb t3, (t1)
	#sb t4, (t2)

	lb t3, 0(t1)
	lb t4, 0(t2)
	
	sb t4, 0(t1)
	sb t3, 0(t2)

	ret


# Reverse a string iteratively
#
# a0 - char arr[] (string)
# a1 - int len
# t0 - int i
# t1 - int j

rstr_s:
	addi sp, sp, -16
	sw ra, (sp)
	sb a0, 8(sp)	

	call strlen
	mv a1, a0 

	lw ra, (sp)
	lb a0, 8(sp)
	addi sp, sp, 16

	mv t0, a1
	addi t0, t0, -1
	li t1, 0 
	
loop:
	blt t0, t1, done

	addi sp, sp, -48
	sw ra, (sp)
	sb a0, 8(sp)
	sb a1, 16(sp)
	sb t0, 24(sp)
	sb t1, 32(sp)

	mv a1, t1
	mv a2, t0
	call swap_s	

	lw ra, (sp)
	lb a0, 8(sp)
	lb a1, 16(sp)
	lb t0, 24(sp)
	lb t1, 32(sp)
	addi sp, sp, 48
	
	addi t0, t0, -1
	addi t1, t1, 1
	j loop		

done:
	ret
    
