.global pack_bytes_s

# Pack 4 bytes into one 32 bit word (uint32_t).
# Assume a3 is least significant byte.
#
# a0-a3 - single 4 byte words
# t0 - uint32_t val

pack_bytes_s:
    mv t0, a0
	sllw t0, t0, 8
	or t0, t0, a1
	sllw t0, t0, 8
	or t0, t0, a2
	sllw t0, t0, 8
	or t0, t0, a3
	mv a0, t0
	ret

