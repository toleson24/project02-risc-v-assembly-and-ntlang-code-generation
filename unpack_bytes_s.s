.global unpack_bytes_s

# Unpack the bytes of a 32 bit word (uint32_t) into individual bytes in an array
#
# a0 - int val
# a1 - uint32_t bytes[]
# t0 - int i
# t1 - int len = 4
# t2 - uint32_t temp
# t3 - int mask = 0xFF = 255

unpack_bytes_s:
    li t0, 0
	li t1, 4
	#mv t2, a0
	#li t3, 255
	
loop:
	bge t0, t1, done
	# andi t2, t2, 0xFF
 	andi t2, a0, 0xFF
	mul t4, t0, t1
	add t4, a1, t4
	sb t2, (t4)		# (a1)
	sraw a0, a0, 8
	addi t0, t0, 1
	j loop

done:
	mv a0, a1
	ret
