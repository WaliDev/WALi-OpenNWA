int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = nondet();
    int A_ = nondet();
    if (0 >= C_) {
     A = A_;
     C = C_;
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    int C_ = nondet();
    int A_ = nondet();
    if (C_ >= 101) {
     A = A_;
     C = C_;
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int C_ = nondet();
    int A_ = nondet();
    int B_ = 0;
    if (C_ >= 1 && 100 >= C_) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     goto loc_f15;
    }
   }
  goto end;
 }
 loc_f15:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1 + B) {
     goto loc_f16;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_f16;
    }
   }
   if (nondet_bool()) {
    int E_ = 0;
    int B_ = -1 + A;
    if (A == B) {
     B = B_;
     E = E_;
     goto loc_f23;
    }
   }
  goto end;
 }
 loc_f16:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (1 >= 0) {
     B = B_;
     goto loc_f15;
    }
   }
   if (nondet_bool()) {
    int E_ = 0;
    int B_ = -1 + B;
    if (1 >= 0) {
     B = B_;
     E = E_;
     goto loc_f23;
    }
   }
  goto end;
 }
 loc_f23:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 1 + E;
    if (B >= 1 + E) {
     E = E_;
     goto loc_f23;
    }
   }
   if (nondet_bool()) {
    if (E >= B) {
     goto loc_f7;
    }
   }
  goto end;
 }
 loc_f7:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet());
}
