.global rstr_rec_s

# Reverse a string recursively
#
# a0 - char *dst
# a1 - char *src

# t0 - int src_len
# t1 - int dst_len

rstr_rec_s:
	addi sp, sp, -32
	sd ra, (sp)
	sd a0, 8(sp)
	sd a1, 16(sp)

	mv a0, a1
	call strlen
	mv a3, a0

	ld ra, (sp)
	ld a0, 8(sp)
	ld a1, 16(sp)
	addi sp, sp, 32

	addi sp, sp, -64
	sd ra, (sp)
	sd a0, 8(sp)
	sd a1, 16(sp)
	sd a2, 24(sp)
	sd a3, 32(sp)

	mv a2, a1
	li a1, 0
	addi a3, a3, -1	
	call basecase

	# lw ra, (sp)
	# lw a0, 8(sp)
	# lw a1, 16(sp)
	# lw a2, 24(sp)
	# lw a3, 32(sp)
	# addi sp, sp, 64
	
	j done


basecase:
	add t0, a0, a1			# dst_idx
	add t1, a2, a3			# src_idx
	lw t2, (t0)				# dst[dst_idx]
	lw t3, (t1)				# src[src_idx]
	
	# bne t3, zero, recstep
	# bnez t3, recstep
	bge a3, zero, recstep
	sw x0, (t0)				# t3 ?
	
	j done


recstep:
	sw t3, (t0)
	
	addi sp, sp, -80
	# sw ra, (sp)
	sw a0, 8(sp)
	sw a1, 16(sp)
	sw a2, 24(sp)
	sw a3, 32(sp)
	sw t0, 40(sp)
	sw t1, 48(sp)
	sw t2, 56(sp)
	sw t3, 64(sp)

	addi a1, a1, 1
	addi a3, a3, -1	
	call basecase

	# lw ra, (sp)
	lw a0, 8(sp)
	lw a1, 16(sp)
	lw a2, 24(sp)
	lw a3, 32(sp)
	lw t0, 40(sp)
	lw t1, 48(sp)
	lw t2, 56(sp)
	lw t3, 64(sp)
	addi sp, sp, 80


done:
	ld ra, (sp)
	ld a0, 8(sp)
	ld a1, 16(sp)
	ld a2, 24(sp)
	ld a3, 32(sp)
	addi sp, sp, 64
	
	ret
