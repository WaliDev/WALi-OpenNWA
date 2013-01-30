%{

// compile with: yacc -vd -p bp

#include <stdlib.h>
#include <stdio.h>
#include "ast.h"

	#define YYERROR_VERBOSE

	extern int bplex(void);
	extern int bperror(const char *error);
	extern int bplineno;
	extern char *bptext;
	extern int bpleng;
	extern FILE *bpin;

unsigned start_thread_stmt_ctr, end_thread_stmt_ctr, sync_stmt_ctr, atomic_begin_stmt_ctr, atomic_end_stmt_ctr, dead_stmt_ctr, print_stmt_ctr;
unsigned schoose_stmt_ctr;

prog * parsing_result = NULL;

void yyerror(char * s);
%}

%locations

%token <v> IDENTIFIER "identifier"
%token <v> TICKED_IDENTIFIER "escaped identifier"
%token <number> INTEGER "integer"
%token NONDET "*"
%token ASSIGN "assignment"
%token EQ_OP "="
%token NE_OP "<>"
%token IMPLIES "implication"
%token DECL "decl"
%token ENFORCE "enforce"
%token AND "&"
%token OR "|"
%token TERNARY "?"
%token IF "if"
%token THEN "then"
%token ELSE "else"
%token ELSIF "elsif"
%token FI "fi"
%token WHILE "while"
%token DO "do"
%token OD "od"
%token RETURN "return"
%token SKIP "skip"
%token GOTO "goto"
%token BEG "begin"
%token END "end"
%token BOOL "bool"
%token VOID "void"
%token ASSERT "assert"
%token ASSUME "assume"
%token PRINT "print"
%token CHOOSE "schoose"
%token CONSTRAIN "constrain"
%token DEAD "dead"
%token ABORTIF "abortif"
%token DFS "dfs"
%token START_THREAD "start_thread"
%token END_THREAD "end_thread"
%token SYNC "sync"
%token ATOMIC_BEGIN "atomic_begin"
%token ATOMIC_END "atomic_end"

%type <number> fun_type
%type <vl> identifier_list g_identifier_list parameter_list assign_id_list decl_list g_decl_list parms
%type <v> decl_id g_decl_id
%type <v> asgn_id
%type <el> expression_list assign_expression_list
%type <sl> statement_list elsif_list
%type <p> function_definition
%type <pl> fun_list
%type <s> labelled_statement statement concurrent_statement
%type <s> parallel_assign call assert assume print selection_statement iteration_statement jump_statement dead_statement
%type <s> start_thread_statement end_thread_statement sync_statement atomic_begin_statement atomic_end_statement
%type <e> primary_expression unary_expression equality_expression
%type <e> and_expression exclusive_or_expression expression
%type <e> or_expression enforce abortif choose_expression assign_constrain_opt
%type <vl> label_list target_label_list
%type <pg> program

%start program

%%

program:
	  pstart g_decl_list fun_list pend 
		{
			parsing_result = make_prog($2,$3);
		}
	   ;

pstart:
	  {
			start_thread_stmt_ctr = 0;
			end_thread_stmt_ctr = 0;
			sync_stmt_ctr = 0;
			atomic_begin_stmt_ctr = 0;
  		atomic_end_stmt_ctr = 0;
			dead_stmt_ctr = 0;
			print_stmt_ctr = 0;
      schoose_stmt_ctr = 0;
	  } ;

pend:
	  {
			if(start_thread_stmt_ctr > 0)
				fprintf(stderr, "Converted %d 'start_thread' statements to skip\n", start_thread_stmt_ctr);
			if(end_thread_stmt_ctr > 0)
				fprintf(stderr, "Converted %d 'end_thread' statements to skip\n", end_thread_stmt_ctr);
			if(sync_stmt_ctr > 0)
				fprintf(stderr, "Converted %d 'sync' statements to skip\n", sync_stmt_ctr);
			if(atomic_begin_stmt_ctr > 0)
				fprintf(stderr, "Converted %d 'atomic_begin' statements to skip\n", atomic_begin_stmt_ctr);
			if(atomic_end_stmt_ctr > 0)
				fprintf(stderr, "Converted %d 'atomic_end' statements to skip\n", atomic_end_stmt_ctr);
			if(dead_stmt_ctr > 0)
			  fprintf(stderr, "Converted %d 'dead' statements to skip\n", dead_stmt_ctr);
			if(print_stmt_ctr > 0)
				fprintf(stderr, "Covnerted %d 'print' statements to skip\n", print_stmt_ctr);
      if(schoose_stmt_ctr > 0)
        fprintf(stderr, "***WARNING*** converted %d 'schoose' statements to *\n", schoose_stmt_ctr);

	  }
	;

/***** decls *****/

g_decl_list
	: DECL g_identifier_list ';' g_decl_list
	  {
			$$ = splice_str($2,$4);
		}
	| {$$ = NULL;}
  ;
	
g_identifier_list
	: g_decl_id
	  {
	  	$$ = make_str_list_item($1);
	  }
	| g_identifier_list ',' g_decl_id
	  {
	  	$$ = add_str_right($1,$3);
	  } ;

g_decl_id
	: IDENTIFIER			
	  {
	  	$$ = $1;
	  } ;
	
decl_list
	: DECL identifier_list ';' decl_list
	  {
			$$ = splice_str($2,$4);
	  }
	| {$$ = NULL;}
  ;

identifier_list
	: decl_id
	  {
	    $$ = make_str_list_item($1);
	  }
	| identifier_list ',' decl_id
	  {
	  	$$ = add_str_right($1,$3);
	  } ;

decl_id	: IDENTIFIER			
	  {
	  	$$ = $1;
	  } ;
	  
/***** functions *****/

fun_list
	: function_definition { $$ = make_proc_list_item($1);}
	| function_definition fun_list {$$ = add_proc_left($2,$1);}
	; 

function_definition
	: fun_type IDENTIFIER parms BEG decl_list enforce abortif statement_list END
	  {
	    $$ = make_proc($1,$2,$3,$5,$6,$8);
	  }
  | fun_type IDENTIFIER parms BEG decl_list enforce abortif END
    {
      // there are no statements, so add a skip statement.
      stmt * s = make_skip_stmt();
      stmt_list * sl = make_stmt_list_item(s);
      $$ = make_proc($1, $2, $3, $5, $6, sl);
    }
   
	 ;

fun_type
    : fun_search_order BOOL	{ $$ = (unsigned) 1;}
	| fun_search_order BOOL '<' INTEGER '>' { $$ = (unsigned) $4;}
    | fun_search_order VOID	{ $$ = (unsigned) 0; }
    ;

fun_search_order
	: DFS
	|
	;

enforce
	: ENFORCE expression ';'
	  {
	  	$$ = $2;
	  }
	|
	  {
	  	$$ = NULL;
	  } ;

abortif
	: ABORTIF expression ';'
	  {
	  	deep_erase_expr(&$2);
	  }
	|
	  {
	  	// ...
	  } ;

parms	: '(' parameter_list ')'
	  {
	  	$$ = $2;
	  }
	  ;

parameter_list
	: identifier_list
	  {
	  	$$ = $1;
	  } 
	|	 { $$ = NULL; }
	;

/***** statements *****/

statement_list
    : labelled_statement
      {
        $$ = make_stmt_list_item($1);
	  }
	| statement_list labelled_statement
	  {
	  	$$ = add_stmt_right($1,$2);
	  }
    | statement_list ';'
      {
	  	$$ = $1;
      }
    ;

labelled_statement
    : label_list concurrent_statement 
	  {
	  	$$ = add_labels($2, $1);
	  }
    | concurrent_statement
	  {
	  	$$ = $1;
	  }
    ;

label_list
		: label_list IDENTIFIER ':'
		  {
	  		$$ = add_str_right($1,$2);
		  }
		| IDENTIFIER ':'
		  {
	  		$$ = make_str_list_item($1);
		  }
		;

target_label_list
		: target_label_list ',' IDENTIFIER
		  {
		  	$$ = add_str_right($1,$3);
		  }
		| IDENTIFIER
		  {
		    $$ = make_str_list_item($1);
		  }
		;

concurrent_statement
    : statement {$$ = $1;}
	| start_thread_statement ';' 
	{
		start_thread_stmt_ctr++;
		$$ = make_skip_stmt();
	}
	| end_thread_statement ';' 
	{
		end_thread_stmt_ctr++;
		$$ = make_skip_stmt();
	}
	| sync_statement ';'
	{
		sync_stmt_ctr++;
		$$ = make_skip_stmt();
	}
	| atomic_begin_statement ';' 
	{
		atomic_begin_stmt_ctr++;
		$$ = make_skip_stmt();
	}
	| atomic_end_statement ';'
	{
		atomic_end_stmt_ctr++;
		$$ = make_skip_stmt();
	}
    ;
        
statement
    : dead_statement ';' 
		{
			dead_stmt_ctr++;
			$$ = make_skip_stmt();
		}
    | parallel_assign ';' {$$ = $1;}
    | assert ';'{$$ = $1;}
    | assume ';' {$$ = $1;}
    | print ';'
		{
			print_stmt_ctr++;
			$$ = make_skip_stmt();
		}
    | call ';' {$$ = $1;}
    | selection_statement {$$ = $1;}
    | iteration_statement {$$ = $1;}
    | jump_statement ';' {$$ = $1;}
    ;

dead_statement
	: DEAD assign_id_list 
	{
    str_list const * it = $2;
    if(it != NULL){
      expr_list * el = NULL;
      while(it!=NULL){
         expr * e =  make_non_det_expr();
         if(!el)
           el = make_expr_list_item(e);
         else
           add_expr_right(el, e);
         it = it->n;
      }
      $$ = make_assign_stmt($2, el, NULL);
    }else{
      $$ = make_skip_stmt();
    }
	}
	;
	   
parallel_assign 
       	: assign_id_list ASSIGN assign_expression_list assign_constrain_opt
	  {
      $$ = make_assign_stmt($1,$3,$4);
	  }
       	| assign_id_list ASSIGN call
	  { 
	    $3->vl = $1;
      $$ = $3;
	  } ;

assign_constrain_opt
	: {
	$$ = NULL; 
	}
	| CONSTRAIN expression
	  {
	  $$ = $2; 
	  }
	;

assign_expression_list
	: choose_expression
	  {
	  	$$ = make_expr_list_item($1);
	  }
	| assign_expression_list ',' choose_expression
	  {
	  	$$ = add_expr_right($1,$3);
	  } ;
  
assign_id_list
	: asgn_id
	  {
	  	$$ = make_str_list_item($1);
	  }
	| assign_id_list ',' asgn_id
	  {
	  	$$ = add_str_right($1,$3);
	  } ;

asgn_id	
	: IDENTIFIER			
	  {
	  	$$ = $1;
	  } ;

assert	
	: ASSERT expression
	  {
	  	$$ = make_assert_stmt($2);
	  };
	  
assume	
	: ASSUME expression
	  {
	    $$ = make_assume_stmt($2);
	  } ;

print	
	: PRINT '(' expression_list ')' 
	{
		deep_erase_expr_list(&$3);
	} ;

call
	: IDENTIFIER '(' expression_list ')'
	  {
	  	$$ = make_call_stmt($1, NULL, $3);
	  }
	| IDENTIFIER '(' ')'
	  {
	  	$$ = make_call_stmt($1,NULL,NULL);
	  } ;

selection_statement
	: IF choose_expression THEN statement_list elsif_list
	  {
	  	$$ = make_ite_stmt($2,$4,$5);
	  } ;

elsif_list
	: FI
	  {	
			$$ = NULL;	
	  }
	| ELSE statement_list FI
	  {	
	  	$$ = $2;
	  }
	| ELSIF choose_expression THEN statement_list elsif_list
	  {	
	  	$$ = make_stmt_list_item(make_ite_stmt($2,$4,$5));
	  } ;

iteration_statement
	: WHILE expression DO statement_list OD
	  {
	  	$$ = make_while_stmt($2,$4);
	  } ;

jump_statement
	: RETURN
	  {
	  	$$ = make_return_stmt(NULL);
	  }
	| RETURN expression_list
	  {
	  	$$ = make_return_stmt($2);
	  }
	| SKIP
	  {
	  	$$ = make_skip_stmt();
	  }
	| GOTO target_label_list
	  {
	  	$$ = make_goto_stmt($2);
	  } ;

start_thread_statement
    : START_THREAD GOTO IDENTIFIER 
		{
			free($3);
			$3 = NULL;
		}
    | START_THREAD IDENTIFIER '(' ')' 
		{
			free($2);
			$2 = NULL;
		}
    ;

end_thread_statement
    : END_THREAD {}
    ;

sync_statement
    : SYNC IDENTIFIER 
		{
			free($2);
			$2 = NULL;
		}
    ;

atomic_begin_statement
    : ATOMIC_BEGIN {}
    ;

atomic_end_statement
    : ATOMIC_END {}
    ;


/***** expressions *****/

primary_expression
	: '(' expression ')'
	  {
	  	$$ = $2;
  	  }
    | NONDET
      {
	  	$$ = make_non_det_expr();
      }
    | CHOOSE '[' expression ',' expression ']'
	  {
	  	$$ = make_non_det_expr();
      deep_erase_expr(&$3);
      deep_erase_expr(&$5);
      //make_schoose_expr($3,$5);
	  }
	| IDENTIFIER 	
	  { 
	  	$$ = make_var_expr($1);
	  }
	| '\'' IDENTIFIER
	  {
      $$ = mark_post_expr(make_var_expr($2));
	  }
	| INTEGER 
	  { 
	  	$$ = make_constant_expr(($1 == 0) ? ZERO : ONE);
	  }
	;
	
unary_expression
	: primary_expression { $$ = $1; }
	| unary_operator primary_expression 
	  { 
	  	$$ = make_not_expr($2);
	  }
	;

unary_operator
	: '~'
	| '!' ;

equality_expression
	: unary_expression { $$ = $1;}
	| equality_expression EQ_OP unary_expression
	  { 
	  	$$ = make_eq_expr($1,$3);
	  }
	| equality_expression NE_OP unary_expression
	  { 
	  	$$ = make_neq_expr($1,$3);
	  }
	;

and_expression
	: equality_expression { $$ = $1;}
	| and_expression AND equality_expression
	  { 
	  	$$ = make_and_expr($1, $3);
	  }
	;

exclusive_or_expression
	: and_expression {$$ = $1;}
	| exclusive_or_expression '^' and_expression
	  { 
	  	$$ = make_xor_expr($1,$3);
	  }
	;

or_expression
	: exclusive_or_expression {$$ = $1;}
	| or_expression OR exclusive_or_expression
	  { 
	  	$$ = make_or_expr($1,$3);
	  }
	| or_expression IMPLIES exclusive_or_expression
	  { 
	  	$$ = make_imp_expr($1,$3);
	  }
	;
	
expression
	: or_expression {$$ = $1;}
	| expression TERNARY expression ':' or_expression 
	  { 
	  	assert(0);
      $$ = NULL;
	  }
	;

choose_expression
	: expression {$$ = $1;}
	;

expression_list
	: choose_expression
	  { 
	  	$$ = make_expr_list_item($1);
	  }
	| expression_list ',' choose_expression
	  { 
	  	$$ = add_expr_right($1,$3);
	  }
	;

%%

void yyerror(char * s)
{
	fprintf(stderr, "(%d,%d)-(%d,%d): %s\n", yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, s);
	exit(1);
}

int parse(FILE * fin)
{
  yyset_in(fin);
  return yyparse();
}
