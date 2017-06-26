int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = nondet();
    int C_ = 0;
    if (0 >= B_) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f14;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = nondet();
    int C_ = 0;
    if (B_ >= 1024) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f14;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int B_ = nondet();
    int A_ = nondet();
    int C_ = 0;
    if (B_ >= 1 && 1023 >= B_) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     goto loc_f14;
    }
   }
  goto end;
 }
 loc_f14:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int C_ = 1 + C;
    if (Y_0 >= 1 + C) {
     C = C_;
     goto loc_f14;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (C >= Y_0) {
     goto loc_f22;
    }
   }
  goto end;
 }
 loc_f22:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
