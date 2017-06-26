int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 0;
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_f5;
    }
   }
  goto end;
 }
 loc_f5:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int C_ = Y_0 + Y_1;
    int B_ = 2 + B;
    int A_ = 1 + A;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f5;
    }
   }
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f16;
    }
   }
  goto end;
 }
 loc_f16:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
