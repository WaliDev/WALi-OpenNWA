#include "BplToPds.hpp"

#include <string>
#include <sstream>

using namespace std;
using namespace wali;
using namespace wali::wpds;
using namespace wali::domains::binrel;
using namespace wali::cprover;
using namespace wali::cprover::resolve_details;

namespace wali
{
  namespace cprover
  {
    namespace resolve_details
    {

      void clear_stmt_ptr_stmt_list_ptr_hash_map(stmt_ptr_stmt_list_ptr_hash_map& m)
      {
        for(stmt_ptr_stmt_list_ptr_hash_map::iterator iter = m.begin(); iter != m.end(); ++iter){
          stmt_list * sl = iter->second, * nsl;
          while(sl){
            nsl = sl->n;
            free(sl);
            sl = nsl;
          }
          iter->second = NULL;
        }
        m.clear();
      }

      static void map_label_to_stmt_helper(str_stmt_ptr_hash_map& m, const stmt_list * sl)
      {
        const str_list * ll;
        while(sl){
          ll = sl->s->ll;
          while(ll){
            assert(m.find(ll->v) == m.end());
            m[ll->v] = sl->s;
            ll = ll->n;
          }
          if(sl->s->sl1)
            map_label_to_stmt_helper(m, sl->s->sl1);
          if(sl->s->sl2)
            map_label_to_stmt_helper(m, sl->s->sl2);
          sl = sl->n;
        }
      }
      void map_label_to_stmt(str_stmt_ptr_hash_map& m, const proc * p)
      {
        map_label_to_stmt_helper(m, p->sl);
      }

      void map_name_to_proc(str_proc_ptr_hash_map& m, const prog * pg)
      {
        const proc_list * pl;
        pl = pg->pl;
        while(pl){
          assert(m.find(pl->p->f) == m.end());
          m[pl->p->f] = pl->p;
          pl = pl->n;
        }
      }

      static void map_goto_to_targets_helper(stmt_ptr_stmt_list_ptr_hash_map& mout, str_stmt_ptr_hash_map& min, const stmt_list * sl)
      {
        while(sl){
          if(sl->s->op == AST_GOTO){
            assert(mout.find(sl->s) == mout.end());
            str_list * vl = sl->s->vl;
            while(vl){
              assert(min.find(vl->v) != min.end());
              if(mout.find(sl->s) == mout.end())
                mout[sl->s] = make_stmt_list_item(min[vl->v]);
              else
                add_stmt_right(mout[sl->s],min[vl->v]);            
              vl = vl->n;
            }
          }
          if(sl->s->sl1)
            map_goto_to_targets_helper(mout, min, sl->s->sl1);
          if(sl->s->sl2)
            map_goto_to_targets_helper(mout, min, sl->s->sl2);
          sl = sl->n;
        }
      }
      void map_goto_to_targets(stmt_ptr_stmt_list_ptr_hash_map& mout, str_stmt_ptr_hash_map& min, const proc * p)
      {
        map_goto_to_targets_helper(mout, min, p->sl);
      }


      void map_call_to_callee_helper(stmt_ptr_proc_ptr_hash_map& mout, str_proc_ptr_hash_map& min, const stmt_list * sl)
      {
        while(sl){
          if(sl->s->op == AST_CALL){
            assert(mout.find(sl->s) == mout.end());
            assert(min.find(sl->s->f) != min.end());
            mout[sl->s] = min[sl->s->f];
          }
          if(sl->s->sl1)
            map_call_to_callee_helper(mout, min, sl->s->sl1);
          if(sl->s->sl2)
            map_call_to_callee_helper(mout, min, sl->s->sl2);
          sl = sl->n;
        }
      }
      void map_call_to_callee(stmt_ptr_proc_ptr_hash_map& mout, str_proc_ptr_hash_map& min, const proc * p)
      {
        map_call_to_callee_helper(mout, min, p->sl);
      }

    }

    void dump_pds_from_prog(wpds::WPDS * pds, const prog * pg)
    {
      ProgramBddContext * con = new ProgramBddContext(); 

      const str_list * vl = pg->vl;
      while(vl){
        con->addBoolVar(string(vl->v));
        vl = vl->n;
      }
      const proc_list * pl = pg->pl;
      while(pl){
        vl = pl->p->al;
        while(vl){
          stringstream ss;
          ss << pl->p->f << "::" << vl->v;
          con->addBoolVar(ss.str());
          vl = vl->n;
        }
        vl = pl->p->vl;
        while(vl){
          stringstream ss;
          ss << pl->p->f << "::" << vl->v;
          con->addBoolVar(ss.str());
          vl = vl->n;
        }
        pl = pl->n;
      }

      str_stmt_ptr_hash_map label_to_stmt;
      str_proc_ptr_hash_map name_to_proc;
      stmt_ptr_stmt_list_ptr_hash_map goto_to_targets;
      stmt_ptr_proc_ptr_hash_map call_to_callee;

      map_name_to_proc(name_to_proc, pg);

      pl = pg->pl;
      while(pl){
        const proc * p = pl->p;
        map_label_to_stmt(label_to_stmt, p);
        map_goto_to_targets(goto_to_targets, label_to_stmt, p);
        map_call_to_callee(call_to_callee, name_to_proc, p);

        dump_pds_from_proc(pds, p, con, goto_to_targets, call_to_callee); 

        label_to_stmt.clear();
        clear_stmt_ptr_stmt_list_ptr_hash_map(goto_to_targets);
        call_to_callee.clear();
        pl = pl->n;
      }
      name_to_proc.clear();
    }
  

   
    bdd expr_as_bdd(const expr * e, const program_bdd_context_t con, const char * f)
    {
      if(!e)
        assert(0 && "expr_as_bdd");
      bdd l = bddfalse, r = bddfalse;
      stringstream ss;
      if(e->l)
        l = expr_as_bdd(e->l, con, f);
      if(e->r)
        l = expr_as_bdd(e->r, con, f);
      switch(e->op){
        case AST_NOT:
          return con->Not(l);
        case AST_XOR:
          return con->Or(con->And(l, con->Not(r)), con->And(con->Not(l), r));
        case AST_OR:
          return con->Or(l, r);
        case AST_AND:
          return con->And(l, r);
        case AST_EQ:
          return con->Or(con->And(l, r), con->And(con->Not(l), con->Not(r)));
        case AST_NEQ:
          return con->Or(con->And(l, con->Not(r)), con->And(con->Not(l), r));
        case AST_IMP:
          return con->Or(con->Not(l), r);
        case AST_NONDET:
          return con->NonDet();
        case AST_SCHOOSE:
          assert(0 && "expr_as_bdd: AST_SCHOOSE not implemented");
        case AST_VAR:
          ss << f << "::" << e->v;
          if(con->find(ss.str()) != con->end()){
            return con->From(ss.str());
          }else{
            assert(con->find(string(e->v)) != con->end());
            return con->From(string(e->v));
          }
        case AST_CONSTANT:
          if(e->c == ONE)
            return con->True();
          else
            return con->False();
        default:
          assert(0 && "expr_as_bdd: Unknown case");
      }
    }

    static wali::Key stt()
    {
      return getKey("Unique State Name");
    }

    static wali::Key stk(const stmt * s)
    {
      return getKey((long) s);
    };

    void dump_pds_from_stmt(WPDS * pds, const stmt * s, const ProgramBddContext * con, const stmt_ptr_stmt_list_ptr_hash_map& goto_to_targets, const
        stmt_ptr_proc_ptr_hash_map& call_to_callee, const char * f, stmt * ns)
    {
      binrel_t temp = new BinRel(con, con->True());
      binrel_t one = dynamic_cast<BinRel*>(temp->one().get_ptr());
      bdd b;
      str_list * vl;
      expr_list * el;
      stmt_list * sl;
      stmt_ptr_stmt_list_ptr_hash_map::const_iterator goto_iter;

      if(!s)
        assert(0 && "dump_pds_from_stmt");
      switch(s->op){
        case AST_SKIP:
          pds->add_rule(stt(), stk(s), stt(), stk(ns), one);
          break;
        case AST_GOTO:
          goto_iter = goto_to_targets.find(s);
          assert(goto_iter != goto_to_targets.end());
          sl = goto_iter->second;
          while(sl){
            pds->add_rule(stt(), stk(s), stt(), stk(sl->s), one);
          }
          break;
        case AST_RETURN:
          pds->add_rule(stt(), stk(s), stt(), one);
          break;
        case AST_ASSIGN:
          b = bddtrue;
          vl = s->vl;
          el = s->el;
          while(vl || el){
            if(!vl || !el)
              assert(0 && "[dump_pds_from_stmt] Assignment should have the same number of lhs/rhs");
            b = b & con->Assign(string(vl->v), expr_as_bdd(el->e, con, f));
            vl = vl->n;
            el = el->n;
          }            
          pds->add_rule(stt(), stk(s), stt(), stk(ns), new BinRel(con, b));
          break;
        case AST_ITE:   
          if(s->sl1)
            dump_pds_from_stmt_list(pds, s->sl1, con, goto_to_targets, call_to_callee, f, ns);
          if(s->sl2)
            dump_pds_from_stmt_list(pds, s->sl2, con, goto_to_targets, call_to_callee, f, ns);
          break;
      }
    }

    static void dump_pds_from_stmt_list(WPDS * pds, const stmt_list * sl, const ProgramBddContext * con, const stmt_ptr_stmt_list_ptr_hash_map&
        goto_to_targets, const stmt_ptr_proc_ptr_hash_map& call_to_callee, const char * f, stmt * es)
    {
      while(sl){
        if(sl->n)
          dump_pds_from_stmt(pds, sl->s, con, goto_to_targets, call_to_callee, f, sl->n->s);
        else
          dump_pds_from_stmt(pds, sl->s, con, goto_to_targets, call_to_callee, f, es);
        sl = sl->n;
      }
    }
    void dump_pds_from_proc(WPDS * pds, const proc * p, const ProgramBddContext * con, const stmt_ptr_stmt_list_ptr_hash_map& goto_to_targets, const stmt_ptr_proc_ptr_hash_map& call_to_callee)
    {
      dump_pds_from_stmt_list(pds, p->sl, con, goto_to_targets, call_to_callee, p->f, NULL);
    }
  }
}



















int main(int argc, char ** argv)
{
  FILE * fin;
  if(argc >= 2)
    fin = fopen(argv[1], "r");
  else
    fin = fopen("satabs.3.bp", "r"); 

  if(parse(fin))
    assert(0 &&  "Parsing Error");
  prog * pg = parsing_result;
  parsing_result = NULL;
  fclose(fin);

  emit_prog(stdout, pg);

  const proc_list * pl;

  str_stmt_ptr_hash_map label_to_stmt;
  str_proc_ptr_hash_map name_to_proc;
  stmt_ptr_stmt_list_ptr_hash_map goto_to_targets;
  stmt_ptr_proc_ptr_hash_map call_to_callee;

  map_name_to_proc(name_to_proc, pg);

  fprintf(stdout, "Printing name_to_proc\n#######################\n");
  for(str_proc_ptr_hash_map::iterator iter = name_to_proc.begin(); iter != name_to_proc.end(); ++iter)
    fprintf(stdout, "%s: %s\n", iter->first, iter->second->f);

  pl = pg->pl;
  while(pl){
    proc * p = pl->p;
    map_label_to_stmt(label_to_stmt, p);
    map_goto_to_targets(goto_to_targets, label_to_stmt, p);
    map_call_to_callee(call_to_callee, name_to_proc, p);

    fprintf(stdout, "PROC: %s\n@@@@@@@@@@@@@@@@@@@@\n@@@@@@@@@@@@@@@@@@@@\n", p->f);
    fprintf(stdout, "Printing label_to_stmt\n#######################\n");
    for(str_stmt_ptr_hash_map::iterator iter = label_to_stmt.begin(); iter != label_to_stmt.end(); ++iter){
      fprintf(stdout, "%s ---> ", iter->first);
      emit_stmt(stdout, iter->second, 0);
    }
    fprintf(stdout, "Printing goto_to_targets\n#######################\n");
    for(stmt_ptr_stmt_list_ptr_hash_map::iterator iter = goto_to_targets.begin(); iter != goto_to_targets.end(); ++iter){
      emit_stmt(stdout, iter->first, 0);
      stmt_list * sl = iter->second;
      while(sl){
        emit_stmt(stdout, sl->s, 2);
        sl = sl->n;
      }
    }
    fprintf(stdout, "Printing call_to_callee\n#######################\n");
    for(stmt_ptr_proc_ptr_hash_map::iterator iter = call_to_callee.begin(); iter != call_to_callee.end(); ++iter){
      emit_stmt(stdout, iter->first, 0);
      fprintf(stdout, "  %s\n", iter->second->f);
    }

    label_to_stmt.clear();
    clear_stmt_ptr_stmt_list_ptr_hash_map(goto_to_targets);
    call_to_callee.clear();
    pl = pl->n;
  }
  name_to_proc.clear();

  deep_erase_prog(&pg);
  return 0;
}

