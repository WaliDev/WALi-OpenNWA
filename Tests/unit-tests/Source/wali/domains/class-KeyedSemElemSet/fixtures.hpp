#ifndef WALI_TEST_DOMAINS_KEYED_SEM_ELEM_SET_FIXTURES_HPP
#define WALI_TEST_DOMAINS_KEYED_SEM_ELEM_SET_FIXTURES_HPP

#include <string>
#include <vector>
#include "wali/domains/KeyedSemElemSet.hpp"
#include "wali/ShortestPathSemiring.hpp"

namespace {
    using wali::ref_ptr;
    using std::string;
    using wali::sem_elem_t;
    using wali::domains::PositionKey;
}

struct PKFixtures {    
    ref_ptr<PositionKey<int> > i_empty, i_id, i00, i01, i10, i11, i10copy;
    ref_ptr<PositionKey<string> > s_empty, s_id, s00, s01, s10, s11, s10copy;

    // Don't really use these, they're just to make the initialization list
    // readable
    typedef PositionKey<int> Pki;
    typedef PositionKey<string> Pks;

    PKFixtures()
        : i_empty(new Pki(PositionKey<int>::makeZero()))
        , i_id(new Pki(PositionKey<int>::makeOne()))
        , i00(new Pki(0, 0))
        , i01(new Pki(0, 1))
        , i10(new Pki(1, 0))
        , i11(new Pki(1, 1))
        , i10copy(new Pki(*i10))
        , s_empty(new Pks(PositionKey<string>::makeZero()))
        , s_id(new Pks(PositionKey<string>::makeOne()))
        , s00(new Pks("zero", "zero"))
        , s01(new Pks("zero", "one"))
        , s10(new Pks("one", "zero"))
        , s11(new Pks("one", "one"))
        , s10copy(new Pks(*s10))
    {}
};

struct PKLists {   
    std::vector<sem_elem_t> ints;
    std::vector<sem_elem_t> strings;

    PKLists() {
        PKFixtures f;
        ints.push_back(f.i_empty);
        ints.push_back(f.i_id);
        ints.push_back(f.i00);
        ints.push_back(f.i01);
        ints.push_back(f.i10);
        ints.push_back(f.i11);

        strings.push_back(f.s_empty);
        strings.push_back(f.s_id);
        strings.push_back(f.s00);
        strings.push_back(f.s01);
        strings.push_back(f.s10);
        strings.push_back(f.s11);
    }
};


struct ShortestPathLengths
{
    sem_elem_t ten, twenty, thirty, fourty, fifty;

    ShortestPathLengths()
        : ten(new wali::ShortestPathSemiring(10))
        , twenty(new wali::ShortestPathSemiring(20))
        , thirty(new wali::ShortestPathSemiring(30))
        , fourty(new wali::ShortestPathSemiring(40))
        , fifty(new wali::ShortestPathSemiring(50))
    {}
};



#endif /* WALI_TEST_DOMAINS_KEYED_SEM_ELEM_SET_FIXTURES_HPP */
