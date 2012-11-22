#include "BplToPds.hpp"

#include <string>
#include <sstream>
#include <map>

using namespace std;
using namespace wali;
using namespace wali::wpds;
using namespace wali::wpds::fwpds;
using namespace wali::domains::binrel;
using namespace wali::cprover;
using namespace wali::cprover::details;
using namespace wali::cprover::details::resolve_details;

#define MILLION 1000000
//#define MILLION 0

namespace wali
{
  namespace domains
  {
    namespace binrel
    {
      //We're breaking the ProgramBddContext abstraction here to get the bdds for constrain clauses.
      static bdd xformer_for_constrain(const expr * e, ProgramBddContext * con, const char * f)
      {
        bddinfo_t varInfo;
        string s;
        if(!e)
          assert(0 && "xformer_for_constrain");
        bdd l = bddfalse, r = bddfalse;
        stringstream ss;
        if(e->l)
          l = xformer_for_constrain(e->l, con, f);
        if(e->r)
          r = xformer_for_constrain(e->r, con, f);
        switch(e->op){
          case AST_NOT:
            return bdd_not(l);
          case AST_XOR:
            return bdd_or(bdd_and(l, bdd_not(r)), bdd_and(bdd_not(l), r));
          case AST_OR:
            return bdd_or(l, r);
          case AST_AND:
            return bdd_and(l, r);
          case AST_EQ:
            return bdd_or(bdd_and(l, r), bdd_and(bdd_not(l), bdd_not(r)));
          case AST_NEQ:
            return bdd_or(bdd_and(l, bdd_not(r)), bdd_and(bdd_not(l), r));
          case AST_IMP:
            return bdd_or(bdd_not(l), r);
          case AST_SCHOOSE:
            assert(0 && "xformer_for_constrain: AST_SCHOOSE not implemented");
          case AST_VAR:
          case AST_VAR_POST:
            ss << f << "::" << e->v;
            s = ss.str();
            if(con->find(ss.str()) == con->end()){
              stringstream ss2;
              ss2 << "::" << e->v;
              assert(con->find(ss2.str()) != con->end());
              s = ss2.str();
            }
            varInfo = con->find(s)->second;
            if(e->op == AST_VAR)
              return bdd_ithvar(varInfo->baseLhs);
            else // e->op == AST_VAR_POST
              return bdd_ithvar(varInfo->baseRhs);
          case AST_NONDET:
            return bddtrue;
          case AST_CONSTANT:
            if(e->c == ONE)
              return bddtrue;
            else
              return bddfalse;
          default:
            assert(0 && "expr_as_bdd: Unknown case");
        }
      }
    }
  }
  namespace cprover
  {
    namespace details
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

      
        // Froward declaration of static procedures having to do with instrumeting asserts. 
        // The procedures are static because it is *wrong* to use them directly.
        static void instrument_asserts_in_proc(proc * p, const char * errLbl);
        static void instrument_asserts_in_stmt_list(stmt_list * sl, const char * errLbl);
        static void instrument_asserts_in_stmt(stmt * s, const char * errLbl);

        void instrument_asserts_prog(prog * pg, const char * errLbl)
        {          
          assert(pg && "instrument_asserts");
          // Convert asserts to assumes + goto
          proc_list * pl = pg->pl;
          while(pl){
            proc * p = pl->p;
            instrument_asserts_in_proc(p, errLbl);
            pl = pl->n;
          }
          // Add the error label to a new proc.
          str_list * l = make_str_list_item(strdup(errLbl));
          stmt * s = make_goto_stmt(l);
          str_list * ll = make_str_list_item(strdup(errLbl));
          s->ll = ll;
          stmt_list * sl = make_stmt_list_item(s);
          proc * m = make_proc(0, strdup(errLbl), NULL, NULL, NULL, sl);
          pg->pl = add_proc_right(pg->pl, m);

          // Update error label in program struct.
          pg->e = s;
          return;
        }

        static void instrument_asserts_in_proc(proc * p, const char * errLbl)
        {        
          assert(p && "instrument_asserts_in_proc");
          if(p->sl)
            instrument_asserts_in_stmt_list(p->sl, errLbl);
        }

        static void instrument_asserts_in_stmt_list(stmt_list * sl, const char * errLbl)
        {
          while(sl){
            instrument_asserts_in_stmt(sl->s, errLbl);
            sl = sl->n;
          }
        }

        static void instrument_asserts_in_stmt(stmt * s, const char * errLbl)
        {
          assert(s && "instrument_asserts_in_stmt");
          if(s->sl1)
            instrument_asserts_in_stmt_list(s->sl1, errLbl);
          if(s->sl1)
            instrument_asserts_in_stmt_list(s->sl2, errLbl);
          if(s->op == AST_ASSERT){
            assert(!s->sl1 && !s->sl2);
            // in place modification saves us from having to change the containing stmt_list
            s->op = AST_ITE;
            //condition holds
            stmt * s1 = make_skip_stmt();
            stmt_list * sl1 = make_stmt_list_item(s1);
            s->sl1 = sl1;
            //condition violated
            stmt * s2 = make_call_stmt(strdup(errLbl), NULL, NULL); //We simply call the 'error' proc
            stmt_list * sl2 = make_stmt_list_item(s2);
            s->sl2 = sl2;
          }
        }


        void make_void_returns_explicit_in_prog(prog * pg)
        {
          assert(pg && "make_void_returns_explicit_in_prog");
          proc_list * pl = pg->pl;
          while(pl){
            proc * p = pl->p;
            if(p){
              stmt_list * sl = p->sl;
              if(sl){
                stmt_list * tl = sl->t;
                stmt * t = tl->s;
                assert(t && "make_void_returns_explicit");
                if(t->op != AST_RETURN){
                  if(p->r != 0){
                    assert(0 && "procedure with non-void return, has a path that does not end in a return");
                  }else{
                    stmt * s = make_return_stmt(NULL);
                    stmt_list * nsl = make_stmt_list_item(s);
                    tl->n = nsl;
                    tl->t = sl->t = nsl;
                  }
                }
              }
            }
            pl = pl->n;
          }
        }

      }


      BddContext * dump_pds_from_prog(wpds::WPDS * pds, prog * pg)
      {
        ProgramBddContext * con = new ProgramBddContext(20*MILLION, 20 * MILLION); 

        map<string, int> vars;

        const str_list * vl = pg->vl;
        while(vl){
          stringstream ss;
          ss << "::" << vl->v;
          //con->addBoolVar(ss.str());
          vars[ss.str()] = 2;
          vl = vl->n;
        }
        proc_list * pl = pg->pl;
        while(pl){
          vl = pl->p->al;
          while(vl){
            stringstream ss;
            ss << pl->p->f << "::" << vl->v;
            //con->addBoolVar(ss.str());
            vars[ss.str()] = 2;
            vl = vl->n;
          }
          vl = pl->p->vl;
          while(vl){
            stringstream ss;
            ss << pl->p->f << "::" << vl->v;
            //con->addBoolVar(ss.str());
            vars[ss.str()] = 2;
            vl = vl->n;
          }
          pl = pl->n;
        }
        fprintf(stderr, "Entering setIntVars\n");
        con->setIntVars(vars);
        fprintf(stderr, "Done with setIntVars\n");

        str_stmt_ptr_hash_map label_to_stmt;
        str_proc_ptr_hash_map name_to_proc;
        stmt_ptr_stmt_list_ptr_hash_map goto_to_targets;
        stmt_ptr_proc_ptr_hash_map call_to_callee;

        map_name_to_proc(name_to_proc, pg);

        pl = pg->pl;
        while(pl){
          proc * p = pl->p;
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
        fprintf(stderr, "Done converting\n");

        return con;
      }


      

      static bdd expr_as_bdd(const expr * e, ProgramBddContext *  con, const char * f)
      {
        if(!e)
          assert(0 && "expr_as_bdd");
        bdd l = bddfalse, r = bddfalse;
        stringstream ss;
        if(e->l)
          l = expr_as_bdd(e->l, con, f);
        if(e->r)
          r = expr_as_bdd(e->r, con, f);
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
              stringstream ss2;
              ss2 << "::" << e->v;
              assert(con->find(ss2.str()) != con->end());
              return con->From(ss2.str());
            }
          case AST_VAR_POST:
            assert(0 && "expr_as_bdd: AST_VAR_POST should not occur in expr_as_bdd");
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
        stringstream ss;
        ss << (long) s;
        str_list * ll = s->ll;
        while(ll){
          if(ll->v)
            ss << "::" << ll->v;
          ll = ll->n;
        }
        return getKey(ss.str());
      };

      void dump_pds_from_stmt(WPDS * pds, stmt * s, ProgramBddContext * con, const stmt_ptr_stmt_list_ptr_hash_map& goto_to_targets, const
          stmt_ptr_proc_ptr_hash_map& call_to_callee, const char * f, stmt * ns)
      {
        binrel_t temp = new BinRel(con, con->True());
        binrel_t one = dynamic_cast<BinRel*>(temp->one().get_ptr());
        bdd b, b1;
        str_list * vl;
        expr_list * el;
        stmt_list * sl;
        stmt_ptr_stmt_list_ptr_hash_map::const_iterator goto_iter;
        stmt_ptr_proc_ptr_hash_map::const_iterator callee_iter;
        string lhs;

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
              sl = sl->n;
            }
            break;
          case AST_RETURN:
            b = bddtrue;
            // havoc all local variables because there is no merge function yet.
            for(ProgramBddContext::const_iterator cit = con->begin(); cit != con->end(); ++cit){
              if(cit->first.size() > 0 && cit->first.at(0) == ':'){
                string st = string(cit->first);
                b = b & con->Assign(st, con->From(st));            
              }
            }
            pds->add_rule(stt(), stk(s), stt(), new BinRel(con, b));
            break;
          case AST_ASSIGN:
            assert(s->vl && s->el);
            b = bddtrue;
            vl = s->vl;
            el = s->el;
            while(vl || el){
              if(!vl || !el)
                assert(0 && "[dump_pds_from_stmt] Assignment should have the same number of lhs/rhs");
              stringstream ss;
              ss << f << "::" << vl->v;
              if(con->find(ss.str()) != con->end()){
                lhs = ss.str();
              }else{
                stringstream ss2;
                ss2 << "::" << vl->v;           
                lhs = string(ss2.str());
              }
              b = b & con->Assign(lhs, expr_as_bdd(el->e, con, f));
              vl = vl->n;
              el = el->n;
            }
            if(s->e)
              b = b & xformer_for_constrain(s->e, con, f);
            pds->add_rule(stt(), stk(s), stt(), stk(ns), new BinRel(con, b));
            break;       
          case AST_ITE:   
            assert(s->e && s->sl1);
            if(s->sl1){
              assert(s->sl1->s);
              b = con->Assume(expr_as_bdd(s->e, con, f), con->True());
              pds->add_rule(stt(), stk(s), stt(), stk(s->sl1->s), new BinRel(con, b));
            }
            if(s->sl2){
              assert(s->sl2->s);
              b = con->Assume(expr_as_bdd(s->e, con, f), con->False());
              pds->add_rule(stt(), stk(s), stt(), stk(s->sl2->s), new BinRel(con, b));
            }else{
              // fall through edge.
              b = con->Assume(expr_as_bdd(s->e, con, f), con->False());
              pds->add_rule(stt(), stk(s), stt(), stk(ns), new BinRel(con, b));
            }
            if(s->sl1)
              dump_pds_from_stmt_list(pds, s->sl1, con, goto_to_targets, call_to_callee, f, ns);
            if(s->sl2)
              dump_pds_from_stmt_list(pds, s->sl2, con, goto_to_targets, call_to_callee, f, ns);
            break;
          case AST_WHILE:
            assert(s->e && s->sl1);
            b = con->Assume(expr_as_bdd(s->e, con, f), con->True());
            pds->add_rule(stt(), stk(s), stt(), stk(s->sl1->s), new BinRel(con, b));
            dump_pds_from_stmt_list(pds, s->sl1, con, goto_to_targets, call_to_callee, f, s);
            b = con->Assume(expr_as_bdd(s->e, con, f), con->False());
            pds->add_rule(stt(), stk(s), stt(), stk(ns), new BinRel(con, b));
            break;       
          case AST_ASSERT:
            assert(0 && "assert statements can't be dumped to PDS");
            //WARNING: Asserts are treated as assumes.
            //write a prepass to handle asserts.
            /*
            assert(s->e);
            b = con->Assume(expr_as_bdd(s->e, con, f), con->True());
            pds->add_rule(stt(), stk(s), stt(), stk(ns), new BinRel(con, b));
            */
            break;
          case AST_ASSUME:
            assert(s->e);
            b = con->Assume(expr_as_bdd(s->e, con, f), con->True());
            pds->add_rule(stt(), stk(s), stt(), stk(ns), new BinRel(con, b));
            break;
          case AST_CALL:
            callee_iter = call_to_callee.find(s);
            assert(callee_iter != call_to_callee.end());
            assert(callee_iter->second->sl);
            assert(callee_iter->second->sl->s);
            pds->add_rule(stt(), stk(s), stt(), stk(callee_iter->second->sl->s), stk(ns), one);
            break;
        }
      }

      void dump_pds_from_stmt_list(WPDS * pds, stmt_list * sl, ProgramBddContext * con, const stmt_ptr_stmt_list_ptr_hash_map&
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
      void dump_pds_from_proc(WPDS * pds, proc * p, ProgramBddContext * con, const stmt_ptr_stmt_list_ptr_hash_map& goto_to_targets, const stmt_ptr_proc_ptr_hash_map& call_to_callee)
      {
        dump_pds_from_stmt_list(pds, p->sl, con, goto_to_targets, call_to_callee, p->f, NULL);
      }


      static unsigned loc(stmt_list * sl)
      {
        unsigned sc = 0;
        while(sl)
        {
          sc++;
          if(sl->s->sl1)
            sc += loc(sl->s->sl1);
          if(sl->s->sl2)
            sc += loc(sl->s->sl2);
          sl = sl->n;
        }
        return sc;
      }

    } //namespace details
  
    BddContext * read_prog(WPDS * pds, const char * fname, bool dbg)
    {
      FILE * fin;
      fin = fopen(fname, "r");
      parse(fin);
      prog * pg = parsing_result;
      parsing_result = NULL;
      if(dbg)
        emit_prog(stdout, pg);
      BddContext * con = dump_pds_from_prog(pds, pg);
      deep_erase_prog(&pg);
      return con;
    }

    prog * parse_prog(const char * fname)
    {
      FILE * fin;
      fin = fopen(fname, "r");
      if(parse(fin))
        assert(0 && "Parsing Error");
      prog * pg =  parsing_result;
      parsing_result = NULL;
      return pg;
    }

    BddContext * pds_from_prog(wpds::WPDS * pds, prog * pg)
    {
      assert(pg);
      BddContext * con = dump_pds_from_prog(pds, pg);
      return con;
    }
    /*
    WPDS * wpds_from_prog(prog * pg)
    {
      assert(pg);
      WPDS * pds = new WPDS;
      dump_pds_from_prog(pds, pg);
      return pds;
    }

    FWPDS * fwpds_from_prog(prog * pg)
    {
      FWPDS * pds = new FWPDS;
      dump_pds_from_prog(pds, pg);
      return pds;
    }
*/
    void print_prog_stats(prog * pg)
    {
      assert(pg);

      // Number of variables
      unsigned pc = 0, vc = 0, gvc = 0;
      str_list * vl;
      proc_list * pl;

      pl = pg->pl;
      while(pl)
      {
        proc * p = pl->p;
        unsigned lvc = 0, sc = 0;
        pc++;
        fprintf(stdout, "Procedure: %s\n", p->f);
        vl = p->vl;
        while(vl){
          lvc++;
          vl = vl->n;
        }
        vc += lvc;
        sc = loc(p->sl);
        fprintf(stdout, "#local vars: \t\t%u\n", lvc);
        fprintf(stdout, "#loc: \t\t%u", sc);
        fprintf(stdout, "\n");            
        pl = pl->n;
      }

      vl = pg->vl;
      while(vl)
      {
        gvc++;
        vl = vl->n;
      }

      fprintf(stdout, "Total Stats:\n");
      fprintf(stdout, "#procs: \t\t%u\n", pc);
      fprintf(stdout, "#global vars: \t\t%u\n", gvc);
      fprintf(stdout, "#local vars: \t\t%u\n", vc);
      fprintf(stdout, "#total vars: \t\t\t%u\n", vc + gvc);
      fprintf(stdout, "\n");      
    }


    void instrument_asserts(prog * pg, const char * errLbl)
    {
      instrument_asserts_prog(pg, errLbl);
    }


    void make_void_returns_explicit(prog * pg)
    {
      make_void_returns_explicit_in_prog(pg);  
    }

    Key getEntryStk(const prog * pg, const char * f)
    {
      assert(pg);
      const proc_list * pl = pg->pl;
      while(pl)
      {
        if(strcmp(pl->p->f, f) == 0){
          assert(pl->p->sl);
          return stk(pl->p->sl->s);
        }
        pl = pl->n;
      }
      assert(0 && "[getEntryStk] Procedure not found");
    }
    
    Key getPdsState() 
    {
      return stt();
    }

    Key getErrStk(const prog * pg)
    {
      assert(pg && pg->e && "error label not set yet. Did you use instrument_assert?");
      return stk(pg->e);
    }
  }
}
