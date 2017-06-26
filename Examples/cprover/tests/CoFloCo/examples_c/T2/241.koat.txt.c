int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_f1;

 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = A;
    if (A >= 1) {
     B = B_;
     goto loc_f3;
    }
   }
  goto end;
 }
 loc_f3:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = -1 + B;
    if (B >= 1) {
     B = B_;
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    int B_ = -1 + A;
    int A_ = -1 + A;
    if (A >= 2 && 0 >= B) {
     A = A_;
     B = B_;
     goto loc_f3;
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
