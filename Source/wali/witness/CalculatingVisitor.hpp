#ifndef wali_CALCULATING_VISITOR_GUARD
#define wali_CALCULATING_VISITOR_GUARD 1

/*!
 * @author Evan Driscoll
 */
#include <stack>

#include "wali/Common.hpp"
#include "wali/witness/Visitor.hpp"

#include "wali/witness/CalculatingVisitor.hpp"
#include "wali/witness/WitnessExtend.hpp"
#include "wali/witness/WitnessCombine.hpp"
#include "wali/witness/WitnessMerge.hpp"
#include "wali/witness/WitnessRule.hpp"
#include "wali/witness/WitnessTrans.hpp"


namespace wali
{
  namespace witness
  {
    /*!
     * @class CalculatingVisitor
     *
     * Provides a version of the visitor which essentially gives you an easy
     * way of doing a reinterpretation of a witness. The type of the
     * reinterpretation is given as a template parameter. Subclasses should
     * then implement the calculateType functions, returning the value of a
     * given node.
     *
     * @see Visitable
     * @see Witness
     * @see WitnessExtend
     * @see WitnessCombine
     * @see WitnessRule
     * @see WitnessTrans
     */
    class Witness;
    class WitnessExtend;
    class WitnessCombine;
    class WitnessRule;
    class WitnessTrans; 
    class WitnessMerge;

    template<typename AnswerType>
    class CalculatingVisitor
    {
    private:
      std::stack<AnswerType> answerStack;

      AnswerType popCheck(Witness * expected_source) {
        assert(!answerStack.empty());
        AnswerType temp = answerStack.top();
        answerStack.pop();
        return temp;
      }

      AnswerType push(Witness * source, AnswerType const & ans) {
        answerStack.push(ans);
      }
      
    public:
      // These functions return true to continue visiting children.
      virtual bool visit( Witness * w )                { (void) w; return true; }
      virtual bool visitExtend( WitnessExtend * w )    { (void) w; return true; }
      virtual bool visitCombine( WitnessCombine * w )  { (void) w; return true; }
      virtual bool visitMerge( WitnessMerge * w )      { (void) w; return true; }
      virtual bool visitRule( WitnessRule * w )        { (void) w; return true; }
      virtual bool visitTrans( WitnessTrans * w )      { (void) w; return true; }

      // These functions do fancy-schmancy stuff
      virtual void postvisitExtend( WitnessExtend * w) {
        AnswerType left  = popCheck(w->left().get_ptr());
        AnswerType right = popCheck(w->right().get_ptr());
        
        AnswerType answer = calculateExtend(w, left, right);
        push(w, answer);
      }
      
      virtual void postvisitCombine( WitnessCombine * w) {
        std::list<witness_t> children = w->children();
        
        std::list<AnswerType> child_answers;
        for (std::list<witness_t>::const_iterator child = children.begin(); child != children.end(); ++child) {
          child_answers.push_back(popCheck(child->get_ptr()));
        }

        AnswerType answer = calculateCombine(w, child_answers);
        push(w, answer);
      }
      
      virtual void postvisitMerge( WitnessMerge * w) {
        AnswerType caller = popCheck(w->caller().get_ptr());
        AnswerType rule   = popCheck(w->rule().get_ptr());
        AnswerType callee = popCheck(w->callee().get_ptr());
        
        AnswerType answer = calculateMerge(w, caller, rule, callee);
        push(w, answer);
      }
      
      virtual void postvisitRule( WitnessRule * w) {
        AnswerType answer = calculateRule(w);
        push(w, answer);
      }
      
      virtual void postvisitTrans( WitnessTrans * w) {
        AnswerType answer = calculateTrans(w);
        push(w, answer);
      }


      //! Overload to calculate the value of an extend node. Modifications to
      //! the AnswerType& parameters will not persist.
      virtual AnswerType calculateExtend( WitnessExtend * w, AnswerType & leftValue, AnswerType & rightValue ) = 0;

      //! Overload to calculate the value of an combine node. Modifications to
      //! the list parameter will not persist.
      virtual AnswerType calculateCombine( WitnessCombine * w, std::list<AnswerType> & childrenValues ) = 0;
      
      //! Overload to calculate the value of an merge node. Modifications to
      //! the AnswerType& parameters will not persist.
      virtual AnswerType calculateMerge( WitnessMerge * w, AnswerType & leftValue, AnswerType & ruleValue, AnswerType & rightValue ) = 0;

      //! Overload to calculate the value of a rule node.
      virtual AnswerType calculateRule( WitnessRule * w) = 0;

      //! Overload to calculate the value of a trans node.
      virtual AnswerType calculateTrans( WitnessTrans * w) = 0;
      
    }; // class Visitor

  } // namespace witness

} // namespace wali

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif  // wali_CALCULATING_VISITOR_GUARD

