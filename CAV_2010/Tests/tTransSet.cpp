/*!
 * @author Nick Kidd
 *
 * Test basic operations on wfa::TransSet.
 * TODO - Finish.
 */

#include "wali/Common.hpp"
#include "wali/wfa/Trans.hpp"
#include "wali/wfa/TransSet.hpp"
#include "wali/wfa/WFA.hpp"
#include "Reach.hpp"

int main()
{
  using wali::Key;
  using wali::getKey;
  using wali::wfa::Trans;
  using wali::wfa::TransSet;
  using wali::wfa::WFA;
  Key a = getKey("a");
  Key b = getKey("b");
  Key c = getKey("c");
  Key d = getKey("d");

  TransSet s;

  sem_elem_t R = new Reach(true);
  s.insert( new Trans(a,b,c,R->one()) );
  s.insert( new Trans(a,getKey("b"),d,R->one()) );
  s.print( std::cout << "TransSet " ) << std::endl;

  return 0;


}

