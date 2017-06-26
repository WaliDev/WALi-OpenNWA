int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = nondet();
    int C_ = 0;
    int B_ = 1;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = -1 + B;
    int A_ = -10 + A;
    if (A >= 101 && B >= 1) {
     A = A_;
     B = B_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    int A_ = 11 + A;
    if (100 >= A && B >= 1) {
     A = A_;
     B = B_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int E_ = B;
    int D_ = A;
    int C_ = 1;
    int B_ = -1 + B;
    int A_ = -10 + A;
    if (B >= 1 && 0 >= C && A >= 101) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int E_ = B;
    int D_ = A;
    int C_ = 1;
    int B_ = 1 + B;
    int A_ = 11 + A;
    if (B >= 1 && 0 >= C && 100 >= A) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    if (B >= E && C >= 1 && D >= A) {
     goto loc_f2;
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
