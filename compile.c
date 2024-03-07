/* compile.c - stack machine code generator */

#include "ntlang.h"

struct compile_oper_pair_st {
	enum parse_oper_enum opid;
	char *as_code;
};

struct compile_oper_pair_st compile_oper_map[] = {
	{OP_PLUS, "addi t0, t0, t1\n"},
	{OP_MINUS, "sub t0, t0, t1\n"},
	{OP_MULT, "mul t0, t0, t1\n"},
	{OP_DIV, "div t0, t0, t1\n"},
	{OP_LSR, "srlw t0, t0, t1\n"},
	{OP_ASR, "sraw t0, t0, t1\n"},
	{OP_LSL, "sllw t0, t0, t1\n"},
	{OP_NOT, "li t1, -1\nxor t0, t0, t1\n"},	// TODO remove ?
	{OP_AND, "and t0, t0, t1\n"},
	{OP_OR, "or t0, t0, t1\n"},
	{OP_XOR, "xor t0, t0, t1\n"},
	{OP_NONE, ""},								// TODO remove ?
};

void compile_output_main(char *name);
void generate_code(struct parse_node_st *pt);
void generate_unary_oper_code(int oper);
void generate_binary_oper_code(int oper);
char * compile_oper_lookup(int opid);

void compile_output_main(char *name) {
    int fd;
    char c;
    int rv;

    fd = open(name, O_RDONLY);
    while (true) {
        rv = read(fd, &c, 1);
        if (rv <= 0) {
            break;
        }
        printf("%c", c);
    }
}

void generate_code(struct parse_node_st *pt) {
	int sp_space = 4;
	switch(pt->type) {
		case EX_INTVAL:
			printf("addi sp, sp, %d\n", -sp_space);
			printf("li t0, %d\n", pt->intval.value);
			printf("sw t0, (sp)\n");
			break;
		case EX_REG:
			printf("addi sp, sp, %d\n", -sp_space);
			printf("sw %s, (sp)\n", pt->reg.value);
			break;
		case EX_OPER1:
			generate_code(pt->oper1.operand);
			printf("lw t0, (sp)\n");
			generate_unary_oper_code(pt->oper1.oper);
			printf("sw t0, (sp)\n");
			break;
		case EX_OPER2:
			generate_code(pt->oper2.left);
			generate_code(pt->oper2.right);
			printf("lw t1, (sp)\n");
			printf("addi, sp, sp, %d\n", sp_space);
			printf("lw t0, (sp)\n");
			generate_binary_oper_code(pt->oper2.oper);
			printf("sw t0, (sp)\n");
			break;
	}
}

void generate_unary_oper_code(int oper) {
	if (oper == OP_MINUS) {
		printf("neg t0, t0\n");
	} else if (oper == OP_NOT) {
		printf("li t1, -1\n");
		printf("xor t0, t0, t1\n");
	} else {
		// TODO compile_error("Bad operator");
	}
}

void generate_binary_oper_code(int oper) {
	printf("%s\n", compile_oper_lookup(oper));
}

char * compile_oper_lookup(int opid) {
	int len = *(&compile_oper_map + 1) - compile_oper_map;
	char *as = "\n";

	for (int i = 0; i < len; i++) {
		if (opid == compile_oper_map[i].opid) {
			as = compile_oper_map[i].as_code;
			break;
		}
	}
	
	return as;
}
