#ifndef WALI_TESTING_STRINGWEIGHT_HPP
#define WALI_TESTING_STRINGWEIGHT_HPP

#include "wali/SemElem.hpp"

namespace testing
{
    /// NOTE: THIS IS NOT ACTUALLY A VALID SEMIRING
    struct StringWeight : wali::SemElem
    {
        std::string str;

        StringWeight() {}
        StringWeight(std::string const & s) : str(s) {}

        wali::sem_elem_t one() const {
            static wali::sem_elem_t r = new StringWeight();
            return r;
        }

        wali::sem_elem_t zero() const {
            static wali::sem_elem_t r;
            if (r == NULL) {
                r = new StringWeight("ZERO");
            }
            return r;
        }

        wali::sem_elem_t extend(SemElem * se) {
            StringWeight * w = dynamic_cast<StringWeight*>(se);
            if (w) {
                if (this == zero().get_ptr() || w == zero().get_ptr()) {
                    return zero();
                }
                return new StringWeight(str + " " + w->str);
            }
            assert (false);
            return NULL;
        }

        wali::sem_elem_t combine(SemElem * se) {
            StringWeight * w = dynamic_cast<StringWeight*>(se);
            if (w) {
                if (this == zero().get_ptr()) {
                    return w;
                }
                else if (w == zero().get_ptr()) {
                    return this;
                }
                return new StringWeight(str + " | " + w->str);
            }
            assert (false);
            return NULL;
        }

        bool equal(SemElem * se) const {
            StringWeight * w = dynamic_cast<StringWeight*>(se);
            if (w) {
                return w->str == str;
            }
            return false;
        }

        std::ostream& print( std::ostream & o ) const {
            o << ("[" + str + "]");
            return o;
        }

    };
}


#endif /* WALI_TESTING_STRINGWEIGHT_HPP */
