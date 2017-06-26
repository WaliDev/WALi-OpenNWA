int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 99;
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_f4;
    }
   }
  goto end;
 }
 loc_f4:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int A_ = nondet();
    if (Y_0 >= 1 + Y_1 && B >= 1 + A) {
     A = A_;
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    if (B >= 1 + A) {
     B = B_;
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_f11;
    }
   }
  goto end;
 }
 loc_f11:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
