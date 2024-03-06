.global get_bitseq_signed_s
.global get_bitseq_s

# a0 - uint32_t num
# a1 - int start
# a2 - int end

# t0 - int val_signed
# t1 - int len = (end - start) + 1
# t2 - int shift = 32 - len
# t3 - int val

get_bitseq_signed_s:
	sub t1, a2, a1
	addi t1, t1, 1
	li t3, 32
	sub t2, t3, t1	

	addi sp, sp, -64
	sw ra, (sp)
	sw a0, 8(sp)
	sw a1, 16(sp)
	sw a2, 24(sp)
	sw t0, 32(sp)
	sw t1, 40(sp)
	sw t2, 48(sp)

	call get_bitseq_s
	mv t3, a0	
	
	lw ra, (sp)
	lw a0, 8(sp)
	lw a1, 16(sp)
	lw a2, 24(sp)
	lw t0, 32(sp)
	lw t1, 40(sp)
	lw t2, 48(sp)
	addi sp, sp, 64
	
	sllw t3, t3, t2
	srlw t0, t3, t2	

	mv a0, t0
	ret
