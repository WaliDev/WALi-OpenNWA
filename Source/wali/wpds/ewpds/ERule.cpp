/*!
 * @author Akash Lal
 */
#include "wali/Common.hpp"
#include "wali/SemElemPair.hpp"
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
                    extended_se = (SemElem *)(new SemElemPair(se->one(), se));
                } else {
                    extended_se = (SemElem *)(new SemElemPair(se, se->one()));
                }
            }

            ERule::~ERule()
            {

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
                o << "<ERule>\n";
                Rule::f->marshall( o << "\t" ) << std::endl;
                Rule::t->marshall( o << "\t" ) << std::endl;
                o << "\t<Stack2 name=\"" << Rule::to_stack2() << "\"/>\n";
                o << "\t<Weight>" << Rule::weight()->toString() << "</Weight>\n";
                if(merge_fn().get_ptr() != NULL) {
                    o << "\t<MergeFn>" << merge_fn()->toString() << "</MergeFn>\n";
                } else {
                    o << "\t<MergeFn> NONE </MergeFn>\n";
                }
                o << "</ERule>";
                return o;
            }

        } // namespace ewpds

    } // namespace wpds

} // namespace wali

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
 */
