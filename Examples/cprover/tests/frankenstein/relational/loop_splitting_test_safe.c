
/* Source: Rahul Sharma, Isil Dillig, Thomas Dillig, Alex Aiken
   Simplifying Loop Invariant Generation Using Splitter Predicates
   CAV 2011 */

int main(int argc, char ** argv) {
    int x1 = __VERIFIER_nondet_int();
    int y1 = __VERIFIER_nondet_int();
    int x2 = __VERIFIER_nondet_int();
    int y2 = __VERIFIER_nondet_int();
   
    __VERIFIER_assume(x1 == x2);    
    __VERIFIER_assume(y1 == y2);    
 
    while(x1<100) { //multi-path loop
      x1 = x1 + 1;
      if (x1>50) y1 = y1 + 1;
    }

    while (x2<=49) x2 = x2 + 1; //previous loop split into two
    while (x2<100 && x2>49) {
      x2 = x2 + 1;
      y2 = y2 + 1;
    }


    __VERIFIER_assert(x1 == x2);
    __VERIFIER_assert(y1 == y2);

    return 0;
}

