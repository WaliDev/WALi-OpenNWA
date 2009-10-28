#ifndef wali_MERGE_FN_GUARD
#define wali_MERGE_FN_GUARD 1

/**
 * @author Akash Lal
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/ref_ptr.hpp"
#include "wali/IMergeFn.hpp"
#include <iostream>


namespace wali
{
  class MergeFn;
  //typedef ref_ptr< MergeFn > merge_fn_t;

  class MergeFn : public IMergeFn//public Printable, public Countable
  {
    public:
      //static const std::string XMLTag;

      MergeFn();

      explicit MergeFn(sem_elem_t sr);

      virtual ~MergeFn();

      virtual sem_elem_t apply_f(sem_elem_t w1, sem_elem_t w2);

      virtual bool equal(merge_fn_t mf);

      virtual MergeFn *parse_element(const char *s, sem_elem_t sem);

      virtual std::ostream &print(std::ostream &o) const;

    private:
      sem_elem_t sr_data;

  };

} // namespacw wali

#endif // wali_MERGE_FN_GUARD

