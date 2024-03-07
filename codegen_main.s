#! risc-v

.data
	array:  
		.word 0, 0, 0, 0, 0, 0, 0, 0
	format_string:	
		.string "%d (0x%X)\n"   # Example format string
    	.byte 0

.text
.global main
.global codegen_func_s
.global atoi
.global printf

# Parameters


main:
	li t1, 0				# i = 0
	li t2, 8				# len = 8
	#li t3, 4				# int width = 4
	
	la t0, array


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
	
	ld a0, (t0)
	ld a1, 4(t0)
	ld a2, 8(t0)
	ld a3, 12(t0)
	ld a4, 16(t0)
	ld a5, 20(t0)
	ld a6, 24(t0)
	ld a7, 28(t0)
	call codegen_func_s
	# call test_function_1
	mv t4, a0	

	ld ra, (sp)
	ld a0, 8(sp)
	ld a1, 16(sp)
	ld t0, 24(sp)
	ld t1, 32(sp)
	ld t2, 40(sp)
	ld t3, 48(sp)
	addi sp, sp, 64

	addi sp, sp, -64
	sd ra, (sp)
	sd a0, 8(sp)
	sd a1, 16(sp)
	sd t0, 24(sp)
	sd t1, 32(sp)
	sd t2, 40(sp)
	sd t3, 48(sp)
	
	la a0, format_string
	mv a1, t4
	mv a2, t4
	call printf

	ld ra, (sp)
	ld a0, 8(sp)
	ld a1, 16(sp)
	ld t0, 24(sp)
	ld t1, 32(sp)
	ld t2, 40(sp)
	ld t3, 48(sp)
	addi sp, sp, 64
	

done:
    ret


# Dummy test function that returns a predetermined value
test_function_1:
    # Set the return value (e.g., 42)
    #li a0, 42
	add a0, a0, a1
	mul a0, a0, a2    

    # Return to the caller
    ret


codegen_func_s:

