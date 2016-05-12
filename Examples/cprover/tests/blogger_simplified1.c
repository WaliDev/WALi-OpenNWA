#include "tick.h"

int main(int argc, char ** argv) {
    int tuples;
    //int peek;
    int string_len;
    int URI_tree_nodes, URI_tree_nodes_remaining;
    int i;

    init_tick(0);

    // The original version of this program takes a multi-graph of URIElements
    // as input. In this simplified version, we assume that the multi-graph is
    // acyclic.  Furthermore, we assume that the multi-graph has been converted
    // into a tree, which possibly requires an exponential blowup in the number
    // of nodes.  URI_tree_nodes is the number of nodes in the tree that
    // results from this conversion.
    URI_tree_nodes = __VERIFIER_nondet_int();
    __VERIFIER_assume(URI_tree_nodes >= 0);
    URI_tree_nodes_remaining = URI_tree_nodes;
    string_len = __VERIFIER_nondet_int();
    __VERIFIER_assume(string_len >= 0);

    tuples = 0;
    tuples++;
    while(tuples > 0) {
        tuples--;
        if (__VERIFIER_nondet_bool()) { 
            break; // originally was "return true"
        }
        if (string_len > __VERIFIER_nondet_int()) {
            while(URI_tree_nodes_remaining > 0 && __VERIFIER_nondet_bool()) {
                URI_tree_nodes_remaining--;
                tuples++;
                //tick(1);
            }
        }
        while(URI_tree_nodes_remaining > 0 && __VERIFIER_nondet_bool()) {
            URI_tree_nodes_remaining--;
            tuples++;
            //tick(1);
        }
        tick(1);
    }
    assert(__cost <= URI_tree_nodes + 1);
    //assert(__cost <= 2 * URI_tree_nodes + 1);

    return 0; // originally was "return false"
}
