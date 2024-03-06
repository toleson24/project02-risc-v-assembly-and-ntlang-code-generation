/* scan.c - an initial project01 scanner */

#include "ntlang.h"

char *scan_token_strings[] = SCAN_TOKEN_STRINGS;

void scan_table_init(struct scan_table_st *st) {
    st->len = 0;
    st->cur = 0;
}

void scan_token_print(struct scan_token_st *tp) {
    printf("%s(\"%s\")\n", scan_token_strings[tp->id], tp->value);
}

void scan_table_print(struct scan_table_st *st) {
    int i;

    for (i = 0; i < st->len; i++) {
        scan_token_print(&st->table[i]);
    }
}

struct scan_token_st * scan_table_new_token(struct scan_table_st *st) {
    /* Allocate a new token from the token table */
    /* We just return a pointer to the next avaiable token in the array. */
    struct scan_token_st *tp;

    tp = &(st->table[st->len]);
    st->len += 1;

    return tp;
}

bool scan_is_whitespace(char ch) {
    return (ch == ' ') || (ch == '\t');
}

char *scan_whitespace(char *p, char *end) {
    while(scan_is_whitespace(*p) && (p < end)) {
        p += 1;
    }
    return p;
}

bool scan_is_digit(char ch) {
    return (ch >= '0' && ch <= '9');
}

char * scan_intlit(char *p, char *end, struct scan_token_st *tp) {
    int i = 0;

    while (scan_is_digit(*p) && (p < end)) {
        tp->value[i] = *p;
        p += 1;
        i += 1;
    }
    tp->value[i] = '\0';
    tp->id = TK_INTLIT;

    return p;
}

/* 
 * Function written for lab01
 * 
 * Checks the start of a token for '0b' as a prefix. 
 */
bool scan_is_binary_prefix(char ch1, char ch2) {
    return (ch1 == '0' && (ch2 == 'b' || ch2 == 'B'));
}

/* 
 * Function written for lab01
 * 
 * Checks token characters for either a 0 or 1. 
 */
bool scan_is_binary(char ch) {
    return (ch == '0' || ch == '1');
}

/* 
 * Function written for lab01
 * 
 * Scans the remaining characters creating binlit val.
 * Error: hexadecimal-like values.
 */
char * scan_binlit(char *p, char *end, struct scan_token_st *tp) {
    int i = 0;
    p += 2;

    if (!scan_is_binary(*p)) {
        printf("error: scan_binlit - expecting 0 or 1.\n");
        exit(-1);
    }

    while (scan_is_binary(*p) && (p < end)) {
        tp->value[i] = *p;
        p += 1;
        i += 1;
    }
    tp->value[i] = '\0';
    tp->id = TK_BINLIT;

    return p;
}

/*
 * Function written for project01
 *
 * Checks the start of a token for '0x' as a prefix.
*/
bool scan_is_hex_prefix(char ch1, char ch2) {
    return (ch1 == '0' && (ch2 == 'x' || ch2 == 'X'));
}

/* 
 * Function written for project01
 * 
 * Checks token characters for range 'a-f', 'A-F', and digits. 
 */
bool scan_is_hexadecimal(char ch) {
    return (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F') || (ch >= '0' && ch <= '9');
}

/* 
 * Function written for project01
 * 
 * Converts characters in the range 'a-f' into 'A-F'. 
 */
char to_upper(char ch) {
    if (ch >= 'A' && ch <= 'F') {
        return ch;
    } else if (ch >= '0' && ch <= '9') {
        return ch;
    } else {
        return (ch - 'a') + 'A';
    }
}

/*
 * Function written for project01
 * 
 * Scans the remaining characters creating a hexlit val.
 * Error: non-hexadecimal values.
*/
char * scan_hexlit(char *p, char *end, struct scan_token_st *tp) {
    int i = 0;
    p += 2;

    if (!scan_is_hexadecimal(*p)) {
        printf("error: scan_hexlit - expecting a-f, A-F, or 0-9.");
        exit(-1);
    }

    while (scan_is_hexadecimal(*p) && (p < end)) {
        tp->value[i] = to_upper(*p);
        p += 1;
        i += 1;
    }
    tp->value[i] = '\0';
    tp->id = TK_HEXLIT;

    return p;
}

/**/
bool scan_is_register(char ch1, char ch2) {
	return (ch1 == 'a' && (ch2 >= '0' && ch2 <= '9'));
}

/**/
char * scan_register(char *p, char *end, struct scan_token_st *tp) {
	int i = 0
	for (i = 0; i < 2; i++, p++) {
		tp->value[i] = *p;
	}	
	tp->value[i] = '\0';
	tp->id = TK_REGISTER;

	return p;
}

/*
 * Read a token starting a p for len characters.
 * Update the given token with the token string and token id.
*/
char * scan_token_helper(struct scan_token_st *tp, char *p, int len,
                       enum scan_token_enum id) {
    int i;

    tp->id = id;
    for (i = 0; i < len; i++) {
        tp->value[i] = *p;
        p += 1;
    }
    tp->value[i] = '\0';
    return p;
}

char * scan_token(char *p, char *end, struct scan_token_st *tp) {
    /* Scan the next token at the current location in the input string */
    /* p points to the current location in the input string */
    /* end is a pointer to the end of the string */
    /* The return value is the updated location in the input string. */
    /* The token pointed to by tp is updated with the scanned token. */

    if (p == end) {
        /* Check if we are at the end of the input string */
        tp->value[0] = '\0';
        tp->id = TK_EOT;
    } else if (scan_is_whitespace(*p)) {
        /* Ingore whitespace. Notice the recursive call. */
        p = scan_whitespace(p, end);
        p = scan_token(p, end, tp);
	} else if (scan_is_register(*p, *(p + 1))) {
		p = scan_register(p, end, TK_REGISTER);
    } else if (scan_is_binary_prefix(*p, *(p + 1))) {
        p = scan_binlit(p, end, tp);
    } else if (scan_is_hex_prefix(*p, *(p + 1))) {
        p = scan_hexlit(p, end, tp);
    } else if (scan_is_digit(*p)) {
        p = scan_intlit(p, end, tp);        
    } else if (*p == '+') {
        p = scan_token_helper(tp, p, 1, TK_PLUS);
    } else if (*p == '-') {
        p = scan_token_helper(tp, p, 1, TK_MINUS);
    } else if (*p == '*') {
        p = scan_token_helper(tp, p, 1, TK_MULT);
    } else if (*p == '/') {
        p = scan_token_helper(tp, p, 1, TK_DIV);
    } else if (*p == '>' && *(p + 1) == '>') {
        p = scan_token_helper(tp, p, 2, TK_LSR);
    } else if (*p == '>' && *(p + 1) == '-') {
        p = scan_token_helper(tp, p, 2, TK_ASR);
    } else if (*p == '<' && *(p + 1) == '<') {
        p = scan_token_helper(tp, p, 2, TK_LSL);
    } else if (*p == '~') {
        p = scan_token_helper(tp, p, 1, TK_NOT);
    } else if (*p == '&') {
        p = scan_token_helper(tp, p, 1, TK_AND);
    } else if (*p == '|') {
        p = scan_token_helper(tp, p, 1, TK_OR);
    } else if (*p == '^') {
        p = scan_token_helper(tp, p, 1, TK_XOR);
    } else if (*p == '(') {
        p = scan_token_helper(tp, p, 1, TK_LPAREN);
    } else if (*p == ')') {
        p = scan_token_helper(tp, p, 1, TK_RPAREN);
    } else {
        /* Instead of returning an error code, we will usually
           exit on failure. */
        printf("scan error: invalid char: %c\n", *p);
        exit(-1);
    }
    return p;
}


/* Scan valid tokens in the given input string. */
void scan_table_scan(struct scan_table_st *st, char *input) {
    struct scan_token_st *tp;
    char *p = input;
    char *end;
    int len;

    /* Get a pointer to the end of the input string. */
    /* Technically this pointer points to the null termination character. */
    len = strnlen(input, SCAN_INPUT_LEN);
    end = p + len;

    do {
        /* Allocate a token */
        tp = scan_table_new_token(st);
        /* Scan one token from input string */
        p = scan_token(p, end, tp);
        /* Are we done? */
        if (tp->id == TK_EOT) {
            break;
       }
    } while(true);
}

/* Get the token at the current (cur) position + i in the token table. */
struct scan_token_st * scan_table_get(struct scan_table_st *st, int i) {
    return &(st->table[st->cur + i]);
}

/* Accept the current token if it matches tk_expected.
 * If it matches, then increment the cur position by 1 and return true.
 * Otherwise, return false */
bool scan_table_accept(struct scan_table_st *st, enum scan_token_enum tk_expected) {
    struct scan_token_st *tp;

    if (tk_expected == TK_ANY) {
        st->cur += 1;
        return true;
    }

    tp = scan_table_get(st, 0);

    if (tp->id == tk_expected) {
        st->cur += 1;
        return true;
    }

    return false;
}
