/* parse.c - parsing and parse tree construction */

#include "ntlang.h"

struct parse_oper_pair_st {
    enum scan_token_enum tkid;
    enum parse_oper_enum opid;
};

struct parse_oper_pair_st parse_oper_map[] = {
    {TK_PLUS, OP_PLUS},
    {TK_MINUS, OP_MINUS},
    {TK_MULT, OP_MULT},
    {TK_DIV, OP_DIV},
    {TK_LSR, OP_LSR},
    {TK_ASR, OP_ASR},
    {TK_LSL, OP_LSL},
    {TK_NOT, OP_NOT},
    {TK_AND, OP_AND},
    {TK_OR, OP_OR},
    {TK_XOR, OP_XOR},
	{TK_NONE, OP_NONE}
};


void parse_table_init(struct parse_table_st *pt) {
    pt->len = 0;
}

struct parse_node_st * parse_node_new(struct parse_table_st *pt) {
    struct parse_node_st *np;

    np = &(pt->table[pt->len]);
    pt->len += 1;

    return np;
}

void parse_error(char *err) {
    printf("parse_error: %s\n", err);
    exit(-1);
}

char *parse_oper_strings[] = {"PLUS", "MINUS", "MULT", "DIV", "LSR", "ASR", "LSL", "NOT", "AND", "OR", "XOR", "NONE"};


/* We need to provide prototypes for the parsing functions because
 * we call them before they are defined.
 */
struct parse_node_st * parse_program(struct parse_table_st *pt, 
                                        struct scan_table_st *st);
struct parse_node_st * parse_expression(struct parse_table_st *pt, 
                                        struct scan_table_st *st);
struct parse_node_st * parse_expression_helper(struct parse_table_st *pt,
												struct scan_table_st *st,
												struct parse_node_st *np1,
												struct parse_node_st *np2,
												int pee,
												int poe);
struct parse_node_st * parse_operand(struct parse_table_st *pt, 
                                        struct scan_table_st *st);
int parse_oper_lookup(int tkid);

/* We need a parsing function for each rule in the EBNF grammer */

struct parse_node_st * parse_program(struct parse_table_st *pt, 
                                        struct scan_table_st *st) {
    struct parse_node_st *np1;

    /* A program is a single expression followed by EOT */
    np1 = parse_expression(pt, st);

    if (!scan_table_accept(st, TK_EOT)) {
        parse_error("Expecting EOT");
    }

    return np1;
}

/*
 * Function written for project01
 *
 * // TODO
*/
int parse_oper_lookup(int tkid) {
	int len = *(&parse_oper_map + 1) - parse_oper_map;
	int op = OP_NONE;	

	for (int i = 0; i < len; i ++) {
		if (tkid == parse_oper_map[i].tkid) {
			op = parse_oper_map[i].opid;
			break;
		}
	}

	return op;
}

struct parse_node_st * parse_expression(struct parse_table_st *pt,
										struct scan_table_st *st) {
	struct scan_token_st *tp;
	struct parse_node_st *np1, *np2;
	int opid;	

	np1 = parse_operand(pt, st);

	while (true) {
		tp = scan_table_get(st, 0);
		opid = parse_oper_lookup(tp->id);
		if (opid == OP_NONE)
			break;
		np1 = parse_expression_helper(pt, st, np1, np2, tp->id, opid);
	}

	return np1;
}

/*
 * Function written for lab02
 * 
 * // TODO
*/
struct parse_node_st * parse_expression_helper(struct parse_table_st *pt, 
        		                                struct scan_table_st *st,
                		                        struct parse_node_st *np1,
                        		                struct parse_node_st *np2,
                                		        int pee,
                                        		int poe) { 
		/* Use TK_ANY as a wildcard */
        scan_table_accept(st, pee); 
        np2 = parse_node_new(pt);
        np2->type = EX_OPER2;
        np2->oper2.oper = poe;
        np2->oper2.left = np1;
        /* Now parse second operand */
        np2->oper2.right = parse_operand(pt, st);
        np1 = np2;

        return np1;
}

/*
 * Function updated for project01
 * 
 * // TODO
*/
struct parse_node_st * parse_operand(struct parse_table_st *pt,
                                     struct scan_table_st *st) {
    struct scan_token_st *tp;
    struct parse_node_st *np1;

    if (scan_table_accept(st, TK_MINUS)) {
        tp = scan_table_get(st, -1);
        np1 = parse_node_new(pt);
        np1->type = EX_OPER1;
        np1->oper1.oper = OP_MINUS;
        np1->oper1.operand = parse_operand(pt, st);
    } else if (scan_table_accept(st, TK_NOT)) {
        tp = scan_table_get(st, -1);
        np1 = parse_node_new(pt);
        np1->type = EX_OPER1;
        np1->oper1.oper = OP_NOT;
        np1->oper1.operand = parse_operand(pt, st);
    } else if (scan_table_accept(st, TK_LPAREN)) {
        np1 = parse_expression(pt, st);
		if (!scan_table_accept(st, TK_RPAREN)) {
			parse_error("Missing right paren");
		}
	} else if (scan_table_accept(st, TK_REG)) {
		tp = scan_table_get(st, -1);
		np1 = parse_node_new(pt);
		np1->type = EX_REG;
		np1->reg.value = tp->value;  // TODO test
    } else if (scan_table_accept(st, TK_INTLIT)) {
        tp = scan_table_get(st, -1);
        np1 = parse_node_new(pt);
        np1->type = EX_INTVAL;
        np1->intval.value = atoi(tp->value);  // convert_from_base(tp->value, 10);
    } else if (scan_table_accept(st, TK_BINLIT)) {
        tp = scan_table_get(st, -1);
        np1 = parse_node_new(pt);
        np1->type = EX_INTVAL;
        np1->intval.value = atoi(tp->value);  // convert_from_base(tp->value, 2);
    } else if (scan_table_accept(st, TK_HEXLIT)) {
        tp = scan_table_get(st, -1);
        np1 = parse_node_new(pt);
        np1->type = EX_INTVAL;
        np1->intval.value = atoi(tp->value);  // convert_from_base(tp->value, 16);
    } else {
        parse_error("Bad operand");
    }

    return np1;
}

void parse_tree_print_indent(int level) {
    level *= 2;
    for (int i = 0; i < level; i++) {
        printf(".");
    }
}

void parse_tree_print_expr(struct parse_node_st *np, int level) {
    parse_tree_print_indent(level);
    printf("EXPR ");

    if (np->type == EX_INTVAL) {
        printf("INTVAL %d\n", np->intval.value);
	} else if (np->type == EX_REG) {
		// TODO test
		printf("REG %s\n", np->reg.value);
    } else if (np->type == EX_OPER1) {
        printf("OPER1 %s\n", parse_oper_strings[np->oper1.oper]);
        parse_tree_print_expr(np->oper1.operand, level+1);
    } else if (np->type == EX_OPER2) {
        printf("OPER2 %s\n", parse_oper_strings[np->oper2.oper]);
        parse_tree_print_expr(np->oper2.left, level+1);
        parse_tree_print_expr(np->oper2.right, level+1);
    }
}

void parse_tree_print(struct parse_node_st *np) {
    parse_tree_print_expr(np, 0);    
}
