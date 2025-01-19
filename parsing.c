#include <stdio.h>
#include <stdlib.h>
#include "mpc.h"

#include <editline/readline.h> // read input while allowing us to edit edit the input 
#include <histedit.h> 

// Declare buffer size for user input 
static char input_buffer[2048]; 

int main(int argc, char** argv) {

    mpc_parser_t* Number = mpc_new("number"); 
    mpc_parser_t* Operator = mpc_new("operator");
    mpc_parser_t* Expr     = mpc_new("expr");
    mpc_parser_t* Lispy    = mpc_new("lispy");

    /* Define them with the following Language */
    mpca_lang(MPCA_LANG_DEFAULT,
    "                                                     \
      number   : /-?[0-9]+/ ;                             \
      operator : '+' | '-' | '*' | '/' ;                  \
      expr     : <number> | '(' <operator> <expr>+ ')' ;  \
      lispy    : /^/ <operator> <expr>+ /$/ ;             \
    ",
    Number, Operator, Expr, Lispy);

    puts("Lispy Version Beta"); 
    puts("Press Control + C to Exit\n"); 

    while (1) {
  
    char* input = readline("lispy> ");
    add_history(input);
    
    /* Attempt to parse the user input */
    mpc_result_t r;
    if (mpc_parse("<stdin>", input, Lispy, &r)) {
        /* On success print and delete the AST */
        mpc_ast_print(r.output);
        mpc_ast_delete(r.output);
    } else {
        /* Otherwise print and delete the Error */
        mpc_err_print(r.error);
        mpc_err_delete(r.error);
    }
    
    free(input);
    }
  
    /* Undefine and delete our parsers */
    mpc_cleanup(4, Number, Operator, Expr, Lispy);

    return 0; 
} 