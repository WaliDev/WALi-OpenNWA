#ifndef wali_TAGGED_WEIGHT_GUARD
#define wali_TAGGED_WEIGHT_GUARD 1

/*!
 * @author Akash Lal
 */

#include "wali/SemElem.hpp"
#include <iostream>

namespace wali
{
  namespace walienum {
    enum ETag {NONE, CALL, RETURN};
  }

  class TaggedWeight : public Printable {
    public:
      TaggedWeight(sem_elem_t w, walienum::ETag et) : weight(w), tag(et) { }
      ~TaggedWeight() {}

      bool isCall() {
        return (tag == walienum::CALL);
      }

      bool isRet() {
        return (tag == walienum::RETURN);
      }

      sem_elem_t getWeight() {
        return weight;
      }

      walienum::ETag getTag() {
        return tag;
      }

      std::ostream &print(std::ostream &o) const {
        o << "TaggedWeight[";
        if(tag == walienum::CALL) {
          o << "CALL";
        } else if(tag == walienum::RETURN) {
          o << "RETURN";
        } 
        o << ",";
        weight->print(o);
        o << "]\n";
        return o;
      }

    private:
      sem_elem_t weight;
      walienum::ETag tag;
  };

} // namespace wali

#endif  // wali_TAGGED_WEIGHT_GUARD

