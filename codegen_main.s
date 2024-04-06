#! risc-v
.global main
.global codegen_func_s
.global atoi
.global printf

.data
	format_string:	
		.string "%d (0x%X)\n"   # Example format string
    	.byte 0

.text
# Parameters
# TODO

main:
	ld t0, 24(sp)					# a[8]
	# sub sp, sp, t0
	# sd, t0, (sp)
	li t1, 0					# i = 0
	li t2, 8					# len = 8
	li t3, 0


initialize_loop:
	bge t1, a0, pre_populate	# if i >= 8, branch
	mul t4, t2, t1
	add t4, t0, t4				
	sd t3, 0(t4)				# a[i] = 0
	addi t1, t1, 1				# i++
	j initialize_loop


pre_populate:
	li t1, 1					# i = 1, argv[0] = program


populate_loop:
    bge t1, a0, func_call
	bge t1, t2, func_call
	
	addi sp, sp, -104
	sd ra, (sp)
	sd a0, 8(sp)
	sd a1, 16(sp)
	sd t0, 24(sp)
	sd t1, 88(sp)
	sd t2, 96(sp)

	li t2, 8					# 8-bit ptr
	mul t2, t2, t1
	add t1, a1, t2
	mv a0, t1
	
	call atoi
	mv t3, a0					# t3 = atoi(argv[i])

	ld ra, (sp)
	ld a0, 8(sp)
	ld a1, 16(sp)
	ld t0, 24(sp)
	ld t1, 88(sp)
	ld t2, 96(sp)
	addi sp, sp, 104

	addi t1, t1, -1				# i = i - 1
	li t4, 8					# 8-bit arr[int]
	mul t4, t4, t1
	add t4, t0, t4
	sd t3, (t4)					# a[i - 1] = t3
	
	addi t1, t1, 1	
	addi t1, t1, 1
	j populate_loop


func_call:
	addi sp, sp, -120
	sd ra, (sp)
	sd a0, 8(sp)
	sd a1, 16(sp)
	sd t0, 24(sp)
	sd t1, 88(sp)
	sd t2, 96(sp)
	sd t3, 104(sp)
	
	ld a0, (t0)
	ld a1, 8(t0)
	ld a2, 16(t0)
	ld a3, 24(t0)
	ld a4, 32(t0)
	ld a5, 40(t0)
	ld a6, 48(t0)
	ld a7, 56(t0)
	call codegen_func_s
	mv t4, a0	

	la a0, format_string
	mv a1, t4
	mv a2, t4
	call printf

	ld ra, (sp)
	# ld a0, 8(sp)
	# ld a1, 16(sp)
	# ld t0, 24(sp)
	# ld t1, 88(sp)
	# ld t2, 88(sp)
	#ld t3, 104(sp)
	addi sp, sp, 120
	

done:
    ret


codegen_func_s:
