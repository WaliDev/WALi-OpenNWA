#include "opennwa/query/details/iterators.hpp"

#include <iostream>
#include <cassert>
#include <vector>
#include <iterator>

const int arr[] = {0,1,2,3,4,5,6,7,8,9,10};
const int size_arr = sizeof(arr)/sizeof(arr[0]);

std::vector<int> v(arr, arr+size_arr);


using namespace wali::nwa::query::details::iterators;

int main(int argc, char** argv)
{
    cplusplus_iterator_adapter<std::vector<int>::const_iterator> a_begin(v.begin());
    cplusplus_iterator_adapter<std::vector<int>::const_iterator> a_end(v.end());

    //assert(a_begin == a_end);
    
    const int* pi = arr;
    std::vector<int>::const_iterator vi = v.begin();
    iterator_base_wrapper<const int> ii(a_begin);
    iterator_base_wrapper<const int> ii_end(a_end);


    
    iterator_base_wrapper<const int> ii2 = ii;
    

    while(ii != ii_end) {
        assert( *pi == *vi );
        assert( *pi == *ii );

        ++pi;
        ++vi;
        ++ii;
    }

    assert( pi == arr+size_arr );
    assert( vi == v.end() );


    assert( *ii2 == arr[0] );

    //////////

    const int num = 5;

    iterator_sequence<std::vector<int>::const_iterator> sequence;
    iterator_sequence<std::vector<int>::const_iterator> end;

    for(int i=0; i<num; ++i) {
        sequence.append_range(v.begin(), v.end());
    }

    for(int i=0; i<num; ++i) {
        for(std::vector<int>::const_iterator vi = v.begin(); vi != v.end(); ++vi, ++sequence) {
            assert(sequence != end);
            assert(*sequence == *vi);
        }
    }
    assert(sequence == end);


    if (argc > 1) {
        std::cerr << "\n>> Tests good!\n";
        std::cerr << "\n****\nThere should be an error here; this is making sure assert is enabled\n";
        assert(false);
    }
}
