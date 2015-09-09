
#include "ast.h"


inline expr * make_clean_expr()
{
  expr * e = (expr *) malloc(sizeof(expr));
  e->l = NULL;
  e->r = NULL;
  e->v = NULL;
  return e;
}
inline expr * make_schoose_expr(expr * l, expr * r)
{
  expr * e = NULL;
  if(!l || !r)
    assert(0 && "make_schoose_expr");
  e = make_clean_expr();
  e->op = AST_SCHOOSE;
  e->l = l;
  e->r = r;
  return e;
}
inline expr * make_non_det_expr()
{
  expr * e = make_clean_expr();
  e->op = AST_NONDET;
  return e;
}
inline expr * make_var_expr(char * v)
{
  expr * e = NULL;
  if(!v)
    assert(0 && "make_var_expr");
  e = make_clean_expr();
  e->op = AST_VAR;
  e->v = v;
  return e;
}
inline expr * make_constant_expr(constant c)
{
  expr * e = NULL;
  if(c != ZERO && c != ONE)
    assert(0 && "make_constant_expr");
  e = make_clean_expr();
  e->op = AST_CONSTANT;
  e->c = c;
  return e;
}
inline expr * make_not_expr(expr * l)
{
	expr * e = NULL;
  if(!l)
    assert(0 && "make_not_expr");
  e = make_clean_expr();
  e->op = AST_NOT;
  e->l = l;
  return e;
}
inline expr * make_or_expr(expr * l, expr * r)
{
  expr * e = NULL;
  if(!l || !r)
    assert(0 && "make_or_expr");
  e = make_clean_expr();
  e->op = AST_OR;
  e->l = l;
  e->r = r;
  return e;
}
inline expr * make_xor_expr(expr * l, expr * r)
{
  expr * e = NULL;
  if(!l || !r)
    assert(0 && "make_xor_expr");
  e = make_clean_expr();
  e->op = AST_XOR;
  e->l = l;
  e->r = r;
  return r;
}
inline expr * make_and_expr(expr * l, expr * r)
{
  expr * e = NULL;
  if(!l || !r)
    assert(0 && "make_and_expr");
  e = make_clean_expr();
  e->op = AST_AND;
  e->l = l;
  e->r = r;
  return e;
}
inline expr * make_eq_expr(expr * l, expr * r)
{
  expr * e = NULL;
  if(!l || !r)
    assert(0 && "make_eq_expr");
  e = make_clean_expr();
  e->op = AST_EQ;
  e->l = l;
  e->r = r;
  return e;
}
inline expr * make_neq_expr(expr * l, expr * r)
{
	expr * e = NULL;
  if(!l || !r)
    assert(0 && "make_neq_expr");
  e = make_clean_expr();
  e->op = AST_NEQ;
  e->l = l;
  e->r = r;
  return e;
}
inline expr * make_imp_expr(expr * l, expr * r)
{
	expr * e = NULL;
  if(!l || !r)
    assert(0 && "make_imp_expr");
  e = make_clean_expr();
  e->op = AST_IMP;
  e->l = l;
  e->r = r;
  return e;
}
inline expr * make_post_expr(expr *l)
{
  if(!l)
    assert(0 && "make_post_expr");
  if(!(l->op == AST_VAR || l->op == AST_VAR_POST))
    assert(0 && "make_post_expr");
  l->op = AST_VAR_POST;
  return l;
}
inline expr * make_deep_copy_expr(expr const * e)
{
  expr * res  = NULL;
  if(!e) return NULL;
  res = make_clean_expr();
  res->op = e->op;
  res->l = make_deep_copy_expr(e->l);
  res->r = make_deep_copy_expr(e->r);
  res->c = e->c;
  if(e->v)
    res->v = strdup(e->v);
  return res;
}

inline stmt * make_clean_stmt()
{
  stmt * s = (stmt *) malloc(sizeof(stmt));
  s->ll = NULL;
  s->f = NULL;
  s->e = NULL;
  s->vl = NULL;
  s->el = NULL;
  s->sl1 = NULL;
  s->sl2 = NULL;
  return s;
}

inline str_list * make_str_list_item(char * v)
{
	str_list * sl = NULL;
  if(!v)
    assert(0 && "make_str_list_item");
  sl = (str_list *) malloc(sizeof(str_list));
  sl->n = NULL;
  sl->t = sl;
  sl->v = v;
  return sl;
}
inline expr_list * make_expr_list_item(expr * e)
{
	expr_list * el = NULL;
  if(!e)
    assert(0 && "make_expr_list_item");
  el = (expr_list *) malloc(sizeof(expr_list));
  el->n = NULL;
  el->t = el;
  el->e = e;
  return el;
}
inline stmt_list * make_stmt_list_item(stmt * s)
{
	stmt_list * sl = NULL;
  if(!s)
    assert(0 && "make_stmt_list_item");
  sl = (stmt_list *) malloc(sizeof(stmt_list));
  sl->n = NULL;
  sl->t = sl;
  sl->s = s;
  return sl;
}

inline str_list * add_str_right(str_list * vl, char * v)
{
	str_list * t = NULL;
  if(!vl || !v)
    assert(0 && "add_str_right");
  t = make_str_list_item(v);
  vl->t->n = t;
  vl->t = t;
  return vl;
}
inline str_list * add_str_left(str_list * vl, char * v)
{
	str_list * h = NULL;
  if(!vl || !v)
    assert(0 && "add_str_left");
  h = make_str_list_item(v);
  h->n = vl;
  h->t = vl->t;
  return h;
}
inline str_list * splice_str(str_list * vll, str_list * vlr)
{
  if(!vll)
    return vlr;
  if(!vlr)
    return vll;
  vll->t->n = vlr;
  vll->t = vlr->t;
  return vll;
}
inline expr_list * add_expr_right(expr_list * el, expr * e)
{
	expr_list * t = NULL;
  if(!el || !e)
    assert(0 && "add_expr_right");
  t = make_expr_list_item(e);
  el->t->n = t;
  el->t = t;
  return el;
}
inline expr_list * add_expr_left(expr_list * el, expr * e)
{
	expr_list * h = NULL;
  if(!el || !e)
    assert(0 && "add_expr_left");
  h = make_expr_list_item(e);
  h->n = el;
  h->t = el->t;
  return h;
}
inline expr_list * splice_expr(expr_list * ell, expr_list * elr)
{
  if(!ell)
    return elr;
  if(!elr)
    return ell;
  ell->t->n = elr;
  ell->t = elr->t;
  return ell;
}
inline stmt_list * add_stmt_right(stmt_list * sl, stmt * s)
{
	stmt_list * t = NULL;
  if(!sl || !s)
    assert(0 && "add_stmt_right");
  t = make_stmt_list_item(s);
  sl->t->n = t;
  sl->t = t;
  return sl;
}
inline stmt_list * add_stmt_left(stmt_list * sl, stmt * s)
{
	stmt_list * h = NULL;
  if(!sl || !s)
    assert(0 && "add_stmt_left");
  h = make_stmt_list_item(s);
  h->n = sl;
  h->t = sl->t;
  return h;
}
inline stmt_list * splice_stmt(stmt_list * sll, stmt_list * slr)
{
  if(!sll)
    return slr;
  if(!slr)
    return sll;
  sll->t->n = slr;
  sll->t = slr->t;
  return sll;
}

inline stmt * make_call_stmt(char * f, str_list * vl, expr_list * el)
{
	stmt * s = NULL;
  if(!f)
    assert(0 && "make_call_stmt");
  s = make_clean_stmt();
  s->op = AST_CALL;
  s->f = f;
  s->vl = vl;
  s->el = el;
  return s;
}
inline stmt * make_assume_stmt(expr * e)
{
	stmt * s = NULL;
  if(!e)
    assert(0 && "make_assume_stmt");
  s = make_clean_stmt();
  s->op = AST_ASSUME;
  s->e = e;
  return s;
}
inline stmt * make_assert_stmt(expr * e)
{
	stmt * s = NULL;
  if(!e)
    assert(0 && "make_assert_stmt");
  s = make_clean_stmt();
  s->op = AST_ASSERT;
  s->e = e;
  return s;
}
inline stmt * make_ite_stmt(expr * e, stmt_list * slt, stmt_list * sle)
{
	stmt * s = NULL;
  // The else branch might not have any statements
  if(!e || !slt)
    assert(0 && "make_ite_stmt");
  s = make_clean_stmt();
  s->op = AST_ITE;
  s->e = e;
  s->sl1 = slt;
  s->sl2 = sle;
  return s;
}
inline stmt * make_while_stmt(expr * e, stmt_list * sl)
{
	stmt * s = NULL;
  if(!e || !sl)
    assert(0 && "make_while_stmt");
  s = make_clean_stmt();
  s->op = AST_WHILE;
  s->e = e;
  s->sl1 = sl;
  return s;
}
inline stmt * make_assign_stmt(str_list * vl, expr_list * el, expr * e)
{
	stmt * s = NULL;
  if(!vl || !el)
    assert(0 && "make_assign_stmt");
  s = make_clean_stmt();
  s->op = AST_ASSIGN;
  s->vl = vl;
  s->el = el;
  s->e = e;
  return s;
}
inline stmt * make_return_stmt(expr_list * el)
{
  // The expression list might be empty
  stmt * s = make_clean_stmt();
  s->op = AST_RETURN;
  s->el = el;
  return s;
}
inline stmt * make_goto_stmt(str_list * vl)
{
	stmt * s = NULL;
  if(!vl)
    assert(0 && "make_goto_stmt");
  s = make_clean_stmt();
  s->op = AST_GOTO;
  s->vl = vl;
  return s;
}
inline stmt * make_skip_stmt()
{
  stmt * s = make_clean_stmt();
  s->op = AST_SKIP;
  return s;
}
inline stmt * add_labels(stmt * s, str_list * ll)
{
  if(!s || !ll)
    assert(0 && "add_labels");
  s->ll = ll;
  return s;
}

inline proc * make_clean_proc()
{
  proc * p = (proc *) malloc(sizeof(proc));
  p->f = NULL;
  p->al = NULL;
  p->e = NULL;
  p->sl = NULL;
  return p;
}
inline proc * make_proc(unsigned r, char * f, str_list * al, str_list * vl, expr * e, stmt_list * sl)
{
  // The return list being empty mean the function is void
  // The statement list, arguments, and the enforce may also be empty
	proc * p = NULL;
  if(!f)
    assert(0 && "make_proc");
  p = make_clean_proc();
  p->r = r;
  p->f = f;
  p->al = al;
  p->vl = vl;
  p->e = e;
  p->sl = sl;
  return p;
}
inline proc_list * make_proc_list_item(proc * p)
{
	proc_list * pl = NULL;
  if(!p)
    assert(0 && "make_proc_list_item");
  pl = (proc_list *) malloc(sizeof(proc_list));
  pl->p = p;
  pl->n = NULL;
  pl->t = pl;
  return pl;
}
inline proc_list * add_proc_right(proc_list * pl, proc * p)
{
	proc_list * t = NULL;
  if(!pl || !p)
    assert(0 && "add_proc_right");
  t = make_proc_list_item(p);
  pl->t->n = t;
  pl->t = t;
  return pl;
}
inline proc_list * add_proc_left(proc_list * pl, proc * p)
{
	proc_list * h = NULL;
  if(!pl || !p)
    assert(0 && "add_proc_left");
  h = make_proc_list_item(p);
  h->n = pl;
  h->t = pl->t;
  return h;
}

inline prog * make_clean_prog()
{
  prog * p = (prog *) malloc(sizeof(prog));
  p->vl = NULL;
  p->pl = NULL;
  p->e = NULL;
  return p;
}
inline prog * make_prog(str_list * vl, proc_list * pl)
{
  prog * p = make_clean_prog();
  p->vl = vl;
  p->pl = pl;
  return p;
}

// //////////////////////////////CONSTRUCTORS//////////////////////////////////

// ///////////////////////Clean Up/////////////////////////////////////////////
void deep_erase_expr(expr ** ep)
{
  expr * e = NULL;
  if(!ep)
    assert(0 && "deep_erase_expr");
  e = * ep;
  if(e){
  deep_erase_expr(&(e->l));
  deep_erase_expr(&(e->r));
  free(e->v);
  free(e);
  }
  *ep = NULL;
}
void deep_erase_stmt(stmt ** sp)
{
	stmt * s = NULL;
  if(!sp)
    assert(0 && "deep_erase_stmt");
  s = *sp;
  if(s){
    deep_erase_str_list(&(s->ll));
    free(s->f);
    deep_erase_expr(&(s->e));
    deep_erase_str_list(&(s->vl));
    deep_erase_expr_list(&(s->el));
    deep_erase_stmt_list(&(s->sl1));
    deep_erase_stmt_list(&(s->sl2));
    free(s);
  }
  *sp = NULL;
}
void deep_erase_proc(proc ** pp)
{
	proc * p = NULL;
  if(!pp)
    assert(0 && "deep_erase_proc");
  p = *pp;
  if(p){
    free(p->f);
    deep_erase_str_list(&(p->al));
    deep_erase_str_list(&(p->vl));
    deep_erase_expr(&(p->e));
    deep_erase_stmt_list(&(p->sl));
    free(p);
  }
  *pp = NULL;
}
void deep_erase_prog(prog ** ppg)
{
	prog * pg = NULL;
  if(!ppg)
    assert(0 && "deep_erase_prog");
  pg = *ppg;
  if(pg){
    deep_erase_str_list(&(pg->vl));
    deep_erase_proc_list(&(pg->pl));
    free(pg);
  }
  *ppg = NULL;
}
void deep_erase_str_list(str_list ** pvl)
{
	str_list * vl = NULL, * nvl;
  if(!pvl)
    assert(0 && "deep_erase_str_list");
  vl = *pvl, nvl;
  while(vl){
    nvl = vl->n;
    free(vl->v);
    free(vl);
    vl = nvl;
  }
  *pvl = NULL;
}
void deep_erase_expr_list(expr_list ** pel)
{
	expr_list * el = NULL, * nel;
  if(!pel)
    assert(0 && "deep_erase_expr_list");
  el = *pel, nel;
  while(el){
    nel = el->n;
    deep_erase_expr(&(el->e));
    free(el);
    el = nel;
  }
  *pel = NULL;
}
void deep_erase_stmt_list(stmt_list ** psl)
{
	stmt_list * sl = NULL, * nsl;
  if(!psl)
    assert(0 && "deep_erase_stmt_list");
  sl = *psl, nsl;
  while(sl){
    nsl = sl->n;
    deep_erase_stmt(&(sl->s));
    free(sl);
    sl = nsl;
  }
  *psl = NULL;
}
void deep_erase_proc_list(proc_list ** ppl)
{
	proc_list * pl = NULL, * npl;
  if(!ppl)
    assert(0 && "deep_erase_proc_list");
  pl = *ppl, npl;
  while(pl){
    npl = pl->n;
    deep_erase_proc(&(pl->p));
    free(pl);
    pl = npl;
  }
  *ppl = NULL;
}


// ////////////////////////Printing////////////////////////////////////////////
void emit_expr(FILE * fout, const expr * e)
{
  if(!e)
    assert(0 && "emit_expr");
  switch(e->op){
    case AST_NOT:
      fprintf(fout, "(!");
      emit_expr(fout, e->l);
      fprintf(fout, ")");
      break;
    case AST_XOR:
    case AST_OR:
    case AST_AND:
    case AST_EQ:
    case AST_NEQ:
    case AST_IMP:
      fprintf(fout, "(");
      emit_expr(fout, e->l);
      switch(e->op){
        case AST_XOR:
          fprintf(fout, " ^ ");
          break;
        case AST_OR:
          fprintf(fout, " || ");
          break;
        case AST_AND:
          fprintf(fout, " && ");
          break;
        case AST_EQ:
          fprintf(fout, " = ");
          break;
        case AST_NEQ:
          fprintf(fout, " != ");
          break;
        case AST_IMP:
          fprintf(fout, " => ");
          break;
        default:
          assert(0 && "emit_expr(expr) Not possible");
      }
      emit_expr(fout, e->r);
      fprintf(fout, ")");
      break;
    case AST_NONDET:
      fprintf(fout, "*");
      break;
    case AST_SCHOOSE:
      fprintf(fout, "schoose [");
      emit_expr(fout, e->l);
      fprintf(fout, ", ");
      emit_expr(fout, e->r);
      fprintf(fout, "]");
      break;
    case AST_VAR:
      fprintf(fout, "%s", e->v);
      break;
    case AST_VAR_POST:
      fprintf(fout, "\'%s", e->v);
      break;
    case AST_CONSTANT:
      fprintf(fout, "%d", (e->c == ONE)? 1 : 0);
      break;
    default:
      assert(0 && "emit_expr: Unknown case");
  }
}
void emit_stmt(FILE * fout, const stmt * s, unsigned indent)
{
	const str_list * ll = NULL;
	const stmt_list * sl = NULL;
  if(!s)
    assert(0 && "emit_stmt");

  emit_space(fout, indent);

  ll = s->ll;
  while(ll){
    fprintf(fout, "%s: ", ll->v);
    ll = ll->n;
  }

  
  switch(s->op){
    case AST_SKIP:
      fprintf(fout, "skip;\n");
      break;
    case AST_GOTO:
      fprintf(fout, "goto ");
      emit_comma_separated_str_list(fout, s->vl);
      fprintf(fout, ";\n");
      break;
    case AST_RETURN:
      fprintf(fout, "return ");
      emit_comma_separated_expr_list(fout, s->el);
      fprintf(fout, ";\n");
      break;
    case AST_ASSIGN:
      emit_comma_separated_str_list(fout, s->vl);
      fprintf(fout, " := ");
      emit_comma_separated_expr_list(fout, s->el);
      if(s->e){
        fprintf(fout, " constrain ");
        emit_expr(fout, s->e);  
      }
      fprintf(fout, ";\n");
      break;
    case AST_ITE:
      fprintf(fout, "if(");
      emit_expr(fout, s->e);
      fprintf(fout, ") then\n");
      sl = s->sl1;
      while(sl){
        emit_stmt(fout, sl->s, indent + 2);
        sl = sl->n;
      }
      if(s->sl2){
        emit_space(fout, indent);
        fprintf(fout, "else\n");
        sl = s->sl2;
        while(sl){
          emit_stmt(fout, sl->s, indent + 2);
          sl = sl->n;
        }
      }
      emit_space(fout, indent);
      fprintf(fout, "fi\n");
      break;
    case AST_WHILE:
      fprintf(fout, "while(");
      emit_expr(fout, s->e);
      fprintf(fout, ") do\n");
      sl = s->sl1;
      while(sl){
        emit_stmt(fout, sl->s, indent + 2);
        sl = sl->n;
      }
      emit_space(fout, indent);
      fprintf(fout, "od\n");
      break;
    case AST_ASSERT:
      fprintf(fout, "assert ");
      emit_expr(fout, s->e);
      fprintf(fout, ";\n");
      break;
    case AST_ASSUME:
      fprintf(fout, "assume ");
      emit_expr(fout, s->e);
      fprintf(fout, ";\n");
      break;
    case AST_CALL:
      if(s->vl){
        emit_comma_separated_str_list(fout, s->vl);
        fprintf(fout, " := ");
      }
      fprintf(fout, "%s(", s->f);
      emit_comma_separated_expr_list(fout, s->el);
      fprintf(fout, ");\n");
      break;
    default:
      assert(0 && "emit_stmt: Unknown case");
      break;
  }
}
void emit_proc(FILE * fout, const proc * p, unsigned indent)
{
	const str_list * vl = NULL;
	const stmt_list * sl = NULL;
  if(!p)
    assert(0 && "emit_proc");
  emit_space(fout, indent);
  if(p->r == 0)
    fprintf(fout, "void ");
  else if(p->r == 1)
    fprintf(fout, "bool ");
  else 
    fprintf(fout, "bool <%d> ", p->r);
  fprintf(fout, "%s", p->f);
  fprintf(fout, "(");
  emit_comma_separated_str_list(fout, p->al);
  fprintf(fout, ") begin\n");

  vl = p->vl;
  while(vl){
    emit_space(fout, indent + 2);
    fprintf(fout, "decl %s;", vl->v);
    vl = vl->n;
  }
  
  if(p->e){
    emit_space(fout, indent + 2);
    fprintf(fout, "enforce ");
    emit_expr(fout, p->e);
    fprintf(fout, ";\n");
  }

   sl = p->sl;
  while(sl){
    emit_stmt(fout, sl->s, indent + 2);
    sl = sl->n;
  }
  
  emit_space(fout, indent);
  fprintf(fout, "end\n\n");
}
void emit_prog(FILE * fout, const prog * pg)
{
  const str_list * vl = pg->vl;
  const proc_list * pl = pg->pl;
  while(vl){
    fprintf(fout, "decl %s;\n", vl->v);
    vl = vl->n;
  }

  while(pl){
    emit_proc(fout, pl->p, 0);
    pl = pl->n;
  }
}

void emit_comma_separated_str_list(FILE * fout, const str_list * vl)
{
  short first = 1;
  while(vl){
    if(!first)
      fprintf(fout, ", ");
    fprintf(fout, "%s", vl->v);
    first = 0;
    vl = vl->n;
  }
}
void emit_comma_separated_expr_list(FILE * fout, const expr_list * el)
{
  short first = 1;
  while(el){
    if(!first)
      fprintf(fout, ", ");
    emit_expr(fout, el->e);
    first = 0;
    el = el->n;
  }
}
void emit_space(FILE * fout, unsigned length)
{
  unsigned i = 0;
  while(i < length){
    fprintf(fout, " ");
    i++;
  }
}
