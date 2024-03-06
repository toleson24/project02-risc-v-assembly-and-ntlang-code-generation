.global rstr_s
.global strlen
.global swap_s

# Put char at index a3 in a1 (array) in index a2 in a0 (array).
#
# a0 - char dst[]
# a1 - char src[]
# a2 - int i
# a3 - int j
# t0-t3 - char temp

swap_s:
 	add t0, a2, a0
	add t1, a3, a1 

	lb t2, 0(t0)
	lb t3, 0(t1)
	
	sb t3, 0(t0)
	sb t2, 0(t1)

	ret


# Reverse a string iteratively
#
# a0 - char dst[]
# a1 - char src[]
# t0 - int i
# t1 - int j

rstr_s:
	addi sp, sp, -32
	sd ra, (sp)
	sd a0, 8(sp)	
	sd a1, 16(sp)
	
	mv a0, a1
	call strlen
	mv t0, a0 

	ld ra, (sp)
	ld a0, 8(sp)
	ld a1, 16(sp)
	addi sp, sp, 32
	
	addi t0, t0, -1
	li t1, 0 
	
loop:
	blt t0, x0, done

	addi sp, sp, -64
	sd ra, (sp)
	sd t0, 24(sp)
	sd t1, 32(sp)

	mv a2, t1
	mv a3, t0
	call swap_s	

	ld ra, (sp)
	ld t0, 24(sp)
	ld t1, 32(sp)
	addi sp, sp, 64
	
	addi t0, t0, -1
	addi t1, t1, 1
	j loop		

done:
	ret
    
