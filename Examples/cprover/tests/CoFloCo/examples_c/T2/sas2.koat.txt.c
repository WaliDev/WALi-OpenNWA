int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = nondet();
    int E_ = 0;
    int B_ = 0;
    int A_ = 4;
    if (1 >= 0) {
     A = A_;
     B = B_;
     D = D_;
     E = E_;
     goto loc_f5;
    }
   }
  goto end;
 }
 loc_f16:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = nondet();
    if (255 >= C) {
     D = D_;
     goto loc_f5;
    }
   }
   if (nondet_bool()) {
    if (C >= 256) {
     goto loc_f30;
    }
   }
  goto end;
 }
 loc_f25:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = nondet();
    if (C >= 0) {
     D = D_;
     goto loc_f5;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + C) {
     goto loc_f30;
    }
   }
  goto end;
 }
 loc_f5:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= A) {
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    if (A >= 2) {
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    int A_ = 1;
    if (A == 1) {
     A = A_;
     goto loc_f30;
    }
   }
  goto end;
 }
 loc_f7:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 0;
    if (B == 0) {
     B = B_;
     goto loc_f9;
    }
   }
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (0 >= 1 + B) {
     A = A_;
     goto loc_f9;
    }
   }
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (B >= 1) {
     A = A_;
     goto loc_f9;
    }
   }
  goto end;
 }
 loc_f9:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 2;
    int C_ = C + A;
    if (D >= 1 + F && 0 >= E) {
     C = C_;
     E = E_;
     goto loc_f16;
    }
   }
   if (nondet_bool()) {
    int E_ = 2;
    int C_ = C + A;
    if (D >= 1 + F && E >= 2) {
     C = C_;
     E = E_;
     goto loc_f16;
    }
   }
   if (nondet_bool()) {
    int E_ = 2;
    int C_ = C + A;
    if (E == 1 && D >= 1 + F && 0 >= 1 + B) {
     C = C_;
     E = E_;
     goto loc_f16;
    }
   }
   if (nondet_bool()) {
    int E_ = 2;
    int C_ = C + A;
    if (E == 1 && D >= 1 + F && B >= 1) {
     C = C_;
     E = E_;
     goto loc_f16;
    }
   }
   if (nondet_bool()) {
    int E_ = 2;
    int C_ = -1 + C + A;
    int B_ = 1;
    int A_ = -1 + A;
    if (E == 1 && B == 0 && D >= 1 + F) {
     A = A_;
     B = B_;
     C = C_;
     E = E_;
     goto loc_f16;
    }
   }
   if (nondet_bool()) {
    int E_ = 1;
    int C_ = C + -A;
    if (F >= 1 + D && 1 >= E) {
     C = C_;
     E = E_;
     goto loc_f25;
    }
   }
   if (nondet_bool()) {
    int E_ = 1;
    int C_ = C + -A;
    if (F >= 1 + D && E >= 3) {
     C = C_;
     E = E_;
     goto loc_f25;
    }
   }
   if (nondet_bool()) {
    int E_ = 1;
    int C_ = C + -A;
    if (E == 2 && F >= 1 + D && 0 >= 1 + B) {
     C = C_;
     E = E_;
     goto loc_f25;
    }
   }
   if (nondet_bool()) {
    int E_ = 1;
    int C_ = C + -A;
    if (E == 2 && F >= 1 + D && B >= 1) {
     C = C_;
     E = E_;
     goto loc_f25;
    }
   }
   if (nondet_bool()) {
    int E_ = 1;
    int C_ = 1 + C + -A;
    int B_ = 1;
    int A_ = -1 + A;
    if (E == 2 && B == 0 && F >= 1 + D) {
     A = A_;
     B = B_;
     C = C_;
     E = E_;
     goto loc_f25;
    }
   }
   if (nondet_bool()) {
    int F_ = D;
    if (D == F) {
     F = F_;
     goto loc_f30;
    }
   }
  goto end;
 }
 loc_f30:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
