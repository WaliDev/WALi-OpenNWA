#ifndef wali_graph_FUNCTIONAL_GUARD  
#define wali_graph_FUNCTIONAL_GUARD 1



#include "wali/SemElemTensor.hpp"
#include "wali/ref_ptr.hpp"
#include "wali/Countable.hpp"

#include <vector>

namespace wali
{
  namespace graph
  {
    class IntraGraph;

    class SemElemFunctional;
    typedef wali::ref_ptr<SemElemFunctional> functional_t;
    class SemElemFunctional : public Countable 
    {
      private:
        SemElemFunctional();
        SemElemFunctional(sem_elem_tensor_t wt);
        SemElemFunctional(int intra_nodeno);

        typedef enum {Constant, In, Extend, Combine, Detensor, Tensor, DetensorTranspose, Transpose, TRASH = -1} FuncType;
        FuncType type;
        functional_t lhs, rhs;
        sem_elem_tensor_t value;
        int intra_nodeno;

      public:

        //Static Constructors
        static functional_t constant(sem_elem_tensor_t wt);
        static functional_t in(int intra_nodeno);
        static functional_t extend(functional_t lhs, functional_t rhs);
        static functional_t combine(functional_t lhs, functional_t rhs);
        static functional_t tensor(functional_t lhs, functional_t rhs);
        static functional_t detensor(functional_t arg);
        static functional_t detensorTranspose(functional_t arg);
        static functional_t transpose(functional_t arg);

        sem_elem_tensor_t evaluate(IntraGraph* const gr);
        
        //DEBUGGING
        void leafNodes(std::vector<int>& leaves);
    };
  }
}
#endif //wali_graph_FUNCTIONAL_GUARD  
