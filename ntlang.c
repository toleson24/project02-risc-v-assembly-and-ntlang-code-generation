/* project01.c - initial parsing implemenation */

#include "ntlang.h"

/*
 * Helper function written for project01
 * 
 * Checks the string for flag notation. Example "-e" checks for '-' at the first position and 'e' at the second.
*/
bool check_flag(char *argv, char ch_expected) {
    return (argv[0] == '-' && argv[1] == ch_expected);
}

bool check_register_flag(char *argv, char ch_expected) {
	return (check_flag(argv, ch_expected) && (argv[2] >= '0' && argv[2] <= '8'));
}

/*
 A simple command line argument parser

# Command Line Parser
*/
void parse_args(struct config_st *cp, int argc, char **argv) {
    int i = 1;

    if (argc <= 2) {
        printf("Usage: project02 -e <expression>\n");
        printf("  Example: project02 -e \"1 + 2\"\n");
        exit(-1);
    }

    // defaults: width 32, base 10, & signed
    cp->width = WIDTH_DEFAULT;
    cp->base = 10;
    cp->is_signed = true;
	// TODO zero out cp->args ?	

    while (i < argc && i + 1 < argc) {
        if (check_flag(argv[i], 'e')) {
            strncpy(cp->input, argv[i + 1], SCAN_INPUT_LEN);
		} else if (check_register_flag(argv[i], 'a')) {
			char *flag = argv[i];
			char *ch_reg_num = &flag[2];
			int reg_num = atoi(ch_reg_num);
			cp->args[reg_num] = atoi(argv[i + 1]);
        } else if (check_flag(argv[i], 'b')) {
            int base = atoi(argv[i + 1]);  // convert_from_base(argv[i + 1], 10);
            if (base == 2 || base == 10 || base == 16) {
                cp->base = atoi(argv[i + 1]);  // convert_from_base(argv[i + 1], 10);
            }
        } else if (check_flag(argv[i], 'w')) {
            int width = atoi(argv[i + 1]);  // convert_from_base(argv[i + 1], 10);
            if (width == 4 || width == 8 || width == 16 || width == 32) {
                cp->width = atoi(argv[i + 1]);  // convert_from_base(argv[i + 1], 10);
            }
        } else if (check_flag(argv[i], 'u')) {
            cp->is_signed = false;
        } else if (check_flag(argv[i], 'c')) {
			cp->compile = true;
		}
        i += 1;
    }

    if (strnlen(cp->input, SCAN_INPUT_LEN) == 0) {
        printf("No expression given to evaluate\n");
		exit(-1);
    }
}

int main(int argc, char **argv) {
    struct config_st config;
    struct scan_table_st scan_table;
    struct parse_table_st parse_table;
    struct parse_node_st *parse_tree;
    uint32_t value;
    
	parse_args(&config, argc, argv);

    scan_table_init(&scan_table);
    scan_table_scan(&scan_table, config.input);
    //scan_table_print(&scan_table);
    //printf("\n");
    
    parse_table_init(&parse_table);
    parse_tree = parse_program(&parse_table, &scan_table);
    //parse_tree_print(parse_tree);
    //printf("\n");

    value = eval(&config, parse_tree);
    eval_print(&config, value);

    return 0;
}
