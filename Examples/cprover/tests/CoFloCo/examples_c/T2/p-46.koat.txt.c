int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E) {
 goto loc_f1;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = nondet();
    if (0 >= A) {
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int E_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int A_ = 1 + 3 * A;
    if (C_ >= 1 && C_ >= 1 + 2 * E_ && 3 * C_ >= 3 * A && 3 * A >= 3 * C_ && A >= 1 + 2 * E_ && A >= 1) {
     A = A_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f0;
    }
   }
   if (nondet_bool()) {
    int E_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int A_ = 1 + 3 * A;
    if (C_ >= 1 && 2 * E_ >= 1 + C_ && 3 * C_ >= 3 * A && 3 * A >= 3 * C_ && A >= 1 + 2 * E_ && A >= 1) {
     A = A_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f0;
    }
   }
   if (nondet_bool()) {
    int E_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int A_ = 1 + 3 * A;
    if (C_ >= 1 && C_ >= 1 + 2 * E_ && 3 * C_ >= 3 * A && 3 * A >= 3 * C_ && 2 * E_ >= 1 + A && A >= 1) {
     A = A_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f0;
    }
   }
   if (nondet_bool()) {
    int E_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int A_ = 1 + 3 * A;
    if (C_ >= 1 && 2 * E_ >= 1 + C_ && 3 * C_ >= 3 * A && 3 * A >= 3 * C_ && 2 * E_ >= 1 + A && A >= 1) {
     A = A_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f0;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int E_ = nondet();
    int D_ = nondet();
    int A_ = nondet();
    if (A == 2 * E_ && Y_0 >= 1 && 2 * E_ >= 1) {
     A = A_;
     D = D_;
     E = E_;
     goto loc_f0;
    }
   }
  goto end;
 }
 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f0;
    }
   }
  goto end;
 }
 loc_f2:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet());
}
