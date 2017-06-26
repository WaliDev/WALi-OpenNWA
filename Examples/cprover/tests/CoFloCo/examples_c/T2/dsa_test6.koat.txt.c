int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = nondet();
    int B_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_f6;
    }
   }
  goto end;
 }
 loc_f6:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (9 >= B) {
     B = B_;
     goto loc_f6;
    }
   }
   if (nondet_bool()) {
    if (B >= 10) {
     goto loc_f15;
    }
   }
  goto end;
 }
 loc_f15:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
