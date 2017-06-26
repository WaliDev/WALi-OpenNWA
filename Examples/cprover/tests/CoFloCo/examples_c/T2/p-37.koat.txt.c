int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F) {
 goto loc_f12;

 loc_f12:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 0;
    int B_ = 0;
    int A_ = 4;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f5;
    }
   }
  goto end;
 }
 loc_f5:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 0;
    int D_ = 0;
    int C_ = 0;
    if (C == 0 && B >= A) {
     C = C_;
     D = D_;
     E = E_;
     goto loc_f11;
    }
   }
   if (nondet_bool()) {
    int E_ = C;
    int D_ = C;
    if (C >= 1) {
     D = D_;
     E = E_;
     goto loc_f10;
    }
   }
   if (nondet_bool()) {
    int E_ = C;
    int D_ = C;
    if (0 >= 1 + C) {
     D = D_;
     E = E_;
     goto loc_f10;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int B_ = 1 + B;
    if (C == 0 && A >= 1 + B) {
     B = B_;
     C = C_;
     D = D_;
     F = F_;
     goto loc_f5;
    }
   }
  goto end;
 }
 loc_f7:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int B_ = 1 + B;
    if (1 >= 0) {
     B = B_;
     C = C_;
     D = D_;
     F = F_;
     goto loc_f5;
    }
   }
  goto end;
 }
 loc_f8:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int B_ = 1 + B;
    if (1 >= 0) {
     B = B_;
     C = C_;
     D = D_;
     F = F_;
     goto loc_f5;
    }
   }
  goto end;
 }
 loc_f11:
 loc_f10:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
