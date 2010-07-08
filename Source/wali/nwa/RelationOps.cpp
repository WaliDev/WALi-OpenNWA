#include "RelationOps.hpp"

using namespace wali::relations;

namespace std {
std::ostream& operator << (std::ostream& os, std::pair<int,int> const & p)
{
  os << "(" << p.first << ", " << p.second << ")";
  return os;
}
}

int main()
{
  TernaryRelation<int>::iterator x;
  x++;
  ++x;
  *x;
  // typedef RelationTypedefs<int>::BinaryRelation BinaryRelation;
  // typedef RelationTypedefs<int>::TernaryRelation TernaryRelation;
  // BinaryRelation br1, br2, br1_2, br4;
  // TernaryRelation m2;
  // std::set<wali::Triple<int, int, int> > delta;
  // std::set<wali::Quad<int, int, int, int> > delta2;

  // br1.insert(make_pair(1, 1));
  // br1.insert(make_pair(1, 2));
  // br1.insert(make_pair(1, 3));
  // br1.insert(make_pair(3, 3));
  // br1.insert(make_pair(4, 4));
  // br1.insert(make_pair(5, 5));

  // br2.insert(make_pair(1, 1));
  // br2.insert(make_pair(1, 2));
  // br2.insert(make_pair(2, 2));
  // br2.insert(make_pair(3, 4));
  // br2.insert(make_pair(4, 3));

  // compose(br1_2, br1, br2);

  // std::cout << "Composition:\n";
  // std::copy(br1_2.begin(), br1_2.end(),
  // 	    std::ostream_iterator<pair<int,int> >(std::cout, "\n"));

  // //merge(m, m, m, m2);

  // //project_symbol_3(m, delta, 0);
  // //project_symbol_4(m2, delta2, 0);
  // //intersection(m,m,m);
}
