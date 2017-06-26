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
    int B_ = 0;
    if (0 >= 1 + A_) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    int A_ = nondet();
    int C_ = 0;
    int B_ = 0;
    if (A_ >= 1) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    int B_ = 1023;
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f7;
    }
   }
  goto end;
 }
 loc_f7:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 2 + D;
    int C_ = 1 + C;
    if (B >= C) {
     C = C_;
     D = D_;
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    if (1022 >= E && C >= 1 + B && E >= 0) {
     goto loc_f21;
    }
   }
   if (nondet_bool()) {
    if (E >= 1023 && C >= 1 + B) {
     goto loc_f21;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + E && C >= 1 + B) {
     goto loc_f21;
    }
   }
  goto end;
 }
 loc_f21:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet());
}
