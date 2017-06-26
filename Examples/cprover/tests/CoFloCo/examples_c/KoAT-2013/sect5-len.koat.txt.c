int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_l0;

 loc_l0:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     goto loc_l1;
    }
   }
  goto end;
 }
 loc_l1:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = -1 + B;
    int A_ = 1 + A;
    if (B >= 1) {
     A = A_;
     B = B_;
     goto loc_l1;
    }
   }
   if (nondet_bool()) {
    if (0 >= B) {
     goto loc_l2;
    }
   }
  goto end;
 }
 loc_l2:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
