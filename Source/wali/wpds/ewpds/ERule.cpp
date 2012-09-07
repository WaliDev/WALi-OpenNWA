/**
 * @author Akash Lal
 * @author Nicholas Kidd
 * 
 * @version $Id: ERule.cpp 1095 2011-06-07 18:47:31Z driscoll $
 */
#include "wali/Common.hpp"
#include "wali/SemElemPair.hpp"
#include "wali/wpds/Config.hpp"
#include "wali/wpds/ewpds/ERule.hpp"
#include <sstream>
#include <list>

namespace wali
{
  namespace wpds
  {
    namespace ewpds
    {
      typedef ref_ptr<SemElemPair> sem_elem_pair_t;

      ERule::ERule( Config *f_, Config *t_, wali_key_t stk2_, sem_elem_t se_, merge_fn_t mf_ ) :
        Rule(f_,t_,stk2_,se_), mf(mf_)
      {
        if(stk2_ != WALI_EPSILON) {
          if(mf_.get_ptr() == NULL) {
            mf = new MergeFn(se_);
          }
          //extended_se = (SemElem *)(new SemElemPair(se->one(), se));
        } else {
          //extended_se = (SemElem *)(new SemElemPair(se, se->one()));
        }
      }

      ERule::~ERule()
      {

      }

      /*!
       * @brief Make a copy of the rule
       */
      void ERule::copy(const rule_t r) {
	Rule::copy(r);
	const ERule *er = dynamic_cast<const ERule *>(r.get_ptr());
	mf = er->mf;
      }

      sem_elem_t ERule::extended_weight() const 
      {
        sem_elem_t ret;
        if(Rule::to_stack2() == WALI_EPSILON) {
          ret = (SemElem *)(new SemElemPair(Rule::weight(), Rule::weight()->one()));
        } 
        else {
          ret = (SemElem *)(new SemElemPair(Rule::weight()->one(), Rule::weight()));
        }
        return ret;
      }

      std::ostream & ERule::print( std::ostream &o ) const
      {
        Rule::print(o);
        o << "\t";
        if(merge_fn().get_ptr() != NULL) merge_fn()->print(o);
        return o;
      }

      std::ostream & ERule::marshall( std::ostream & o ) const
      {
        o << "<" << XMLTag << " ";
        o << XMLFromTag << "='" << key2str(from_state()) << "' ";
        o << XMLFromStackTag << "='" << key2str(from_stack()) << "' ";
        o << XMLToTag << "='" << key2str(to_state()) << "'";

        // Check optional stack symbols
        if( WALI_EPSILON != to_stack1() ) {
          o << " " << XMLToStack1Tag << "='" << key2str(to_stack1()) << "'";
          if( WALI_EPSILON != to_stack2() ) {
            o << " " << XMLToStack2Tag << "='" << key2str(to_stack2()) << "'";
          }
        }
        else {
          // sanity check
          assert( WALI_EPSILON == to_stack2() );
        }
        o << ">";
        se->marshallWeight(o);
        if(merge_fn().get_ptr() != NULL) {
          o << "\t<MergeFn>" << merge_fn()->toString() << "</MergeFn>\n";
        } 
        else {
          o << "\t<MergeFn> NONE </MergeFn>\n";
        }

        o << "</" << XMLTag << ">";
        return o;
      }

    } // namespace ewpds

  } // namespace wpds

} // namespace wali

