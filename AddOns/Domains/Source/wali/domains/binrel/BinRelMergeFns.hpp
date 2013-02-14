// ::wali::domains::binrel
#include "wali/domains/binrel/BinRel.hpp"
// ::wali
#include "wali/MergeFn.hpp"
#include "wali/SemElemTensor.hpp"
namespace wali
{
  namespace cprover
  {
    class MeetMergeFn : public wali::IMergeFn
    {
      public:
        /**
         * @param con 
         * The vocabulary of the semiring in use. MeetMergeFn does not take
         * ownership of con.
         * @param localVars
         * The set of local variables of the callee function.
         **/
        MeetMergeFn(wali::domains::binrel::BddContext * con, std::vector<std::string> const& localVars);

        virtual ~MeetMergeFn() {}

        // For poststar, w1 = weight at callsite, 
        // w2 = weight at exit of callee 
        sem_elem_t apply_f(sem_elem_t w1, sem_elem_t w2); 

        virtual bool equal(merge_fn_t mf);
        virtual std::ostream& print(std::ostream& o) const;
      private:
        // Let (l,g,l',g') be a tuple in the semiring.
        // exitWeight * havocCalleeLocals has the effect of havocking all post
        // vocabulary locals in the exitWeight
        // havocCalleeLocals = \forall{l,g,l'} (l,g,l',g)
        wali::domains::binrel::binrel_t havocCalleeLocals;
        // (exitWeight * havocCalleeLocals) & constrainLocals forces the
        // identity on local variables
        wali::domains::binrel::binrel_t constrainLocals;
    };  
    typedef ref_ptr<MeetMergeFn> meet_merge_fn_t;

    class TensorMergeFn : public wali::IMergeFn
    {
      public:
        /**
         * @param con 
         * The vocabulary of the semiring in use. MeetMergeFn does not take
         * ownership of con.
         * @param localVars
         * The set of local variables of the callee function.
         **/
        TensorMergeFn(wali::domains::binrel::BddContext * con, std::vector<std::string> const& localVars);

        virtual ~TensorMergeFn() {}

        // For poststar, w1 = weight at callsite, 
        // w2 = weight at exit of callee 
        sem_elem_t apply_f(sem_elem_t w1, sem_elem_t w2);

        virtual bool equal(merge_fn_t mf);
        virtual std::ostream& print(std::ostream& o) const;
      private:
        // Let (l,g,l',g') be a tuple in the base semiring.
        // Let ((L1,G1,L1',G1')o(L2,G2,L2',G2')) be a tuple in the tensor semiring such that
        // detensor((L1,G1,L1',G1')o(L2,G2,L2',G2'))
        //   = \exists{L1',G1',L2,G2} ((L1,G1,L1',G1')o(L2,G2,L2',G2')) ^ L1' = L2 ^ G1' = G2

        // exitWeight * havocCalleeLocals has the effect of havocking all post
        // vocabulary locals in the exitWeight
        // havocCalleeLocals = \forall{l,g,l'} (l,g,l',g)
        wali::domains::binrel::binrel_t havocCalleeLocals;
        // (exitWeight * havocCalleeLocals) \tensor id 
        // stacks id on top
        // id = \forall{l,g} (l,g,l,g)
        wali::domains::binrel::binrel_t id;
        // This is a tensored weight, that is a restriction of the tensored id. with L1=L2,
        // idWithEqualLocals = \forall{L1,G1,G2} (L1,G1,L1,G1)o(L1,G2,L1,G2)
        wali::domains::binrel::binrel_t idWithEqualLocals;
    };
    typedef ref_ptr<TensorMergeFn> tensor_merge_fn_t;

  }
}
