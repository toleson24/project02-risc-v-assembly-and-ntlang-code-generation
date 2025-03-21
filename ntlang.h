/* ntlang.h - header file for project01 (ntlang) */

#include <stdbool.h> 
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>


/*
 * scan.c
 */
 
/* 

# Scanner EBNF (microsyntax)

tokens		::= (token)*
token  		::= intlit | binlit | hexlit | symbol
symbol 		::= '+' | '-' | '*' | '/' | '>>' | '>-' | '<<' | '~' | '&' | '|' | '^'
regname		::= 'a0' | 'a1' | ... | 'a7'
intlit		::= digit (digit)*
binlit		::= '0' | '1' ('0' | '1')*
hexlit		::= 'a' - 'f' | digit ('a' - 'f' | digit)* 
digit 		::= '0' | '1' | ... | '9'

# Ignore

whitespace	::= (' ' | '\t') (' ' | '\t')*

*/

#define SCAN_TOKEN_LEN 32
#define SCAN_TABLE_LEN 1024
#define SCAN_INPUT_LEN 4096

enum scan_token_enum {
	TK_INTLIT, /* 1, 22, 403 */
	TK_BINLIT, /* 0b1010, 0b11110000 */
	TK_HEXLIT, /* 0x1A5, 0x0000000F */
	TK_REG,    /* a0, a1 */
	TK_PLUS,   /* + */
	TK_MINUS,  /* - */
	TK_MULT,   /* * */
	TK_DIV,    /* / */
	TK_LSR,    /* >> */
	TK_ASR,    /* >- */
	TK_LSL,    /* << */
	TK_NOT,    /* ~ */
	TK_AND,    /* & */
	TK_OR,     /* | */
	TK_XOR,    /* ^ */
	TK_LPAREN, /* ( */
	TK_RPAREN, /* ) */
	TK_EOT,     /* end of text */
	TK_ANY,    /* A wildcard token used for parsing */
	TK_NONE,
};

#define SCAN_TOKEN_STRINGS {\
	"TK_INTLIT",\
	"TK_BINLIT",\
	"TK_HEXLIT",\
	"TK_REG",\
	"TK_PLUS",\
	"TK_MINUS",\
	"TK_MULT",\
	"TK_DIV",\
	"TK_LSR",\
	"TK_ASR",\
	"TK_LSL",\
	"TK_NOT",\
	"TK_AND",\
	"TK_OR",\
	"TK_XOR",\
	"TK_LPAREN",\
	"TK_RPAREN",\
	"TK_EOT",\
	"TK_ANY",\
	"TK_NONE",\
};

struct scan_token_st {
    enum scan_token_enum id;
    char value[SCAN_TOKEN_LEN];
};

struct scan_table_st {
    struct scan_token_st table[SCAN_TABLE_LEN];
    int len;
    int cur;
};

void scan_token_print(struct scan_token_st *tk);
void scan_table_init(struct scan_table_st *st);
void scan_table_scan(struct scan_table_st *st, char *input);
void scan_table_print(struct scan_table_st *st);
struct scan_token_st * scan_table_get(struct scan_table_st *st, int i);
bool scan_table_accept(struct scan_table_st *st, enum scan_token_enum tk_expected);

/*
 * parse.c
 */

 /*
 A simple grammar for the ntcalc langauge

# Parser

program    ::= expression EOT
expression ::= operand (operator operand)*
operand    ::= intlit | binlit | hexlit
             | regname
             | '~' operand
             | '-' operand
             | '(' expression ')'

operator   ::= '+' | '-' | '*' | '/' | '>>' | '>-' | '<<' | '~' | '&' | '|' | '^'
*/

enum parse_expr_enum {EX_INTVAL, EX_REG, EX_OPER1, EX_OPER2};
enum parse_oper_enum {OP_PLUS, OP_MINUS, OP_MULT, OP_DIV, OP_LSR, OP_ASR, OP_LSL, OP_NOT, OP_AND, OP_OR, OP_XOR, OP_NONE};

struct parse_node_st {
    enum parse_expr_enum type;
    union {
        struct {uint32_t value;} intval;
        struct {char *value;} reg;
		struct {enum parse_oper_enum oper;
                struct parse_node_st *operand;} oper1;
        struct {enum parse_oper_enum oper;
                struct parse_node_st *left;
                struct parse_node_st *right;} oper2;
    };
};


#define PARSE_TABLE_LEN 1024

/* The parse_table_st is similar to the scan_table_st and is
 * used to allocatio parse_node_st structs so we can avoid
 * heap allocation.
 */
struct parse_table_st {
    struct parse_node_st table[PARSE_TABLE_LEN];
    int len;
};

void parse_table_init(struct parse_table_st *pt);
struct parse_node_st * parse_node_new(struct parse_table_st *pt);
struct parse_node_st * parse_program(struct parse_table_st *pt,
                                        struct scan_table_st *st);
void parse_tree_print(struct parse_node_st *np);

/*
 * config
 */

struct config_st {
    char input[SCAN_INPUT_LEN];
	int args[8];
	int base;
	int width;
	bool is_signed;
	bool compile_flag;
	char *fname;
};

/*
 * eval.c
 */

uint32_t eval(struct config_st * cp, struct parse_node_st *pt);
void eval_print(struct config_st *cp, uint32_t value);

//int itoa(uint32_t num, char *str, int base);

#define WIDTH_DEFAULT 32
#define EVAL_OUTPUT_LEN 64

/*
 * compile.c
 */

void compile_output_main(char *name);
void generate_code(struct parse_node_st *pt);


