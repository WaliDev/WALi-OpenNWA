#ifndef __CPROVER_AST_H_
#define __CPROVER_AST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef enum {ZERO = 0, ONE = 1} constant;

typedef enum {AST_NOT, AST_XOR, AST_OR, AST_AND, AST_EQ, AST_NEQ, AST_IMP, AST_NONDET, AST_SCHOOSE, AST_VAR, AST_VAR_POST, AST_CONSTANT} binop;
typedef struct expr_struct
{
  binop op;
  struct expr_struct * l;
  struct expr_struct * r;
  constant c;
  char * v;
} expr;

typedef enum {AST_SKIP, AST_GOTO, AST_RETURN, AST_ASSIGN, AST_ITE, AST_WHILE, AST_ASSERT, AST_ASSUME, AST_CALL} stmt_type;
typedef struct str_list_struct 
{
  char * v;
  struct str_list_struct * n;
  struct str_list_struct * t;
} str_list;
typedef struct expr_list_struct
{
  expr * e;
  struct expr_list_struct * n;
  struct expr_list_struct * t;
} expr_list;
struct stmt_struct;
struct proc_struct;
typedef struct stmt_list_struct
{
  struct stmt_struct * s;
  struct stmt_list_struct * n;
  struct stmt_list_struct * t;
} stmt_list;
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

typedef struct proc_struct
{
  unsigned r;
  char * f;
  str_list * al;
  str_list * vl;
  expr * e;
  stmt_list * sl;
} proc;
typedef struct proc_list_struct
{
  proc * p;
  struct proc_list_struct * n;
  struct proc_list_struct * t;
} proc_list;

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
inline expr * mark_post_expr(expr * l);

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
