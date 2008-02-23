/*!
 * @author Akash Lal
 */

#include "MergeFunction.hpp"

namespace wali
{
    namespace wpds
    {
        namespace ewpds
        {
            const std::string MergeFn::XMLTag("MergeFn");

            SemElem *MergeFn::apply_f(SemElem *w1, SemElem *w2)
            {
                assert(w1 != NULL && w2 != NULL);

                if(sr_data.is_empty()) {
                    return (w1->extend(w2)).get_ptr();
                }
                else {
				    // Note: Although extend is associative, do not change the
				    // order of the following extends. This order is used by
				    // Moped's WALi extension. 
                    return (w1->extend(sr_data->extend(w2))).get_ptr();
                }
            }

            sem_elem_t MergeFn::apply_f(sem_elem_t w1, sem_elem_t w2)
            {
                assert(w1.is_valid() && w2.is_valid());

                if(sr_data.is_empty()) {
                    return w1->extend(w2);
                }
                else {
                    return w1->extend(sr_data->extend(w2));
                }
            }

            MergeFn *MergeFn::parse_element(const char *s ATTR_UNUSED, SemElem *sem)
            {
                return new MergeFn(sem);
            }

            std::ostream &MergeFn::print(std::ostream &o) const
            {
                o << "MergeFn[";
                if(sr_data.is_empty()) {
                    o <<"ONE";
                } else {
                    sr_data->print(o);
                }
                o<< "]";
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
