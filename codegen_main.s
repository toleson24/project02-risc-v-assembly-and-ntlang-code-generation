#! risc-v

.data
	array:  .word 0, 0, 0, 0, 0, 0, 0, 0

.text
.global codegen_func_s
.global atoi

# Parameters
cogegen_func_s:
    # TODO


main:
	# TODO
	li t1, 0				# i = 0
	li t2, 8				# len = 8
	#li t3, 0				# return value
	
	la t0, array


#init_loop:
#    bge t0, t1, reset_t1
#	slli t1, t0, 2
#    add t1, a, t1
#    sw zero, 0(t1)
#	addi t0, t0, 
#	j init_loop
	
       
#reset_t1:
#    li t1, 0


populate_loop:
    bge t1, a0, func_call
	bge t1, t2, func_call
	
	addi sp, sp, -64
	sd ra, (sp)
	sd a0, 8(sp)
	sd a1, 16(sp)
	sd t0, 24(sp)
	sd t1, 32(sp)
	sd t2, 40(sp)
	#sd t3, 48(sp)

	li t2, 4
	mul t2, t2, t1
	add t1, a1, t2
	mv a0, t1
	
	call atoi
	mv t3, a0				# t3 = atoi(argv[i])

	ld ra, (sp)
	ld a0, 8(sp)
	ld a1, 16(sp)
	ld t0, 24(sp)
	ld t1, 32(sp)
	ld t2, 40(sp)
	#ld t3, 48(sp)
	addi sp, sp, 64

	addi t1, t1, -1			# i = i - 1
	li t4, 4
	mul t4, t4, t1
	add t4, t0, t4
	sd t3, (t4)				# a[i - 1] = t3
	
	addi t1, t1, 1
	j populate_loop


func_call:
	addi sp, sp, -64
	sd ra, (sp)
	sd a0, 8(sp)
	sd a1, 16(sp)
	sd t0, 24(sp)
	sd t1, 32(sp)
	sd t2, 40(sp)
	sd t3, 48(sp)

	call codegen_func_s

	ld ra, (sp)
	ld a0, 8(sp)
	ld a1, 16(sp)
	ld t0, 24(sp)
	ld t1, 32(sp)
	ld t2, 40(sp)
	ld t3, 48(sp)
	addi sp, sp, 64

	# TODO print


done:
    ret
