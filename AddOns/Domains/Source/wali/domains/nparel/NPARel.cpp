/**
 * @author Jason Breck
 *
 */
#include "NPARel.hpp"

using namespace wali;
namespace wali {
  namespace domains {
    namespace nparel {
      nparelpair_t NPARelPair::MkNPARelPair(nparel_t v1, nparel_t v2) {
          nparelpair_t d = new NPARelPair(v1, v2);
          return d;
      }

      NPARel::NPARel(const NPARel &that) : SemElemTensor(that) {}

      NPARel::NPARel() : SemElemTensor() {}

      NPARel::~NPARel() {}
    }
  }
}
