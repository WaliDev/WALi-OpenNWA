#ifndef wali_CALCULATING_VISITOR_GUARD
#define wali_CALCULATING_VISITOR_GUARD 1

/*!
 * @author Evan Driscoll
 */
#include <stack>
#include <map>

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
    class CalculatingVisitor : public Visitor
    {
    private:

      typedef std::pair<AnswerType, Witness*> SourcedAnswer;
      std::stack<SourcedAnswer> answerStack;

      typedef std::map<Witness*, AnswerType> Cache;
      Cache cache;
      
      AnswerType popCheck(Witness * expected_source) {
        assert(!answerStack.empty());
        SourcedAnswer temp = answerStack.top();
        answerStack.pop();
        assert(temp.second == expected_source);
        return temp.first;
      }

      void push(Witness * source, AnswerType const & ans) {
        answerStack.push(SourcedAnswer(ans, source));
      }

      // Draw previously calculated values from cache, and if found,
      // return false to indicate that children should not be traversed.
      bool visit_helper(Witness * w) {
        typedef typename Cache::const_iterator iter_type;
        iter_type loc = cache.find(w);
        if (loc == cache.end()) {
          return true;
        }
        push(w, loc->second);
        return false;
      }
      
    public:
      AnswerType const & answer() const {
        assert(answerStack.size() == 1);
        return answerStack.top().first;
      }
      
      // These functions return true to continue visiting children.
      virtual bool visit( Witness * w )                { return visit_helper(w); }
      virtual bool visitExtend( WitnessExtend * w )    { return visit_helper(w); }
      virtual bool visitCombine( WitnessCombine * w )  { return visit_helper(w); }
      virtual bool visitMerge( WitnessMerge * w )      { return visit_helper(w); }
      virtual bool visitRule( WitnessRule * w )        { return visit_helper(w); }
      virtual bool visitTrans( WitnessTrans * w )      { return visit_helper(w); }

      // These functions do fancy-schmancy stuff
      virtual void postvisitExtend( WitnessExtend * w) {
        AnswerType right = popCheck(w->right().get_ptr());
        AnswerType left  = popCheck(w->left().get_ptr());
        
        AnswerType ans = calculateExtend(w, left, right);
        push(w, ans);
        assert(cache.find(w) == cache.end());
        cache[w] = ans;
      }
      
      virtual void postvisitCombine( WitnessCombine * w) {
        std::list<witness_t> const & children = w->children();
        
        std::list<AnswerType> child_answers;
        for (std::list<witness_t>::const_reverse_iterator child = children.rbegin();
             child != children.rend(); ++child)
        {
          child_answers.push_front(popCheck(child->get_ptr()));
        }

        AnswerType ans = calculateCombine(w, child_answers);
        push(w, ans);
        assert(cache.find(w) == cache.end());
        cache[w] = ans;
      }
      
      virtual void postvisitMerge( WitnessMerge * w) {
        AnswerType callee = popCheck(w->callee().get_ptr());
        AnswerType rule   = popCheck(w->rule().get_ptr());
        AnswerType caller = popCheck(w->caller().get_ptr());
        
        AnswerType ans = calculateMerge(w, caller, rule, callee);
        push(w, ans);
        assert(cache.find(w) == cache.end());
        cache[w] = ans;
      }
      
      virtual void postvisitRule( WitnessRule * w) {
        AnswerType ans = calculateRule(w);
        push(w, ans);
        assert(cache.find(w) == cache.end());
        cache[w] = ans;
      }
      
      virtual void postvisitTrans( WitnessTrans * w) {
        AnswerType ans = calculateTrans(w);
        push(w, ans);
        assert(cache.find(w) == cache.end());
        cache[w] = ans;
      }


      //! Overload to calculate the value of an extend node. Modifications to
      //! the AnswerType& parameters will not persist.
      virtual AnswerType calculateExtend( WitnessExtend * w, AnswerType & leftValue, AnswerType & rightValue ) = 0;

      //! Overload to calculate the value of an combine node. Modifications to
      //! the list parameter will not persist.
      virtual AnswerType calculateCombine( WitnessCombine * w, std::list<AnswerType> & childrenValues ) = 0;
      
      //! Overload to calculate the value of an merge node. Modifications to
      //! the AnswerType& parameters will not persist.
      virtual AnswerType calculateMerge( WitnessMerge * w, AnswerType & callerValue, AnswerType & ruleValue, AnswerType & calleeValue ) = 0;

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

