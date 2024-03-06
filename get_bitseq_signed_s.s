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
	sd ra, (sp)
	sd a0, 8(sp)
	sd a1, 16(sp)
	sd a2, 24(sp)
	sd t0, 32(sp)
	sd t1, 40(sp)
	sd t2, 48(sp)

	call get_bitseq_s
	mv t3, a0	
	
	ld ra, (sp)
	ld a0, 8(sp)
	ld a1, 16(sp)
	ld a2, 24(sp)
	ld t0, 32(sp)
	ld t1, 40(sp)
	ld t2, 48(sp)
	addi sp, sp, 64
	
	sllw t3, t3, t2
	
	blt t3, zero, twos_complement
	j done	


twos_complement:
	#neg t3, t3		# why is this unncessary here? how is the full two's complement process applied?
	addi t3, t3, 1


done:
	srl t0, t3, t2

	mv a0, t0
	ret
