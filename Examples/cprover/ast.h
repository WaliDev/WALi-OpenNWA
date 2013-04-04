#ifndef __CPROVER_AST_H_
#define __CPROVER_AST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
/*
 * Important common properties of the ast data-structures (Marker for easy search (%*%*%))
 *
 * There are many functions provided in this file to manipulate the AST data-structures.
 * Some things to remember
 * (1) All unused fields *must* be NULL. This property is used extensively in the code.
 * (2) All lists store a tail pointer to the end of list. Maintain this
 * (3) All data structures take ownership of char * pointed to.
 * invariant if you manipulate the lists yourself
 *
 * Functions that should be used when possible
 * (1) There are constructor functions called make_* 
 *     These functions take ownership of the passed in char * values.
 * (2) For the lists there are functions to 
 *      - add element to the right add_*_right
 *      - add element to the left add_*_left (These return the new head of list, which might change)
 *      - splice two lists splice_* (and return the new head)
 * (3) To erase any part of the AST completely, use deep_erase_* functions
 * (4) emit_* can be used to obtain a human readable representation of the ast objects.
 **/
typedef enum {ZERO = 0, ONE = 1} constant;

typedef enum {AST_NOT, AST_XOR, AST_OR, AST_AND, AST_EQ, AST_NEQ, AST_IMP, AST_NONDET, AST_SCHOOSE, AST_VAR, AST_VAR_POST, AST_CONSTANT} binop;
/**
 * op is the operand
 * l,r are children
 * c is 0 or 1
 * v is the name of the variable
 *
 * unused fields *must* be NULL.
 * read above (search for %*%*%) before you manipulate these structures.
 **/
typedef struct expr_struct
{
  binop op;
  struct expr_struct * l;
  struct expr_struct * r;
  constant c;
  char * v;
} expr;

typedef enum {AST_SKIP, AST_GOTO, AST_RETURN, AST_ASSIGN, AST_ITE, AST_WHILE, AST_ASSERT, AST_ASSUME, AST_CALL} stmt_type;
/**
 * v is the string in the list
 * n is the next item
 * t is the last item
 *
 * unused fields *must* be NULL.
 * read above (search for %*%*%) before you manipulate these structures.
 **/
typedef struct str_list_struct 
{
  char * v;
  struct str_list_struct * n;
  struct str_list_struct * t;
} str_list;
/**
 * e is the expression in the list
 * n is the next item
 * t is the last item
 *
 * unused fields *must* be NULL.
 * read above (search for %*%*%) before you manipulate these structures.
 **/
typedef struct expr_list_struct
{
  expr * e;
  struct expr_list_struct * n;
  struct expr_list_struct * t;
} expr_list;
struct stmt_struct;
struct proc_struct;
/**
 * s is the stmt in the list
 * n is the next item
 * t is the last item
 *
 * unused fields *must* be NULL.
 * read above (search for %*%*%) before you manipulate these structures.
 **/
typedef struct stmt_list_struct
{
  struct stmt_struct * s;
  struct stmt_list_struct * n;
  struct stmt_list_struct * t;
} stmt_list;
/**
 * ll is the list of labels
 * op is the statement type
 * e is 
 *  for AST_ASSUME, AST_ASSERT: condition
 *  for AST_ITE, AST_WHILE: condition
 * vl is 
 *  for AST_ASSIGN: lvals
 * el is
 *  for AST_ASSIGN: rvals
 *  for AST_CALL: arguments
 *  for AST_RETURN: return rvals 
 * sl1, sl2 are statement lists for if and else clauses
 *
 * unused fields *must* be NULL.
 * read above (search for %*%*%) before you manipulate these structures.
 **/
typedef struct stmt_struct 
{ 
  str_list * ll;
  stmt_type op;
  char * f;
  expr * e;
  str_list * vl;
  expr_list * el;
  stmt_list * sl1;
  stmt_list * sl2;
} stmt;

/**
 * r is the number of return values
 * f is the function name
 * al is the list of arguments
 * vl is the list of local variables
 * e is the 'enforce' condition
 * sl is the statement list for the body
 *
 * unused fields *must* be NULL.
 * read above (search for %*%*%) before you manipulate these structures.
 **/
typedef struct proc_struct
{
  unsigned r;
  char * f;
  str_list * al;
  str_list * vl;
  expr * e;
  stmt_list * sl;
} proc;

/**
 * p is the proc in the list
 * n is the next item
 * t is the last item
 *
 * unused fields *must* be NULL.
 * read above (search for %*%*%) before you manipulate these structures.
 **/
typedef struct proc_list_struct
{
  proc * p;
  struct proc_list_struct * n;
  struct proc_list_struct * t;
} proc_list;

/**
 * vl is the list of global variables
 * pl is the list of procedures
 * e is probably not used any more.
 *
 * unused fields *must* be NULL.  
 * read above (search for %*%*%) before you manipulate these structures.
 **/
typedef struct prog_struct
{
  str_list * vl;
  proc_list * pl;
  stmt * e; //error label
} prog;

// //////////////////////////////CONSTRUCTORS//////////////////////////////////
inline expr * make_clean_expr();
inline expr * make_schoose_expr(expr * l, expr * r);
inline expr * make_non_det_expr();
inline expr * make_var_expr(char * v);
inline expr * make_constant_expr(constant c);
inline expr * make_not_expr(expr * l);
inline expr * make_or_expr(expr * l, expr * r);
inline expr * make_xor_expr(expr * l, expr * r);
inline expr * make_and_expr(expr * l, expr * r);
inline expr * make_eq_expr(expr * l, expr * r);
inline expr * make_neq_expr(expr * l, expr * r);
inline expr * make_imp_expr(expr * l, expr * r);

inline expr * make_post_expr(expr * l);
inline expr * make_deep_copy_expr(expr const * e);

inline stmt * make_clean_stmt();

inline str_list * make_str_list_item(char * v);
inline expr_list * make_expr_list_item(expr * e);
inline stmt_list * make_stmt_list_item(stmt * s);

inline str_list * add_str_right(str_list * vl, char * v);
inline str_list * add_str_left(str_list * vl, char * v);
inline str_list * splice_str(str_list * vll, str_list * vlr);
inline expr_list * add_expr_right(expr_list * el, expr * e);
inline expr_list * add_expr_left(expr_list * el, expr * e);
inline expr_list * splice_expr(expr_list * ell, expr_list * elr);
inline stmt_list * add_stmt_right(stmt_list * sl, stmt * s);
inline stmt_list * add_stmt_left(stmt_list * sl, stmt * s);
inline stmt_list * splice_stmt(stmt_list * sll, stmt_list * slr);

inline stmt * make_call_stmt(char * f, str_list * vl, expr_list * el);
inline stmt * make_assume_stmt(expr * e);
inline stmt * make_assert_stmt(expr * e);
inline stmt * make_ite_stmt(expr * e, stmt_list * slt, stmt_list * sle);
inline stmt * make_while_stmt(expr * e, stmt_list * sl);
inline stmt * make_assign_stmt(str_list * vl, expr_list * el, expr * e);
inline stmt * make_return_stmt(expr_list * el);
inline stmt * make_goto_stmt(str_list * vl);
inline stmt * make_skip_stmt();
inline stmt * add_labels(stmt * s, str_list * ll);

inline proc * make_clean_proc();
inline proc * make_proc(unsigned r, char * f, str_list * al, str_list * vl, expr * e, stmt_list * sl);
inline proc_list * make_proc_list_item(proc * p);
inline proc_list * add_proc_right(proc_list * pl, proc * p);
inline proc_list * add_proc_left(proc_list * pl, proc * p);

inline prog * make_clean_prog();
inline prog * make_prog(str_list * vl, proc_list * pl);

// ////////////////////// Clean up ////////////////////////////////////////////
void deep_erase_expr(expr ** e);
void deep_erase_stmt(stmt ** s);
void deep_erase_proc(proc ** p);
void deep_erase_prog(prog ** pg);
void deep_erase_str_list(str_list ** vl);
void deep_erase_expr_list(expr_list ** el);
void deep_erase_stmt_list(stmt_list ** sl);
void deep_erase_proc_list(proc_list ** pl);

// ////////////////////// Printing ////////////////////////////////////////////
void emit_expr(FILE * fout, const expr * e);
void emit_stmt(FILE * fout, const stmt * s, unsigned indent);
void emit_proc(FILE * fout, const proc * p, unsigned indent);
void emit_prog(FILE * fout, const prog * pg);

void emit_comma_separated_str_list(FILE * fout, const str_list * vl);
void emit_comma_separated_expr_list(FILE * fout, const expr_list * el);
void emit_space(FILE * fout, unsigned length);


// ////////////////////// For lex and yacc ////////////////////////////////////
typedef union YYSTYPE{
	unsigned number;
	char * v;
  str_list * vl;
  expr * e;
  expr_list * el;
  stmt * s;
  stmt_list * sl;
  proc * p;
  proc_list * pl;
  prog * pg;
} YYSTYPE;

extern prog * parsing_result;
int parse(FILE * fin);

#endif //#ifdef __CPROVER_AST_H_
