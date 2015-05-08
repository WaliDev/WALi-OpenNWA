
#include "ast.h"

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
