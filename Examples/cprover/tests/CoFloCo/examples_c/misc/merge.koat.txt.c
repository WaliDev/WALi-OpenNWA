int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_start;

 loc_merge:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (B >= 1 && A >= 1) {
     A = A_;
     goto loc_merge;
    }
   }
   if (nondet_bool()) {
    int B_ = -1 + B;
    if (B >= 1 && A >= 1) {
     B = B_;
     goto loc_merge;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_merge;
    }
   }
  goto end;
 }
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
