#! risc-v

.global codegen_func_s

# Parameters
cogegen_func_s:
    # TODO
    # init array
    li t1, 0    # i = 0
    li t2, 8    # len = 8
    li rv, 0    # return value

init_loop:
    bge t1, t2, reset_t1
    # TODO
       
reset_t1:
    li t1, 0

populate_loop:
    bge t1, t2, done
    # TODO

done:
    ret
