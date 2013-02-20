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
    /**
     * @class SemElemFunctional
     * @brief Build an expression from basic SemElemTensor operations and leaves to be computed later.
     * @see InterGraph::setupNewtonSolution
     * @see IntraGraph::saturate
     *
     * A very simply AST -- build expression trees consisting of Constants, Variables (IntraGraph
     * node numbers, the value is pulled from the IntraGraph itself) and SemElemTensor operations
     * (extend, combine, tensor, detensor, detensorTranspose).
     *
     * SemElemFuncional::evaluate can be used to evaluate the expression.
     **/
    class SemElemFunctional : public Countable 
    {
      private:
        /**
         * private Constructors.
         * Use static functions to create the expression tree.
         **/
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

        /**
         * Evaluate the functional in the context of IntraGraph gr.
         * gr is used to obtain the values of the in nodes -- variables that take values from
         * IntraGraphNodes in gr.
         **/
        sem_elem_tensor_t evaluate(IntraGraph* const gr);
        
        //DEBUGGING
        void leafNodes(std::vector<int>& leaves);
    };
  }
}
#endif //wali_graph_FUNCTIONAL_GUARD  
