#ifndef WALI_EWPDS_MERGE_FUNCTIONS_H_
#define WALI_EWPDS_MERGE_FUNCTIONS_H_

/*!
 * @author Akash Lal
 */

#include "wali/Common.hpp"
#include "wali/ref_ptr.hpp"
#include "wali/SemElem.hpp"
#include <stdio.h>


namespace wali
{
    namespace wpds
    {
        namespace ewpds
        {
            class MergeFn;
            typedef ref_ptr< MergeFn > merge_fn_t;

            class MergeFn : public Printable, public Countable
            {
                public:
                    static const std::string XMLTag;

                    MergeFn(bool countme = true) : Countable(countme), sr_data(NULL) { }
                    MergeFn(SemElem *sr, bool countme = true) : Countable(countme), sr_data(sr) { }
                    MergeFn(sem_elem_t sr, bool countme = true) : Countable(countme), sr_data(sr) { }

                    virtual SemElem *apply_f(SemElem *w1, SemElem *w2);
                    virtual sem_elem_t apply_f(sem_elem_t w1, sem_elem_t w2);

                    virtual MergeFn *parse_element(const char *s, SemElem *sem);

                    virtual std::ostream &print(std::ostream &o) const;

                    virtual ~MergeFn() { }

                private:
                    sem_elem_t sr_data;

            };

        } // namespace ewpds

    } // namespace wpds

} // namespacw wali

#endif

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
 */
