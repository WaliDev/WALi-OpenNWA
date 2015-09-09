#ifndef CPROVER_AST_INL
#define CPROVER_AST_INL

__inline expr * make_clean_expr()
{
  expr * e = (expr *) malloc(sizeof(expr));
  e->l = NULL;
  e->r = NULL;
  e->v = NULL;
  return e;
}
__inline expr * make_schoose_expr(expr * l, expr * r)
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
__inline expr * make_non_det_expr()
{
  expr * e = make_clean_expr();
  e->op = AST_NONDET;
  return e;
}
__inline expr * make_var_expr(char * v)
{
  expr * e = NULL;
  if(!v)
    assert(0 && "make_var_expr");
  e = make_clean_expr();
  e->op = AST_VAR;
  e->v = v;
  return e;
}
__inline expr * make_constant_expr(constant c)
{
  expr * e = NULL;
  if(c != ZERO && c != ONE)
    assert(0 && "make_constant_expr");
  e = make_clean_expr();
  e->op = AST_CONSTANT;
  e->c = c;
  return e;
}
__inline expr * make_not_expr(expr * l)
{
	expr * e = NULL;
  if(!l)
    assert(0 && "make_not_expr");
  e = make_clean_expr();
  e->op = AST_NOT;
  e->l = l;
  return e;
}
__inline expr * make_or_expr(expr * l, expr * r)
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
__inline expr * make_xor_expr(expr * l, expr * r)
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
__inline expr * make_and_expr(expr * l, expr * r)
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
__inline expr * make_eq_expr(expr * l, expr * r)
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
__inline expr * make_neq_expr(expr * l, expr * r)
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
__inline expr * make_imp_expr(expr * l, expr * r)
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
__inline expr * make_post_expr(expr *l)
{
  if(!l)
    assert(0 && "make_post_expr");
  if(!(l->op == AST_VAR || l->op == AST_VAR_POST))
    assert(0 && "make_post_expr");
  l->op = AST_VAR_POST;
  return l;
}
__inline expr * make_deep_copy_expr(expr const * e)
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

__inline stmt * make_clean_stmt()
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

__inline str_list * make_str_list_item(char * v)
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
__inline expr_list * make_expr_list_item(expr * e)
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
__inline stmt_list * make_stmt_list_item(stmt * s)
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

__inline str_list * add_str_right(str_list * vl, char * v)
{
	str_list * t = NULL;
  if(!vl || !v)
    assert(0 && "add_str_right");
  t = make_str_list_item(v);
  vl->t->n = t;
  vl->t = t;
  return vl;
}
__inline str_list * add_str_left(str_list * vl, char * v)
{
	str_list * h = NULL;
  if(!vl || !v)
    assert(0 && "add_str_left");
  h = make_str_list_item(v);
  h->n = vl;
  h->t = vl->t;
  return h;
}
__inline str_list * splice_str(str_list * vll, str_list * vlr)
{
  if(!vll)
    return vlr;
  if(!vlr)
    return vll;
  vll->t->n = vlr;
  vll->t = vlr->t;
  return vll;
}
__inline expr_list * add_expr_right(expr_list * el, expr * e)
{
	expr_list * t = NULL;
  if(!el || !e)
    assert(0 && "add_expr_right");
  t = make_expr_list_item(e);
  el->t->n = t;
  el->t = t;
  return el;
}
__inline expr_list * add_expr_left(expr_list * el, expr * e)
{
	expr_list * h = NULL;
  if(!el || !e)
    assert(0 && "add_expr_left");
  h = make_expr_list_item(e);
  h->n = el;
  h->t = el->t;
  return h;
}
__inline expr_list * splice_expr(expr_list * ell, expr_list * elr)
{
  if(!ell)
    return elr;
  if(!elr)
    return ell;
  ell->t->n = elr;
  ell->t = elr->t;
  return ell;
}
__inline stmt_list * add_stmt_right(stmt_list * sl, stmt * s)
{
	stmt_list * t = NULL;
  if(!sl || !s)
    assert(0 && "add_stmt_right");
  t = make_stmt_list_item(s);
  sl->t->n = t;
  sl->t = t;
  return sl;
}
__inline stmt_list * add_stmt_left(stmt_list * sl, stmt * s)
{
	stmt_list * h = NULL;
  if(!sl || !s)
    assert(0 && "add_stmt_left");
  h = make_stmt_list_item(s);
  h->n = sl;
  h->t = sl->t;
  return h;
}
__inline stmt_list * splice_stmt(stmt_list * sll, stmt_list * slr)
{
  if(!sll)
    return slr;
  if(!slr)
    return sll;
  sll->t->n = slr;
  sll->t = slr->t;
  return sll;
}

__inline stmt * make_call_stmt(char * f, str_list * vl, expr_list * el)
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
__inline stmt * make_assume_stmt(expr * e)
{
	stmt * s = NULL;
  if(!e)
    assert(0 && "make_assume_stmt");
  s = make_clean_stmt();
  s->op = AST_ASSUME;
  s->e = e;
  return s;
}
__inline stmt * make_assert_stmt(expr * e)
{
	stmt * s = NULL;
  if(!e)
    assert(0 && "make_assert_stmt");
  s = make_clean_stmt();
  s->op = AST_ASSERT;
  s->e = e;
  return s;
}
__inline stmt * make_ite_stmt(expr * e, stmt_list * slt, stmt_list * sle)
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
__inline stmt * make_while_stmt(expr * e, stmt_list * sl)
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
__inline stmt * make_assign_stmt(str_list * vl, expr_list * el, expr * e)
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
__inline stmt * make_return_stmt(expr_list * el)
{
  // The expression list might be empty
  stmt * s = make_clean_stmt();
  s->op = AST_RETURN;
  s->el = el;
  return s;
}
__inline stmt * make_goto_stmt(str_list * vl)
{
	stmt * s = NULL;
  if(!vl)
    assert(0 && "make_goto_stmt");
  s = make_clean_stmt();
  s->op = AST_GOTO;
  s->vl = vl;
  return s;
}
__inline stmt * make_skip_stmt()
{
  stmt * s = make_clean_stmt();
  s->op = AST_SKIP;
  return s;
}
__inline stmt * add_labels(stmt * s, str_list * ll)
{
  if(!s || !ll)
    assert(0 && "add_labels");
  s->ll = ll;
  return s;
}

__inline proc * make_clean_proc()
{
  proc * p = (proc *) malloc(sizeof(proc));
  p->f = NULL;
  p->al = NULL;
  p->e = NULL;
  p->sl = NULL;
  return p;
}
__inline proc * make_proc(unsigned r, char * f, str_list * al, str_list * vl, expr * e, stmt_list * sl)
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
__inline proc_list * make_proc_list_item(proc * p)
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
__inline proc_list * add_proc_right(proc_list * pl, proc * p)
{
	proc_list * t = NULL;
  if(!pl || !p)
    assert(0 && "add_proc_right");
  t = make_proc_list_item(p);
  pl->t->n = t;
  pl->t = t;
  return pl;
}
__inline proc_list * add_proc_left(proc_list * pl, proc * p)
{
	proc_list * h = NULL;
  if(!pl || !p)
    assert(0 && "add_proc_left");
  h = make_proc_list_item(p);
  h->n = pl;
  h->t = pl->t;
  return h;
}

__inline prog * make_clean_prog()
{
  prog * p = (prog *) malloc(sizeof(prog));
  p->vl = NULL;
  p->pl = NULL;
  p->e = NULL;
  return p;
}
__inline prog * make_prog(str_list * vl, proc_list * pl)
{
  prog * p = make_clean_prog();
  p->vl = vl;
  p->pl = pl;
  return p;
}
#endif