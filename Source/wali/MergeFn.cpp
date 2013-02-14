/**
 * @author Akash Lal
 * @author Nicholas Kidd
 * @version $Id$
 */

#include "wali/MergeFn.hpp"

namespace wali
{
  //const std::string MergeFn::XMLTag("MergeFn");

  //MergeFn::MergeFn() : Countable(), sr_data(NULL) { 
  //}
  MergeFn::MergeFn() : IMergeFn(), sr_data(NULL) {
  }

  //MergeFn::MergeFn(sem_elem_t sr) : Countable(), sr_data(sr) { 
  MergeFn::MergeFn(sem_elem_t sr) : IMergeFn(), sr_data(sr) { 
  }

  MergeFn::~MergeFn() { 
  }

  sem_elem_t MergeFn::apply_f(sem_elem_t w1, sem_elem_t w2)
  {
    assert(w1.is_valid() && w2.is_valid());

    if(sr_data.is_empty()) {
      return w1->extend(w2);
    }
    else {
      // Note: Although extend is associative, do not change the
      // order of the following extends. This order is used by
      // Moped's WALi extension. 
      sem_elem_t ret = (w1->extend(sr_data->extend(w2)));

      // w1->print(std::cout << "w1 = ") << "\n";
      // sr_data->print(std::cout << "sr_data = ") << "\n";
      // w2->print(std::cout << "w2 = ") << "\n";
      // ret->print(std::cout << "ret = ") << "\n";

      return ret.get_ptr();
    }
  }

  bool MergeFn::equal(merge_fn_t mf) {
    MergeFn *amf = dynamic_cast<MergeFn *>(mf.get_ptr());
    return (sr_data->equal(amf->sr_data));
  }

  MergeFn *MergeFn::parse_element(const char *s ATTR_UNUSED, sem_elem_t sem)
  {
    (void) s;
    return new MergeFn(sem);
  }

  std::ostream& MergeFn::print(std::ostream &o) const
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

} // namespace wali

