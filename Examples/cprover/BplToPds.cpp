#include "BplToPds.hpp"

#include <string>
#include <sstream>
#include <map>
#include "wali/domains/binrel/BinRelMergeFns.hpp"
#include "../../../svn-repository/NWAOBDDRelMergeFns.hpp"
#include <boost/cast.hpp>
#include "wali/wpds/ewpds/EWPDS.hpp"
#include "../turetsky/svn-repository/nwaobdd.h"

using namespace std;
using namespace wali;
using namespace wali::wpds;
using namespace wali::wpds::ewpds;
using namespace wali::wpds::fwpds;
using namespace wali::domains::binrel;
using namespace wali::domains::nwaobddrel;
using namespace wali::domains;
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
            return bdd_or(l, bdd_and(bdd_not(r), bddtrue));
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

	namespace nwaobddrel
	{
		/*
		* Create the NWA_OBDD for a contrain statement e with the given context
		*/
		static NWA_OBDD::NWAOBDD xformer_for_constrain(const expr * e, NWAOBDDContext * con, const char * f) {
		{
        int varInfo;
        string s;
        if(!e)
          assert(0 && "xformer_for_constrain");
		//Our left and right sides of the expression start as falase NWA_OBDDs
		NWA_OBDD::NWAOBDD l = NWA_OBDD::MkFalse(), r = NWA_OBDD::MkFalse();
        stringstream ss;
		//Get the left and right sides of the expression as NWA_OBDDs
        if(e->l)
          l = xformer_for_constrain(e->l, con, f);
        if(e->r)
          r = xformer_for_constrain(e->r, con, f);
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
          case AST_SCHOOSE:
			  return con->Or(l, con->And(con->Not(r), NWA_OBDD::MkTrue()));
          case AST_VAR:
		  case AST_VAR_POST:
            ss << f << "::" << e->v;
            s = ss.str();
            if(con->varMap.find(ss.str()) == con->varMap.end()){
              stringstream ss2;
              ss2 << "::" << e->v;
			  assert(con->varMap.find(ss2.str()) != con->varMap.end());
              s = ss2.str();
            }
			varInfo = con->varMap.find(s)->second;
            if(e->op == AST_VAR)
              return con->NWAOBDDTCreate(varInfo,BASE_PRE);
            else // e->op == AST_VAR_POST
				return con->NWAOBDDTCreate(varInfo, BASE_POST);
          case AST_NONDET:
            return NWA_OBDD::MkTrue();
          case AST_CONSTANT:
            if(e->c == ONE)
				return NWA_OBDD::MkTrue();
            else
              return NWA_OBDD::MkFalse();
          default:
            assert(0 && "expr_as_nwaobdd: Unknown case");
        }
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

		//Match the labels in the boolean program with the statements they point for use in the later goto statements
		//Populate the map with the ll "label" values
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

		//Maps names to the procedure locations for use in call statements
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

		//For each call - find the location of the entry statement of the procdure they call to
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

		//Change the asserts to assumes + gotos - and create error labels
		//This is where the error statement is created
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
          // Don't loop forever.
          // stmt * s = make_skip_stmt();
          // Loop Forever
          str_list * l = make_str_list_item(strdup(errLbl));
          stmt * s = make_goto_stmt(l);
          str_list * ll = make_str_list_item(strdup(errLbl));
          s->ll = ll;
          stmt_list * sl = make_stmt_list_item(s);

          // Update error label in program struct.
          pg->e = s;

          // Add a void return.
          s = make_return_stmt(NULL);
          sl = add_stmt_right(sl, s);
          proc * m = make_proc(0, strdup(errLbl), NULL, NULL, NULL, sl);
          pg->pl = add_proc_right(pg->pl, m);

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

		//If there is an assert, create and ite statement which goes to the error statement
		//if the ite is false, go to the error label
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


        //Create the void returns (procedures that have no returns) to have actual return operations used for rule creation
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
                  // Add a return with non-deterministic values. This is a safe approximation
                  expr_list * el = NULL;
                  if(p->r != 0){
                    int r = p->r;
                    el = make_expr_list_item(make_non_det_expr());
                    --r;
                    while(r>0){
                      el = add_expr_right(el, make_non_det_expr());
                      --r;
                    }
                  }
                  stmt * s = make_return_stmt(el);
                  add_stmt_right(sl, s);
                }
              }
            }
            pl = pl->n;
          }
        }


        // Forward declaration of static procedures to remove skip statements from the program.
        static void remove_skip_in_prog(prog * pg);
        static stmt_list * remove_skip_in_stmt_list(stmt_list * sl);

		//Remove skip statements from the program
        static void remove_skip_in_prog(prog * pg)
        {
          assert(pg && "remove_skip_in_prog");
          proc_list * pl = pg->pl;
          while(pl){
            if(pl->p->sl){
              pl->p->sl = remove_skip_in_stmt_list(pl->p->sl);
              if(pl->p->sl == NULL){
                // Don't empty a previously non-empty proc
                pl->p->sl = make_stmt_list_item(make_skip_stmt());
              }

            }

            pl = pl->n;
          }
        }

        // Also rewrite tail for the whole stmt_list. That's easier, trust me.
        static stmt_list * remove_skip_in_stmt_list(stmt_list *sl)
        {
          if(sl == NULL)
            return NULL;
          assert(sl && sl->s && "remove_skip_in_stmt_list");
          if(sl->s->sl1){
            sl->s->sl1 = remove_skip_in_stmt_list(sl->s->sl1);
            if(sl->s->sl1 == NULL){
              //Can't make it null, this will become an if statement w/o any stmt_list
              sl->s->sl1 = make_stmt_list_item(make_skip_stmt());
            }
          }
          if(sl->s->sl2)
            sl->s->sl2 = remove_skip_in_stmt_list(sl->s->sl2);

          stmt_list * cl = remove_skip_in_stmt_list(sl->n);
          if(sl->s->op == AST_SKIP){
            sl->n = NULL;
            deep_erase_stmt_list(&sl);
            return cl;
          }else{
            sl->n = cl;
            if(cl == NULL)
              sl->t = sl;
            else
              sl->t = cl->t;
            return sl;
          }
        }


        // Forward declaration of static procedures having to do with instrumenting enforce conditions.
        // The semantics of the enforce is to assume that the given condition is true at every point in the
        // given procedure, i.e., we begin by assuming that the condition holds at entry, and then assume 
        // the condition holds after every statement.
        static void instrument_enforce_in_prog(prog * pg);
        static void instrument_enforce_in_proc(proc * p);
        static void instrument_enforce_in_stmt_list(stmt_list * h, stmt_list * sl, expr const * e);

        static void instrument_enforce_in_prog(prog * pg)
        {
          assert(pg && "instrument_enforce_in_prog");
          proc_list * pl = pg->pl;
          while(pl){
            instrument_enforce_in_proc(pl->p);
            pl = pl->n;
          }
        }

        
		//Change enforce statements to assume rules
        static void instrument_enforce_in_proc(proc * p)
        {
          assert(p && "instrument_enforce_in_proc");
          assert(p->sl && "instrument_enforce_in_proc: Please call make_void_returns_explicit first");
          if(p->e){
            instrument_enforce_in_stmt_list(p->sl, p->sl, p->e);
            expr * e = make_deep_copy_expr(p->e);
            stmt * s = make_assume_stmt(e);
            p->sl = add_stmt_left(p->sl, s);
          }
        }

		//Change enforce statements to assume rules
        static void instrument_enforce_in_stmt_list(stmt_list * h, stmt_list * sl, expr const * e)
        {
          assert(sl && sl->s && "instrument_enforce_in_stmt_list");
          if(sl->s->sl1)
            instrument_enforce_in_stmt_list(sl->s->sl1, sl->s->sl1, e);
          if(sl->s->sl2)
            instrument_enforce_in_stmt_list(sl->s->sl2, sl->s->sl2, e);
          if(sl->n)
            instrument_enforce_in_stmt_list(h, sl->n, e);

          if(sl->s->op != AST_RETURN){
            expr * ec = make_deep_copy_expr(e);
            stmt * s = make_assume_stmt(ec);
            if(h->t != sl){
              stmt_list * nsl = make_stmt_list_item(s);
              nsl->n = sl->n;
              sl->n = nsl;
            }else{
              add_stmt_right(h,s);
            }
          }
        }
      
        // Forward declaration of static procedure having to do with
        // instrumenting calls to handle return values correctly.
        static void instrument_call_return_in_prog(prog * pg);
        static void instrument_call_result_in_stmt_list(stmt_list * h, stmt_list * sl);
        static void instrument_call_args_in_stmt_list(stmt_list * sl);
        static void instrument_return_in_stmt_list(stmt_list * sl);
        // Helper functions
        static char * getIthRetVarName(unsigned i)        
        {
          stringstream ss;
          ss << "__call_return_reserved_var_" << i << "__";
          return strdup(ss.str().c_str());
        }

        static unsigned count_max_returns_from_any_proc(prog const * pg)
        {
          if(!pg) return 0;
        }

        static stmt * reserved_vars_from_exprs(expr_list const * el)
        {
          assert(el != NULL);
          int i = 0;
          expr_list * fl = make_expr_list_item(make_deep_copy_expr(el->e));
          str_list * tl = make_str_list_item(getIthRetVarName(i));
          el = el->n;
          ++i;
          while(el){
            add_expr_right(fl, make_deep_copy_expr(el->e));
            add_str_right(tl, getIthRetVarName(i));
            ++i;
            el = el->n;
          }
          return make_assign_stmt(tl, fl, NULL);          
        }

        static stmt * vars_from_reserved_vars(str_list const * vl)
        {
          assert(vl);
          int i = 0;
          str_list * tl = make_str_list_item(strdup(vl->v));
          expr_list * fl = make_expr_list_item(make_var_expr(getIthRetVarName(i)));
          vl = vl->n;
          ++i;
          while(vl){
            add_str_right(tl, strdup(vl->v));
            add_expr_right(fl, make_var_expr(getIthRetVarName(i)));
            ++i;     
            vl = vl->n;
          }
          return make_assign_stmt(tl, fl, NULL);
        }

	void splice_nongoto_into_stmt_list(stmt_list** prev, stmt_list* new_sl, stmt_list* to_come_after)
	{
	  new_sl->n = to_come_after;
	  *prev = new_sl;
	  new_sl->s->ll = to_come_after->s->ll;
	  to_come_after->s->ll = NULL;
	}

        static void instrument_call_return_in_prog(prog * pg)
        {
          // Add extra global variables to return values from functions
          unsigned maxReservedVals = 0;
          proc_list * pl = pg->pl;
          while(pl)
          {
            maxReservedVals = pl->p->r > maxReservedVals ? pl->p->r : maxReservedVals;

            str_list const * al = pl->p->al;
            unsigned numArgs = 0;
            while(al){
              ++numArgs;
              al = al->n;
            }
            maxReservedVals = numArgs > maxReservedVals ? numArgs : maxReservedVals;

            pl = pl->n;
          }

          for(unsigned i=0; i < maxReservedVals; ++i)
            if(pg->vl)
              pg->vl = add_str_right(pg->vl, getIthRetVarName(i));
            else
              pg->vl = make_str_list_item(getIthRetVarName(i));
          
          ///

          // Now instrument calls and returns
          pl = pg->pl;
          while(pl){
            // Instrumentation to copy values back from reserved globals
            instrument_call_result_in_stmt_list(pl->p->sl, pl->p->sl);

            // Instrumentation to copy values to reserved globals at return
            instrument_return_in_stmt_list(pl->p->sl);
            stmt_list * sl = pl->p->sl;
            if(sl && sl->s->op == AST_RETURN && sl->s->el != NULL){
              stmt * ns = reserved_vars_from_exprs(sl->s->el);
              stmt_list * asl = make_stmt_list_item(ns);
              asl->t = sl->t;
	      splice_nongoto_into_stmt_list(&(pl->p->sl),asl,sl);
            }

            // Instrumentation to copy values to reserved globals at call
            instrument_call_args_in_stmt_list(pl->p->sl);
            sl = pl->p->sl;
            if(sl && sl->s->op == AST_CALL && sl->s->el != NULL){
              stmt * ns = reserved_vars_from_exprs(sl->s->el);
              stmt_list * asl = make_stmt_list_item(ns);
              asl->t = sl->t;
	      splice_nongoto_into_stmt_list(&(pl->p->sl),asl,sl);
            }

            // Instrumentation at the head to copy values back from reserved globals
            if(pl->p->al){
              stmt * ns = vars_from_reserved_vars(pl->p->al);
              pl->p->sl = add_stmt_left(pl->p->sl, ns);
            } 

            pl = pl->n;
          }
        }

        static void instrument_call_result_in_stmt_list(stmt_list * h, stmt_list * sl)
        {
          if(!sl)
            return;
          stmt * s = sl->s;
          assert(s && "instrument_call_result_in_stmt_list");
          if(s->sl1)
            instrument_call_result_in_stmt_list(s->sl1, s->sl1);
          if(s->sl2)
            instrument_call_result_in_stmt_list(s->sl2, s->sl2);
          if(sl->n)
            instrument_call_result_in_stmt_list(h, sl->n);

          if(s->op == AST_CALL){
            if(s->vl){
              stmt * ns = vars_from_reserved_vars(s->vl);
              if(h->t != sl){
                stmt_list * nsl = make_stmt_list_item(ns);
		splice_nongoto_into_stmt_list(&(sl->n),nsl,sl->n);
              }else{
                add_stmt_right(h,ns);
              }
            }
          }
        }


        static void instrument_return_in_stmt_list(stmt_list * sl)
        {
          if(!sl || !sl->n)
            return;
          stmt * s = sl->s;
          if(s->sl1){
            instrument_return_in_stmt_list(s->sl1);
            if(s->sl1->s->op == AST_RETURN && s->sl1->s->el != NULL){
              stmt * ns = reserved_vars_from_exprs(s->sl1->s->el);
              stmt_list * asl = make_stmt_list_item(ns);
              asl->t = s->sl1->t;
	      splice_nongoto_into_stmt_list(&(s->sl1),asl,s->sl1);
            }
          }
          if(s->sl2){
            instrument_return_in_stmt_list(s->sl2);
            if(s->sl2->s->op == AST_RETURN && s->sl2->s->el != NULL){
              stmt * ns = reserved_vars_from_exprs(s->sl2->s->el);
              stmt_list * asl = make_stmt_list_item(ns);
              asl->t = s->sl1->t;
              splice_nongoto_into_stmt_list(&(s->sl2),asl,s->sl2);
            }
          }
          if(!sl->n)
            return;
          
          instrument_return_in_stmt_list(sl->n);

          stmt_list * nsl = sl->n;
          s = nsl->s;
          assert(s && "instrument_call_return_in_stmt_list");
          if(s->op == AST_RETURN && s->el != NULL){
            stmt * ns = reserved_vars_from_exprs(s->el);
            stmt_list * asl = make_stmt_list_item(ns);
            splice_nongoto_into_stmt_list(&(sl->n),asl,(sl->n));
          }
        }
      
        static void instrument_call_args_in_stmt_list(stmt_list * sl)
        {
          if(!sl || !sl->n)
            return;
          stmt * s = sl->s;
          if(s->sl1){
            instrument_call_args_in_stmt_list(s->sl1);
            if(s->sl1->s->op == AST_CALL && s->sl1->s->el != NULL){
              stmt * ns = reserved_vars_from_exprs(s->sl1->s->el);
              stmt_list * asl = make_stmt_list_item(ns);
              asl->t = s->sl1->t;
              splice_nongoto_into_stmt_list(&(s->sl1),asl,s->sl1);
	    }
          }
          if(s->sl2){
            instrument_call_args_in_stmt_list(s->sl2);
            if(s->sl2->s->op == AST_CALL && s->sl2->s->el != NULL){
              stmt * ns = reserved_vars_from_exprs(s->sl2->s->el);
              stmt_list * asl = make_stmt_list_item(ns);
              asl->t = s->sl1->t;
	      splice_nongoto_into_stmt_list(&(s->sl2),asl,s->sl2);
            }
          }
          if(!sl->n)
            return;
          
          instrument_call_args_in_stmt_list(sl->n);

          stmt_list * nsl = sl->n;
          s = nsl->s;
          assert(s && "instrument_call_args_in_stmt_list");
          if(s->op == AST_CALL && s->el != NULL){
            stmt * ns = reserved_vars_from_exprs(s->el);
            stmt_list * asl = make_stmt_list_item(ns);
	    splice_nongoto_into_stmt_list(&(sl->n),asl,sl->n);
          }
        }
      } // namespce resolve_details


      BddContext * dump_pds_from_prog(wpds::WPDS * pds, prog * pg)
      {
        ProgramBddContext * con = new ProgramBddContext(10*MILLION, 1*MILLION); 
        //ProgramBddContext * con = new ProgramBddContext(); 

        map<string, int> vars;

	//Iterate through the global variables and give them unique names
        const str_list * vl = pg->vl;
        while(vl){
          stringstream ss;
          ss << "::" << vl->v;
          vars[ss.str()] = 2; //A Boolean type
          vl = vl->n;
        }
	
	//For each procedure
        proc_list * pl = pg->pl;
        while(pl){
	  //Iterate through the arguments to the procedure and give them unique names
          vl = pl->p->al;
          while(vl){
            stringstream ss;
            ss << pl->p->f << "::" << vl->v;
            vars[ss.str()] = 2; //A Boolean type
            vl = vl->n;
          }
	  //Iterate through the local variables of the procedure and give them unique names
          vl = pl->p->vl;
          while(vl){
            stringstream ss;
            ss << pl->p->f << "::" << vl->v;
            //con->addBoolVar(ss.str());
            vars[ss.str()] = 2; //A Boolean type
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

        resolve_details::map_name_to_proc(name_to_proc, pg);

        pl = pg->pl;
        while(pl){
          proc * p = pl->p;
		  resolve_details::map_label_to_stmt(label_to_stmt, p);
		  resolve_details::map_goto_to_targets(goto_to_targets, label_to_stmt, p);
		  resolve_details::map_call_to_callee(call_to_callee, name_to_proc, p);

          dump_pds_from_proc(pds, p, con, goto_to_targets, call_to_callee); 

          label_to_stmt.clear();
          clear_stmt_ptr_stmt_list_ptr_hash_map(goto_to_targets);
          call_to_callee.clear();
          pl = pl->n;
        }
		
		cprover::havocLocals(pds, pg, con);
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
	    return con->Or(l, con->And(con->Not(r), con->NonDet()));
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


	  NWAOBDDContext * dump_pds_from_prog_nwa(wpds::WPDS * pds, prog * pg, bool first)
	  {

		  map<string, int> vars;

		  //Iterate through the global variables and give them unique names
		  const str_list * vl = pg->vl;
		  while (vl){
			  stringstream ss;
			  ss << "::" << vl->v;
			  vars[ss.str()] = 2; //A Boolean type
			  vl = vl->n;
		  }

		  //For each procedure
		  proc_list * pl = pg->pl;
		  while (pl){
			  //Iterate through the arguments to the procedure and give them unique names
			  vl = pl->p->al;
			  while (vl){
				  stringstream ss;
				  ss << pl->p->f << "::" << vl->v;
				  vars[ss.str()] = 2; //A Boolean type
				  vl = vl->n;
			  }
			  //Iterate through the local variables of the procedure and give them unique names
			  vl = pl->p->vl;
			  while (vl){
				  stringstream ss;
				  ss << pl->p->f << "::" << vl->v;
				  //con->addBoolVar(ss.str());
				  vars[ss.str()] = 2; //A Boolean type
				  vl = vl->n;
			  }
			  pl = pl->n;
		  }
		  fprintf(stderr, "Entering setIntVars\n");


		  //Determine the number of labels needed for the NWAOBDD
		  int numVars;
		  numVars = vars.size();
		  unsigned int numSpaces = 12 * numVars;
		  unsigned int minLevels = ceil(log2(numSpaces + 4) - 2);
		  if (minLevels % 2)
		  {
			  minLevels++;
		  }

		  if (first)
		  {
			  //Setup the NWAOBDD enviroment
			  //Initialize the Hash table and the NWAOBDDs used
			  //for this specific variable ordering
			  NWA_OBDD::setMaxLevel(minLevels);
			  NWA_OBDD::NWAOBDDNodeHandle::InitNoDistinctionTable();
			  NWA_OBDD::NWAOBDDNodeHandle::InitReduceCache();
			  NWA_OBDD::InitPairProductCache();
			  NWA_OBDD::InitPathSummaryCache();
			  NWA_OBDD::InitPairProductMapCaches();
			  InitModPathSummaryCache();
		  }
		  NWAOBDDContext * con = new NWAOBDDContext(BASE_1ST_TENSOR_ROOT);

		  //Populate the name to variable map
		  con->setIntVars(vars, minLevels);
		  fprintf(stderr, "Done with setIntVars\n");


			  str_stmt_ptr_hash_map label_to_stmt;
			  str_proc_ptr_hash_map name_to_proc;
			  stmt_ptr_stmt_list_ptr_hash_map goto_to_targets;
			  stmt_ptr_proc_ptr_hash_map call_to_callee;

			  resolve_details::map_name_to_proc(name_to_proc, pg);

			  //For each procedure in the program
			  pl = pg->pl;
			  while (pl){
				  proc * p = pl->p;
				  
				  //Run preprocessing on the procedure
				  resolve_details::map_label_to_stmt(label_to_stmt, p);
				  resolve_details::map_goto_to_targets(goto_to_targets, label_to_stmt, p);
				  resolve_details::map_call_to_callee(call_to_callee, name_to_proc, p);

				  //Create pds from the statement list of the procedure
				  dump_pds_from_proc(pds, p, con, goto_to_targets, call_to_callee);

				  //Clear the maps used
				  label_to_stmt.clear();
				  clear_stmt_ptr_stmt_list_ptr_hash_map(goto_to_targets);
				  call_to_callee.clear();
				  
				  //Get the next procedure
				  pl = pl->n;
			  }

			  //Havoc the locals of the NWA a
			  havocLocalsNWA(pds, pg, con);
			  name_to_proc.clear();
			  fprintf(stderr, "Done converting\n");

			  return con;
		  }


		  namespace nwaobddrel {


		  static NWA_OBDD::NWAOBDD expr_as_nwaobdd(const expr * e, NWAOBDDContext *  con, const char * f)
		  {
			  if (!e)
				  assert(0 && "expr_as_nwaobdd");
			  NWA_OBDD::NWAOBDD l = NWA_OBDD::MkFalse(), r = NWA_OBDD::MkFalse();
			  stringstream ss;
			  if (e->l){
				  l = expr_as_nwaobdd(e->l, con, f);
				  }
				  if (e->r){
					  r = expr_as_nwaobdd(e->r, con, f);
				  }
			  switch (e->op){
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
				  return NWA_OBDD::MkTrue();
			  case AST_SCHOOSE:
			  {
				  return con->Or(l, con->And(con->Not(r), NWA_OBDD::MkTrue()));
			  }
			  case AST_VAR:
				  ss << f << "::" << e->v;
				  if (con->varMap.find(ss.str()) != con->varMap.end()){
					  return con->From(ss.str());
				  }
				  else{
					  stringstream ss2;
					  ss2 << "::" << e->v;
					  assert(con->varMap.find(ss2.str()) != con->varMap.end());
					  return con->From(ss2.str());
				  }
			  case AST_VAR_POST:
				  assert(0 && "expr_as_nwaobdd: AST_VAR_POST should not occur in expr_as_nwaobdd");
			  case AST_CONSTANT:
				  if (e->c == ONE)
					  return con->True();
				  else
					  return con->False();
			  default:
				  assert(0 && "expr_as_nwaobdd: Unknown case");
			  }
		  }

	  }

      static wali::Key stt()
      {
        return getKey("Unique State Name");
      }

      static wali::Key stk(stmt const * s)
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

      static wali::Key stk(proc const * p)
      {
        stringstream ss;
        ss << p->f;
        return getKey(ss.str());
      }

      // XXX: This is a very very bad way of choosing the merge function.
      // There is a static global variable merge_type,
      // dump_pds_from_stmt will look at this variables to decide what kind of
      // merge function to dump.
      typedef enum {NO_MERGE, MEET_MERGE, TENSOR_MERGE, NEWTON_MERGE} MergeFn;
      static MergeFn merge_type = NO_MERGE;
      void set_merge_type(MergeFn m)
      {
        merge_type = m;
      }

      void dump_pds_from_stmt(WPDS * pds, stmt * s, ProgramBddContext * con, const stmt_ptr_stmt_list_ptr_hash_map& goto_to_targets, const
          stmt_ptr_proc_ptr_hash_map& call_to_callee, const char * f, stmt * ns)
      {
        binrel_t one = con->getBaseOne();
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
            if(merge_type == NO_MERGE){
              b = one->getBdd();
              // havoc all local variables because there is no merge function.
              stringstream ss;
              ss << f << "::";
              string str = ss.str();
              for(ProgramBddContext::const_iterator cit = con->begin(); cit != con->end(); ++cit)
                if(cit->first.find(str) != string::npos)
                  b = b | con->Assign(cit->first, con->NonDet()); 
            }else b = one->getBdd();
            pds->add_rule(stt(), stk(s), stt(), new BinRel(con, b));
            break;
          case AST_ASSIGN:
            assert(s->vl && s->el);
            b = con->BaseID();
            vl = s->vl;
            el = s->el;
            while(vl || el){
              if(!vl || !el)
                assert(0 && "[dump_pds_from_stmt] Assignment should have the same number of lhs/rhs");
              // Special case added after correspondance with Tom Ball.
              // Assignments of the type _ = <exp> are dummy assignments 
              // that we don't care about.
              if(strcmp(vl->v,"_") == 0){
                cout << "Skipped assignment to dummy variable _" << endl;
                vl = vl->n;
                el = el->n;
                continue;
              }
              stringstream ss;
              ss << f << "::" << vl->v;
	      if(con->find(ss.str()) != con->end()){
                lhs = ss.str();
              }else{
                stringstream ss2;
                ss2 << "::" << vl->v;           
                lhs = string(ss2.str());
              }
              if(con->find(lhs) == con->end()){
                cout << "Unknown variable: [" << vl->v << "]" << endl;
                assert(0);
              }
              b = con->HavocVar(lhs, b);
              vl = vl->n;
              el = el->n;
            }
            vl = s->vl;
            el = s->el;
            b1 = bddtrue;
            while(vl || el){ //Most of the checks for errors were done in the previous loop, no need to do it again.
              if(strcmp(vl->v,"_") == 0){
                cout << "Skipped assignment to dummy variable _" << endl;
                vl = vl->n;
                el = el->n;
                continue;
              }
              stringstream ss;
              ss << f << "::" << vl->v;
              if(con->find(ss.str()) != con->end()){
                lhs = ss.str();
              }else{
                stringstream ss2;
                ss2 << "::" << vl->v;
                lhs = string(ss2.str());
              }
              b1 = b1 & con->AssignGen(lhs, expr_as_bdd(el->e, con, f), b);
              vl = vl->n;
              el = el->n;
            }
            if(s->e)
              b1 = b1 & xformer_for_constrain(s->e, con, f);
            pds->add_rule(stt(), stk(s), stt(), stk(ns), new BinRel(con, b1));
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
            assert(0 && "assert statements can't be dumped to PDS. Use instrument_asserts");
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
            // Make a call rule to the callee.
            // The stack symbol for callee is derived from the proc itself (not the first statement in the proc)
			switch (merge_type){
			case NEWTON_MERGE:
			{
				vector<string> lvars;
				vector<string> lvars2;
				stringstream ss;
				ss << s->f << "::";
				stringstream st;
				st << f << "::";
				string str = ss.str();
				string str2 = st.str();

				for (ProgramBddContext::const_iterator cit = con->begin(); cit != con->end(); ++cit)
				{
					if (cit->first.find(str) != string::npos)
					{
						lvars.push_back(cit->first);
					}
					if (cit->first.find(str2) != string::npos)
					{
						lvars2.push_back(cit->first);
					}
				}
				con->addVarList(std::pair<int, int>(stk(s), stk(callee_iter->second)), lvars, lvars2);

				//std::cout << "stk: " << stk(s) << " tgt " << stk(callee_iter->second) << std::endl;
			}
			case NO_MERGE:
				pds->add_rule(stt(), stk(s), stt(), stk(callee_iter->second), stk(ns), one);
				break;
              case MEET_MERGE:
              case TENSOR_MERGE:
                {
                  vector<string> lvars;
				  vector<string> lvars2;
                  stringstream ss;
                  ss << s->f << "::";
				  stringstream st;
				  st << f << "::";
                  string str = ss.str();
				  string str2 = st.str();
                  for(ProgramBddContext::const_iterator cit = con->begin(); cit != con->end(); ++cit)
				  {
                    if(cit->first.find(str) != string::npos)
                      lvars.push_back(cit->first);
					if(cit->first.find(str2) != string::npos)
					  lvars2.push_back(cit->first);
                  }
				  merge_fn_t merge;
                  if(merge_type == MEET_MERGE)
                    merge = new MeetMergeFn(con, lvars, lvars2);
                  else
                    merge = new TensorMergeFn(con, lvars, lvars2);
                  boost::polymorphic_cast<EWPDS*>(pds)->add_rule(stt(), stk(s), stt(), stk(callee_iter->second), stk(ns), one, merge);
                }
                break;
              default:
                cerr << "Unknown merge_type\n";
                assert(0);
            }
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
        // Create a lead-in edge from the proc itself to the first statment.
        pds->add_rule(stt(), stk(p), stt(), stk(p->sl->s), con->getBaseOne());
        dump_pds_from_stmt_list(pds, p->sl, con, goto_to_targets, call_to_callee, p->f, NULL);
      }

	  
		  void dump_pds_from_proc(WPDS * pds, proc * p, NWAOBDDContext * con, const stmt_ptr_stmt_list_ptr_hash_map& goto_to_targets, const stmt_ptr_proc_ptr_hash_map& call_to_callee)
		  {
			  // Create a lead-in edge from the proc itself to the first statment.
			  pds->add_rule(stt(), stk(p), stt(), stk(p->sl->s), con->getBaseOne());
			  dump_pds_from_stmt_list(pds, p->sl, con, goto_to_targets, call_to_callee, p->f, NULL);
		  }

		  void dump_pds_from_stmt(WPDS * pds, stmt * s, NWAOBDDContext * con, const stmt_ptr_stmt_list_ptr_hash_map& goto_to_targets, const
			  stmt_ptr_proc_ptr_hash_map& call_to_callee, const char * f, stmt * ns)
		  {
			  nwaobdd_t one = con->getBaseOne();
			  nwaobdd_t testB;
			  NWA_OBDD::NWAOBDD b, b1, Tester, Tester2, Tester3, Tester4, bT1, bT2, bT3, nT1, nT2, nT3, nT4, nT5, nT6, nT7, nT8, vT10, vT11, vT9, vT10Pr, vT10Ps, vT9Pr, vT9Ps, vT11Pr, vT11Ps, vT4Pr, vT4N, vT7Pr, vT7PrN, vT7Ps, vT7PsN, vT8Pr, vT8PrN, vT8PsN, vT8Ps;
			  str_list * vl;
			  expr_list * el;
			  stmt_list * sl;
			  stmt_ptr_stmt_list_ptr_hash_map::const_iterator goto_iter;
			  stmt_ptr_proc_ptr_hash_map::const_iterator callee_iter;
			  string lhs;
			  int rnd;

			  if (!s)
				  assert(0 && "dump_pds_from_stmt");
			  switch (s->op){
			  case AST_SKIP:
				  pds->add_rule(stt(), stk(s), stt(), stk(ns), one);
				  break;
			  case AST_GOTO:
				  goto_iter = goto_to_targets.find(s);
				  assert(goto_iter != goto_to_targets.end());
				  sl = goto_iter->second;
				  while (sl){
					  pds->add_rule(stt(), stk(s), stt(), stk(sl->s), one);
					  sl = sl->n;
				  }
				  break;
			  case AST_RETURN:
				  if (merge_type == NO_MERGE){
					  b = one->getNWAOBDD();
					  // havoc all local variables because there is no merge function.
					  stringstream ss;
					  ss << f << "::";
					  string str = ss.str();
					  for (std::map<string, int>::const_iterator cit = con->varMap.begin(); cit != con->varMap.end(); ++cit)
						  if (cit->first.find(str) != string::npos)
							  b = con->HavocVar(cit->first, b);
				  }
				  else b = one->getNWAOBDD();
				  pds->add_rule(stt(), stk(s), stt(), new NWAOBDDRel(con, b));
				  break;
			  case AST_ASSIGN:
				  assert(s->vl && s->el);
				  b = con->BaseID();
				  vl = s->vl;
				  el = s->el;
				  while (vl || el){
					  if (!vl || !el)
						  assert(0 && "[dump_pds_from_stmt] Assignment should have the same number of lhs/rhs");
					  // Special case added after correspondance with Tom Ball.
					  // Assignments of the type _ = <exp> are dummy assignments 
					  // that we don't care about.
					  if (strcmp(vl->v, "_") == 0){
						  cout << "Skipped assignment to dummy variable _" << endl;
						  vl = vl->n;
						  el = el->n;
						  continue;
					  }
					  stringstream ss;
					  ss << f << "::" << vl->v;
					  if (con->varMap.find(ss.str()) != con->varMap.end()){
						  lhs = ss.str();
					  }
					  else{
						  stringstream ss2;
						  ss2 << "::" << vl->v;
						  lhs = string(ss2.str());
					  }
					  if (con->varMap.find(lhs) == con->varMap.end()){
						  cout << "Unknown variable: [" << vl->v << "]" << endl;
						  assert(0);
					  }
					  b = con->HavocVar(lhs, b);
					  vl = vl->n;
					  el = el->n;
				  }
				  rnd = 0;
				  vl = s->vl;
				  el = s->el;
				  b1 = NWA_OBDD::MkTrue();
				  while (vl || el){ //Most of the checks for errors were done in the previous loop, no need to do it again.
					  if (strcmp(vl->v, "_") == 0){
						  cout << "Skipped assignment to dummy variable _" << endl;
						  vl = vl->n;
						  el = el->n;
						  continue;
					  }
					  stringstream ss;
					  ss << f << "::" << vl->v;
					  if (con->varMap.find(ss.str()) != con->varMap.end()){
						  lhs = ss.str();
					  }
					  else{
						  stringstream ss2;
						  ss2 << "::" << vl->v;
						  lhs = string(ss2.str());
					  }
					  b1 = NWA_OBDD::MkAnd(b1, con->AssignGen(lhs,nwaobddrel::expr_as_nwaobdd(el->e, con, f), b));
					  vl = vl->n;
					  el = el->n;
				  }
				  if (s->e)
					  b1 = NWA_OBDD::MkAnd(b1,xformer_for_constrain(s->e, con, f));
				  pds->add_rule(stt(), stk(s), stt(), stk(ns), new NWAOBDDRel(con, b1));
				  break;
			  case AST_ITE:
				  assert(s->e && s->sl1);
				  if (s->sl1){
					  assert(s->sl1->s);
					  //Create a rule that makes s->e true in pre and post
					  b = con->Assume(nwaobddrel::expr_as_nwaobdd(s->e, con, f), con->True());
					  pds->add_rule(stt(), stk(s), stt(), stk(s->sl1->s), new NWAOBDDRel(con, b));
				  }
				  if (s->sl2){
					  //Create a rule that makes s->e false in pre and post
					  assert(s->sl2->s);
					  b = con->Assume(nwaobddrel::expr_as_nwaobdd(s->e, con, f), con->False());
					  pds->add_rule(stt(), stk(s), stt(), stk(s->sl2->s), new NWAOBDDRel(con, b));
				  }
				  else{
					  // fall through edge.
					  b = con->Assume(nwaobddrel::expr_as_nwaobdd(s->e, con, f), con->False());
					  pds->add_rule(stt(), stk(s), stt(), stk(ns), new NWAOBDDRel(con, b));
				  }
				  if (s->sl1)
					  dump_pds_from_stmt_list(pds, s->sl1, con, goto_to_targets, call_to_callee, f, ns);
				  if (s->sl2)
					  dump_pds_from_stmt_list(pds, s->sl2, con, goto_to_targets, call_to_callee, f, ns);
				  break;
			  case AST_WHILE:
				  assert(s->e && s->sl1);
				  b = con->Assume(nwaobddrel::expr_as_nwaobdd(s->e, con, f), con->True());
				  pds->add_rule(stt(), stk(s), stt(), stk(s->sl1->s), new NWAOBDDRel(con, b));
				  dump_pds_from_stmt_list(pds, s->sl1, con, goto_to_targets, call_to_callee, f, s);
				  b = con->Assume(nwaobddrel::expr_as_nwaobdd(s->e, con, f), con->False());
				  pds->add_rule(stt(), stk(s), stt(), stk(ns), new NWAOBDDRel(con, b));
				  break;
			  case AST_ASSERT:
				  assert(0 && "assert statements can't be dumped to PDS. Use instrument_asserts");
				  break;
			  case AST_ASSUME:
				  assert(s->e);
				  b = con->Assume(nwaobddrel::expr_as_nwaobdd(s->e, con, f), con->True());
				  pds->add_rule(stt(), stk(s), stt(), stk(ns), new NWAOBDDRel(con, b));
				  break;
			  case AST_CALL:
				  callee_iter = call_to_callee.find(s);
				  assert(callee_iter != call_to_callee.end());
				  assert(callee_iter->second->sl);
				  assert(callee_iter->second->sl->s);
				  // Make a call rule to the callee.
				  // The stack symbol for callee is derived from the proc itself (not the first statement in the proc)
				  switch (merge_type){
				  case NEWTON_MERGE:
				  {
					  vector<string> lvars;
					  vector<string> lvars2;
					  stringstream ss;
					  ss << s->f << "::";
					  stringstream st;
					  st << f << "::";
					  string str = ss.str();
					  string str2 = st.str();

					  for (std::map<string, int>::const_iterator cit = con->varMap.begin(); cit != con->varMap.end(); ++cit)
					  {
						  if (cit->first.find(str) != string::npos)
						  {
							  lvars.push_back(cit->first);
						  }
						  if (cit->first.find(str2) != string::npos)
						  {
							  lvars2.push_back(cit->first);
						  }
					  }
					  con->addVarList(std::pair<int,int>(stk(s),stk(callee_iter->second)), lvars, lvars2);
				  }
				  case NO_MERGE:
				  {
					  pds->add_rule(stt(), stk(s), stt(), stk(callee_iter->second), stk(ns), one);
					  break;
				  }
				  case MEET_MERGE:
				  case TENSOR_MERGE:
				  {
					  vector<string> lvars;
					  vector<string> lvars2;
					  stringstream ss;
					  ss << s->f << "::";
					  stringstream st;
					  st << f << "::";
					  string str = ss.str();
					  string str2 = st.str();
					  for (std::map<string,int>::const_iterator cit = con->varMap.begin(); cit != con->varMap.end(); ++cit)
					  {
						  if (cit->first.find(str) != string::npos)
							  lvars.push_back(cit->first);
						  if (cit->first.find(str2) != string::npos)
							  lvars2.push_back(cit->first);
					  }
					  merge_fn_t merge;
					  if (merge_type == MEET_MERGE)
						  merge = new MeetMergeFnNWAOBDD(con, lvars, lvars2);
					  else
						  merge = new TensorMergeFnNWAOBDD(con, lvars, lvars2);
					  boost::polymorphic_cast<EWPDS*>(pds)->add_rule(stt(), stk(s), stt(), stk(callee_iter->second), stk(ns), one, merge);
				  }
					  break;
				  default:
					  cerr << "Unknown merge_type\n";
					  assert(0);
				  }
				  break;
			  }
		  }


		  void dump_pds_from_stmt_list(WPDS * pds, stmt_list * sl, NWAOBDDContext * con, const stmt_ptr_stmt_list_ptr_hash_map&
			  goto_to_targets, const stmt_ptr_proc_ptr_hash_map& call_to_callee, const char * f, stmt * es)
		  {
			  while (sl){
				  if (sl->n)
				  {
					  dump_pds_from_stmt(pds, sl->s, con, goto_to_targets, call_to_callee, f, sl->n->s);
				  }
				  else
					  dump_pds_from_stmt(pds, sl->s, con, goto_to_targets, call_to_callee, f, es);
				  sl = sl->n;
			  }
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
      set_merge_type(wali::cprover::details::NO_MERGE);
      BddContext * con = dump_pds_from_prog(pds, pg);
      pds->printStatistics(cout);
      return con;
    }

    BddContext * pds_from_prog_with_meet_merge(wpds::ewpds::EWPDS * pds, prog *pg)
    {
      assert(pg);
      set_merge_type(wali::cprover::details::MEET_MERGE);
      BddContext * con = dump_pds_from_prog(pds, pg);
      pds->printStatistics(cout);
      return con;
    }

    BddContext * pds_from_prog_with_tensor_merge(wpds::ewpds::EWPDS * pds, prog *pg)
    {
      assert(pg);
      set_merge_type(wali::cprover::details::TENSOR_MERGE);
      BddContext * con = dump_pds_from_prog(pds, pg);
      pds->printStatistics(cout);
      return con;
    }

	BddContext * pds_from_prog_with_newton_merge(wpds::ewpds::EWPDS * pds, prog *pg)
	{
		assert(pg);
		set_merge_type(wali::cprover::details::NEWTON_MERGE);
		BddContext * con = dump_pds_from_prog(pds, pg);
		pds->printStatistics(cout);
		return con;
	}

    BddContext * havocLocals(wpds::WPDS * pds, prog * pg, ProgramBddContext * con)
    {
        WpdsRules dr = WpdsRules();
        pds->for_each(dr);
        for( std::set<Rule>::iterator it = dr.pushRules.begin();
	   it != dr.pushRules.end(); ++it ){

        // Find p', y'
        Key p_prime = it->to_state();
        Key y_prime = it->to_stack1();
    	Key y_prime_prime = it->to_stack2();

    	assert(y_prime_prime != WALI_EPSILON);
   	assert(y_prime != WALI_EPSILON);

    	// Create havoc weight h
    	bdd h = con->BaseID();

        //For each procedure
        proc_list * pl = pg->pl;
        while(pl){
          //Iterate through the local variables of the procedure and give them unique names
          const str_list * vl = pl->p->vl;
          while(vl){
            stringstream ss;
            ss << pl->p->f << "::" << vl->v;
            string lhs = ss.str();
            h = con->HavocVar(lhs, h);
            vl = vl->n;
          }
          pl = pl->n;
        }

	BinRel * h_new = new BinRel(con,h);
	sem_elem_t h_sem = (SemElem *)h_new;

    	// For each rule r_n, if r+n is of the form <p',y'> -w-> <p", y''', y''''>
    	//                                      <p',y'> -w-> <p",y'''>
    	//                                      <p',y'> -w-> <p",*>
    	//    weight w_new = h extend w
    	//    add_rule(... same rule with w_new ...);
    	// p", y''', y''''
	for( std::set<Rule>::iterator sit = dr.stepRules.begin();
	     sit != dr.stepRules.end(); sit++ )
	{
	  Key p_prime2 = sit->from_state();
	  Key y_prime2 = sit->from_stack();
	  if ((p_prime2 == p_prime) && (y_prime2 == y_prime))
	  {
	    Key p_dprime = sit->to_state();
	    Key y_prime3 = sit->to_stack1();
	    Key y_prime4 = sit->to_stack2();
	    sem_elem_t w_new = h_sem->extend(sit->weight());
	    pds->replace_rule(p_prime,y_prime,p_dprime,y_prime3,y_prime4,w_new);
	  }
	}

                for( std::set<Rule>::iterator sit = dr.popRules.begin();
             sit != dr.popRules.end(); sit++ )
        {
          Key p_prime2 = sit->from_state();
          Key y_prime2 = sit->from_stack();
          if ((p_prime2 == p_prime) && (y_prime2 == y_prime))
          {
            Key p_dprime = sit->to_state();
            Key y_prime3 = sit->to_stack1();
            Key y_prime4 = sit->to_stack2();
            sem_elem_t w_new = h_sem->extend(sit->weight());
            pds->replace_rule(p_prime,y_prime,p_dprime,y_prime3,y_prime4,w_new);
          }
        }

        for( std::set<Rule>::iterator sit = dr.pushRules.begin();
             sit != dr.pushRules.end(); sit++ )
        {
          Key p_prime2 = sit->from_state();
          Key y_prime2 = sit->from_stack();
          if ((p_prime2 == p_prime) && (y_prime2 == y_prime))
          {
            Key p_dprime = sit->to_state();
            Key y_prime3 = sit->to_stack1();
            Key y_prime4 = sit->to_stack2();
            sem_elem_t w_new = h_sem->extend(sit->weight());
            pds->replace_rule(p_prime,y_prime,p_dprime,y_prime3,y_prime4,w_new);
          }
        }

	return con;
      }
    }

	namespace nwaobddrel {
		NWAOBDDContext * read_prog(WPDS * pds, const char * fname, bool dbg)
		{
			FILE * fin;
			fin = fopen(fname, "r");
			parse(fin);
			prog * pg = parsing_result;
			parsing_result = NULL;
			if (dbg)
				emit_prog(stdout, pg);
			NWAOBDDContext * con = dump_pds_from_prog_nwa(pds, pg, true);
			deep_erase_prog(&pg);
			return con;
		}
	}
		

		NWAOBDDContext * pds_from_prog_nwa(wpds::WPDS * pds, prog * pg, bool first)
		{
			assert(pg);
			set_merge_type(wali::cprover::details::NO_MERGE);
			NWAOBDDContext * con = dump_pds_from_prog_nwa(pds, pg, first);
			pds->printStatistics(cout);
			return con;
		}

		NWAOBDDContext * pds_from_prog_with_meet_merge_nwa(wpds::ewpds::EWPDS * pds, prog *pg, bool first)
		{
			assert(pg);
			set_merge_type(wali::cprover::details::MEET_MERGE);
			NWAOBDDContext * con = dump_pds_from_prog_nwa(pds, pg, first);
			pds->printStatistics(cout);
			return con;
		}

		NWAOBDDContext * pds_from_prog_with_tensor_merge_nwa(wpds::ewpds::EWPDS * pds, prog *pg, bool first)
		{
			assert(pg);
			set_merge_type(wali::cprover::details::TENSOR_MERGE);
			NWAOBDDContext * con = dump_pds_from_prog_nwa(pds, pg, first);
			pds->printStatistics(cout);
			return con;
		}

		NWAOBDDContext * pds_from_prog_with_newton_merge_nwa(wpds::ewpds::EWPDS * pds, prog * pg, bool first)
		{
			assert(pg);
			set_merge_type(wali::cprover::details::NEWTON_MERGE);
			NWAOBDDContext * con = dump_pds_from_prog_nwa(pds, pg, first);
			pds->printStatistics(cout);
			return con;
		}
	
		//Create a rule to havoc the local variables and insert it into the start of each procedure
		NWAOBDDContext * havocLocalsNWA(wpds::WPDS * pds, prog * pg, NWAOBDDContext * con)
		{
			WpdsRules dr = WpdsRules();
			pds->for_each(dr);

			for (std::set<Rule>::iterator it = dr.pushRules.begin();
				it != dr.pushRules.end(); ++it){

				// Find p', y'
				Key p_prime = it->to_state();
				Key y_prime = it->to_stack1();
				Key y_prime_prime = it->to_stack2();

				assert(y_prime_prime != WALI_EPSILON);
				assert(y_prime != WALI_EPSILON);

				// Create havoc weight h
				NWA_OBDD::NWAOBDD h = con->BaseID();

				//For each procedure
				proc_list * pl = pg->pl;
				while (pl){
					//Iterate through the local variables of the procedure and give them unique names
					const str_list * vl = pl->p->vl;
					while (vl){
						stringstream ss;
						ss << pl->p->f << "::" << vl->v;
						string lhs = ss.str();
						//Havoc the locat variables
						h = con->HavocVar(lhs, h);
						vl = vl->n;
					}
					pl = pl->n;
				}

				NWAOBDDRel * h_new = new NWAOBDDRel(con, h);
				sem_elem_t h_sem = (SemElem *)h_new;

				// For each rule r_n, if r+n is of the form <p',y'> -w-> <p", y''', y''''>
				//                                      <p',y'> -w-> <p",y'''>
				//                                      <p',y'> -w-> <p",*>
				//    weight w_new = h extend w
				//    add_rule(... same rule with w_new ...);
				// p", y''', y''''
				for (std::set<Rule>::iterator sit = dr.stepRules.begin();
					sit != dr.stepRules.end(); sit++)
				{
					Key p_prime2 = sit->from_state();
					Key y_prime2 = sit->from_stack();
					if ((p_prime2 == p_prime) && (y_prime2 == y_prime))
					{
						Key p_dprime = sit->to_state();
						Key y_prime3 = sit->to_stack1();
						Key y_prime4 = sit->to_stack2();
						sem_elem_t w_new = h_sem->extend(sit->weight());
						pds->replace_rule(p_prime, y_prime, p_dprime, y_prime3, y_prime4, w_new);
					}
				}

				for (std::set<Rule>::iterator sit = dr.popRules.begin();
					sit != dr.popRules.end(); sit++)
				{
					Key p_prime2 = sit->from_state();
					Key y_prime2 = sit->from_stack();
					if ((p_prime2 == p_prime) && (y_prime2 == y_prime))
					{
						Key p_dprime = sit->to_state();
						Key y_prime3 = sit->to_stack1();
						Key y_prime4 = sit->to_stack2();
						sem_elem_t w_new = h_sem->extend(sit->weight());
						pds->replace_rule(p_prime, y_prime, p_dprime, y_prime3, y_prime4, w_new);
					}
				}

				for (std::set<Rule>::iterator sit = dr.pushRules.begin();
					sit != dr.pushRules.end(); sit++)
				{
					Key p_prime2 = sit->from_state();
					Key y_prime2 = sit->from_stack();
					if ((p_prime2 == p_prime) && (y_prime2 == y_prime))
					{
						Key p_dprime = sit->to_state();
						Key y_prime3 = sit->to_stack1();
						Key y_prime4 = sit->to_stack2();
						sem_elem_t w_new = h_sem->extend(sit->weight());
						pds->replace_rule(p_prime, y_prime, p_dprime, y_prime3, y_prime4, w_new);
					}
				}

				return con;
			}
		}

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

    // May be called any time.
    void remove_skip(prog * pg)
    {
		resolve_details::remove_skip_in_prog(pg);
    }

    void instrument_enforce(prog * pg)
    {
		resolve_details::instrument_enforce_in_prog(pg);
    }

    void instrument_asserts(prog * pg, const char * errLbl)
    {
		resolve_details::instrument_asserts_prog(pg, errLbl);
    }

    void instrument_call_return(prog * pg)
    {
		resolve_details::instrument_call_return_in_prog(pg);
    }

    void make_void_returns_explicit(prog * pg)
    {
		resolve_details::make_void_returns_explicit_in_prog(pg);
    }

    Key getEntryStk(const prog * pg, const char * f)
    {
      assert(pg);
      const proc_list * pl = pg->pl;
      while(pl)
      {
        if(strcmp(pl->p->f, f) == 0){
          assert(pl->p->sl);
          // Entry stack is derived from the proc itself
          // (not the first statement in the proc)
          return stk(pl->p);
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
