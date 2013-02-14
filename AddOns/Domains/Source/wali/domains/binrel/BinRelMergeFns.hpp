// ::wali::domains::binrel
#include "wali/domains/binrel/BinRel.hpp"
// ::wali
#include "wali/MergeFn.hpp"
namespace wali
{
  namespace cprover
  {
    class MeetMergeFn : public wali::IMergeFn
    {
      public:
        MeetMergeFn(
            wali::domains::binrel::binrel_t havocCalleeLocals, 
            wali::domains::binrel::binrel_t constrainLocals  
            );

        virtual ~MeetMergeFn() {}

        sem_elem_t apply_f(sem_elem_t w1, sem_elem_t w2);      // For poststar, w1 = weight at callsite, 
        // w2 = weight at exit of callee 

        virtual bool equal(merge_fn_t mf);
        virtual std::ostream& print(std::ostream& o) const;
      private:
        // exitWeight * havocCalleeLocals has the effect of havocking all post
        // vocabulary locals in the exitWeight
        wali::domains::binrel::binrel_t havocCalleeLocals;
        // (exitWeight * havocCalleeLocals) & constrainLocals forces the
        // identity on local variables
        wali::domains::binrel::binrel_t constrainLocals;
    };
    
    class TensorMergeFn : public wali::IMergeFn
    {
      public:
        TensorMergeFn(
            wali::domains::binrel::binrel_t havocCalleeLocals, 
            wali::domains::binrel::binrel_t constrainLocals  
            );

        virtual ~MeetMergeFn() {}

        sem_elem_t apply_f(sem_elem_t w1, sem_elem_t w2);      // For poststar, w1 = weight at callsite, 
        // w2 = weight at exit of callee 

        virtual bool equal(merge_fn_t mf);
        virtual std::ostream& print(std::ostream& o) const;
      private:
        // exitWeight * havocCalleeLocals has the effect of havocking all post
        // vocabulary locals in the exitWeight
        wali::domains::binrel::binrel_t havocCalleeLocals;
        // (exitWeight * havocCalleeLocals) & constrainLocals forces the
        // identity on local variables
        wali::domains::binrel::binrel_t constrainLocals;
    };
  }
}
