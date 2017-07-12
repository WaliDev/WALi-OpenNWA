#include "popl18-assert.h"

#include "tick.h"
#include "log.h"

DECLARE_LOG(2)

// This function assumes that it has been given a binary search tree (NOT an
//   actual heap) that is arranged like a heap; that is, a binary search tree
//   whose root is at array_tree[0], and where the (smaller) left child of
//   array_tree[i] lives at array_tree[2*i+1] and the (larger) right child of
//   array_tree[i] lives at array_tree[2*i+2].
int binary_search(int value, int* array_tree, int array_tree_size) {
    int position, found;
    position = 0;
    found = 0;
    while(position < array_tree_size) {
        tick(1);
        if (array_tree[position] == 0) break; // Dummy leaf node
        if (array_tree[position] == value) { found = 1; break; }
        if (array_tree[position] < value) {
            position = position * 2 + 1;
        } else {
            position = position * 2 + 2;
        }
    }
    __VERIFIER_print_hull(__cost);
    return found;
}

int main(int argc, char ** argv) {
    init_tick(0);

    int array_tree_size = __VERIFIER_nondet_int();

    __VERIFIER_assert(array_tree_size >= 2);

    int* array_tree = malloc(array_tree_size);

    // ... a real application would write to array_tree here ...
    
    binary_search(100, &array_tree, array_tree_size);

    //int lg_2size_plus_1 = icra_log2((2 * array_tree_size) + 1);
    int lg_size = icra_log2(array_tree_size);

    // We get a reasonable bound from print_hull, but we aren't proving these yet:
    __VERIFIER_assert(__cost <= lg_size + 2);
    //__VERIFIER_assert(__cost <= 1 || __cost <= lg_2size_plus_1 || __cost <= lg_size + 1);

    return 0;
}
