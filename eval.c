/* eval.c - tree evaluation and value printing */

#include "ntlang.h"

void eval_error(char *err) {
    printf("eval_error: %s\n", err);
    exit(-1);
}

/**/
uint32_t get_mask(int width) {
    return (0b1 << width) - 1;
}

/**/
void eval_ex_oper1(uint32_t *v1, int oper) {
	if (oper == OP_MINUS) {
		*v1 = -*v1;
	} else if (oper == OP_NOT) {
		*v1 = ~*v1;
	} else {
		eval_error("Bad operator");
	}
}

/**/
void eval_ex_oper2(uint32_t *v1, uint32_t v2, int oper) {
	// TODO convert to data-driven approach (like parse_expression() with a eval_oper_lookup() helper function)
    if (oper == OP_PLUS) {
        *v1 = *v1 + v2;
    } else if (oper == OP_MINUS) {
        *v1 = *v1 - v2;
    } else if (oper == OP_MULT) {
        *v1 = *v1 * v2;
    } else if (oper == OP_DIV) {
        *v1 = *v1 / v2;
    } else if (oper == OP_LSR) {
        *v1 = *v1 >> v2;
    } else if (oper == OP_ASR) {
	    *v1 = (int32_t) (*v1 >> v2);
    } else if (oper == OP_LSL) {
        *v1 = *v1 << v2;
    } else if (oper == OP_AND) {
        *v1 = *v1 & v2;
    } else if (oper == OP_OR) {
        *v1 = *v1 | v2;
    } else if (oper == OP_XOR) {
        *v1 = *v1 ^ v2;
    } else {
        eval_error("Bad operator");            
    }
}

uint32_t eval(struct config_st *cp, struct parse_node_st *pt) {
    uint32_t v1, v2;

    if (pt->type == EX_INTVAL) {
        v1 = pt->intval.value;
	} else if (pt->type == EX_REG) {
		char *reg = pt->reg.value;
		char *ch_reg_num = &reg[1];
		int reg_num = atoi(ch_reg_num);
		v1 = cp->args[reg_num];
    } else if (pt->type == EX_OPER1) {
        v1 = eval(cp, pt->oper1.operand);
        int oper = pt->oper1.oper;
        eval_ex_oper1(&v1, oper);
    } else if (pt->type == EX_OPER2) {
        v1 = eval(cp, pt->oper2.left);
        v2 = eval(cp, pt->oper2.right);
        int oper = pt->oper2.oper;
        eval_ex_oper2(&v1, v2, oper);
    } 

    return v1;
}

/**/
void reverse_string(char str_value[], char output[], int len, int width, int base) {
	int i = 0, j = 0;
	
	if (base == 2) {
		output[i++] = '0';
		output[i++] = 'b';
		//width /= 4;
	} else if (base == 16) {
		output[i++] = '0';
		output[i++] = 'x';
		width /= 4;
	}

	if (base != 10 && len < width) {
		for (j = 0; j < width - len; j++) {
			output[i++] = '0';
		}
	} 

	for (j = 0; j < width - 1; j++) {
		output[i++] = str_value[j];
	}
	output[i++] = '\0';
}

/**/
// TODO fix
char * to_base_string(uint32_t value, int width, int base, bool is_signed) {
	static char str_value[EVAL_OUTPUT_LEN], output[EVAL_OUTPUT_LEN];
	int i = 0;
	bool is_neg = false;
	
	if (base == 10 && value < 0) {
		is_neg = true;
		value = -value;
	}

	do {
		int remainder = value % base;
		if (remainder > 9) {
			str_value[i++] = (remainder - 10) + 'a';
		} else {
			str_value[i++] = remainder + '0';
		}
		value /= base;
	} while (value > 0 && i < width);

	if (is_neg) {
		str_value[i++] = '-';
	}

	str_value[i++] = '\0';

	reverse_string(str_value, output, --i, width, base);
	
	return output;

}

/**/
void eval_print(struct config_st *cp, uint32_t value) {
    int base = cp->base;
    int width = cp->width;
	bool is_signed = cp->is_signed;
    // mask every number, unless w=32
    if (width != 32) {
        value = value & get_mask(width);
        // take two's complement if signed
        if ((value >> (width - 1)) & 1) {
            value = (~value) + 1;
        }
    }
	
	char output[SCAN_INPUT_LEN];    
    switch(base) {
        case 2:  // BASE_BINARY:
			//itoa(value, output, 2);
			sprintf(output,"%d", value);
			printf("%s\n", output);
			//printf("%s\n", to_base_string(value, width, base, is_signed));
            break;
        case 10:  // BASE_DECIMAL:
			//itoa(value, output, 10);
			sprintf(output,"%d", value);
			printf("%s\n", output);
            //printf("%s\n", to_base_string(value, width, base, is_signed));
            break;
        case 16:  // BASE_HEXADECIMAL:
			//itoa(value, output, 16);
			sprintf(output,"%d", value);
			printf("%s\n", output);
            //printf("%s\n", to_base_string(value, width, base, is_signed));
            break;
    }
}

