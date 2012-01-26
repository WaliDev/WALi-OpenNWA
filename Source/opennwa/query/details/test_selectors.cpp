#include "opennwa/query/details/selectors.hpp"
#include "opennwa/query/details/filters.hpp"
#include "opennwa/query/details/genesis.hpp"

#include <cassert>
#include <iostream>
#include <typeinfo>
#include <vector>

using wali::Key;
using wali::nwa::NWA;
using namespace wali::nwa::query::details::selectors;
using namespace wali::nwa::query::details::predicates;
using namespace wali::nwa::query::details::filter;

struct IdSelector
{
    typedef int type;
    int operator()(int x) {
        return x;
    }
};

std::pair<Key, Key>
make_key_pair(int a, int b)
{
    return std::make_pair(Key(a), Key(b));
}

int main(int argc, char**argv)
{

    NWA::Call c(1, 2, 3);
    NWA::Internal i(10, 20, 30);
    NWA::Return r(100, 500, 200, 300);

    // Test the whole-transition selectors
    assert( CallTransitionSelector()(c) == c );
    assert( InternalTransitionSelector()(i) == i );
    assert( ReturnTransitionSelector()(r) == r );

    // Test the ground selectors
    assert( SourceSelector()(c) == 1 );
    assert( SourceSelector()(i) == 10 );
    assert( SourceSelector()(r) == 100 );

    assert( SymbolSelector()(c) == 2 );
    assert( SymbolSelector()(i) == 20 );
    assert( SymbolSelector()(r) == 200 );

    assert( TargetSelector()(c) == 3 );
    assert( TargetSelector()(i) == 30 );
    assert( TargetSelector()(r) == 300 );

    assert( CallPredecessorSelector()(r) == 500 );

    // Test the paired selector
    assert(( PairSelector<SourceSelector, TargetSelector>()(c) == make_key_pair(1, 3) ));
    assert(( PairSelector<SourceSelector, TargetSelector>()(i) == make_key_pair(10, 30) ));
    assert(( PairSelector<SourceSelector, TargetSelector>()(r) == make_key_pair(100, 300) ));

    assert(( PairSelector<SourceSelector, CallPredecessorSelector>()(r) == make_key_pair(100, 500) ));


    ////////////////////
    // Test predicates

    SelectorEqualityPredicate<SourceSelector> source_matcher(SourceSelector(), 1);
    
    assert( makeSelectorEqualityPredicate(SourceSelector(), 1)(c) );
    assert( !makeSelectorEqualityPredicate(SourceSelector(), 0)(c) );

    assert( makeSelectorEqualityPredicate(SymbolSelector(), 2)(c) );
    assert( makeSelectorEqualityPredicate(SymbolSelector(), 20)(i) );
    assert( makeSelectorEqualityPredicate(SymbolSelector(), 200)(r) );
    
    assert( !makeSelectorEqualityPredicate(SymbolSelector(), 3)(c) );
    assert( !makeSelectorEqualityPredicate(SymbolSelector(), 30)(i) );
    assert( !makeSelectorEqualityPredicate(SymbolSelector(), 300)(r) );


    ///////////////////
    // Test IteratorPairIterator

    const int arr[] = {0, 1, 0, 2, 3, 4, 5, 0, 12};
    const int arr_size = sizeof(arr)/sizeof(arr[0]);

    {
        IteratorPairIterator<const int*> ii(arr, arr+arr_size);

        for (int c=0; c<arr_size; ++c) {
            assert( !ii.atEnd() );
            assert( *ii == arr[c] );
            ++ii;
        }
        assert( ii.atEnd() );
    }
    

    //////////////////
    // Test FilterIterator

    {
        IteratorPairIterator<const int*> ii(arr, arr+arr_size);

        int num_zeroes = 0;
        for(FilteringIterator<IteratorPairIterator<const int*>,
                              SelectorEqualityPredicate<IdSelector> >
                fi(ii, makeSelectorEqualityPredicate(IdSelector(), 0));
            !fi.atEnd(); ++fi)
        {
            ++num_zeroes;
        }
        assert( num_zeroes == 3 );
    }

        
    
    

    if (argc > 1) {
        std::cerr << "\n>> Tests good!\n";
        std::cerr << "\n****\nThere should be an error here; this is making sure assert is enabled\n";
        assert(false);
    }
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 4
// End:
