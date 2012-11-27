#ifndef __CPROVER_BPL_TO_PDS_HPP_
#define __CPROVER_BPL_TO_PDS_HPP_

#include "wali/HashMap.hpp"

#include "wali/wpds/WPDS.hpp"
#include "wali/wpds/fwpds/FWPDS.hpp"

#include "wali/domains/binrel/BinRel.hpp"
#include "wali/domains/binrel/ProgramBddContext.hpp"

extern "C" {
#include "ast.h"
}

namespace wali 
{
  namespace cprover 
  {
    namespace details 
    {
      namespace resolve_details
      {
        struct hash_str 
        {
          size_t operator() (const char * const & str) const 
          {
            const char * c = str;
            size_t sum = 0;
            while(*c != '\0'){
              sum += *c;
              c++;
            }
            return sum;
          };
        };
        struct str_equal 
        {
          bool operator () (const char * c1, const char * c2)
          {
            if(c1 == NULL && c2 == NULL)
              return true;
            if(c1 == NULL || c2 == NULL)
              return false;
            while(*c1 != '\0' && *c2 != '\0'){
              if(*c1 != *c2)
                return false;
              c1++;
              c2++;
            }
            if(*c1 != '\0' || *c2 != '\0')
              return false;
            return true;
          };
        };
        struct hash_stmt_ptr
        {
          size_t operator() (const stmt * const & s) const 
          {
            return (size_t) s; 
          };
        };
        struct stmt_ptr_equal
        {
          bool operator() (const stmt * s1, const stmt * s2) const
          {
            return s1 == s2;
          };
        };

        typedef wali::HashMap<const char *, stmt *, hash_str, str_equal> str_stmt_ptr_hash_map;
        typedef wali::HashMap<const char *, proc *, hash_str, str_equal> str_proc_ptr_hash_map;
        //IMP: This hashmap owns the stmt_list, but not the stmt in the key, or those in the list.
        //On clearing, we must delete the stmt_list objects, but nothing deeper.
        typedef wali::HashMap<stmt *, stmt_list *, hash_stmt_ptr, stmt_ptr_equal> stmt_ptr_stmt_list_ptr_hash_map;
        typedef wali::HashMap<stmt *, proc *, hash_stmt_ptr, stmt_ptr_equal> stmt_ptr_proc_ptr_hash_map;

        // must use this to delete a hashmap of type stmt_ptr_stmt_list_ptr_hash_map
        void clear_stmt_ptr_stmt_list_ptr_hash_map(stmt_ptr_stmt_list_ptr_hash_map& m);

        void map_label_to_stmt(str_stmt_ptr_hash_map& m, const proc * p);
        void map_name_to_proc(str_proc_ptr_hash_map& m, const prog * pg);
        void map_goto_to_targets(stmt_ptr_stmt_list_ptr_hash_map& mout, str_stmt_ptr_hash_map& min, const proc * p);
        void map_call_to_callee(stmt_ptr_proc_ptr_hash_map& mout, str_proc_ptr_hash_map& min, const proc * p);
      }

      wali::domains::binrel::BddContext * dump_pds_from_prog(wpds::WPDS * pds, prog * pg);
      void dump_pds_from_proc(
          wpds::WPDS * pds, 
          proc * p, 
          domains::binrel::ProgramBddContext * con, 
          const resolve_details::stmt_ptr_stmt_list_ptr_hash_map& goto_to_targets,
          const resolve_details::stmt_ptr_proc_ptr_hash_map& call_to_callee);
      void dump_pds_from_stmt(
          wpds::WPDS * pds, 
          stmt * s, 
          domains::binrel::ProgramBddContext * con, 
          const resolve_details::stmt_ptr_stmt_list_ptr_hash_map& goto_to_targets,
          const resolve_details::stmt_ptr_proc_ptr_hash_map& call_to_callee,
          const char * f,
          stmt * ns);

      void dump_pds_from_stmt_list(
          wpds::WPDS * pds, 
          stmt_list * sl, 
          domains::binrel::ProgramBddContext * con, 
          const resolve_details::stmt_ptr_stmt_list_ptr_hash_map& goto_to_targets, 
          const resolve_details::stmt_ptr_proc_ptr_hash_map& call_to_callee, 
          const char * f, 
          stmt * es);
    }

    // Parses the program in file fname and generates the PDS in pds. pds must be preallocated.
    // Returns the vocabulary generated when parsing the program.
    wali::domains::binrel::BddContext * read_prog(wpds::WPDS * pds, const char * fname, bool dbg = false);
    prog * parse_prog(const char * fname);
//    wpds::WPDS * wpds_from_prog(prog * pg);
//    wpds::fwpds::FWPDS * fwpds_from_prog(prog * pg);
    // Dumps the program as a PDS into pds. pds must be preallocated.
    // Returns the vocabulary gnerated when creating the pds.
    wali::domains::binrel::BddContext * pds_from_prog(wpds::WPDS * pds, prog * pg);
    void print_prog_stats(prog * pg);

    // Must be called to fix fall-through returns *before* dumping PDS
    void make_void_returns_explicit(prog * pg);
    // Must be called after make_void_returns_explicit and before instrument_asserts
    void instrument_enforce(prog * pg);
    // Must be called to fix assert statements, *before* resolving the program.
    void instrument_asserts(prog * pg, const char * errLbl = "error");

    wali::Key getEntryStk(const prog * pg, const char * procname);
    wali::Key getPdsState();
    wali::Key getErrStk(const prog * pg);
  } 
}





#endif //#ifndef __CPROVER_BPL_TO_PDS_HPP_

