int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F) {
 goto loc_f2;

 loc_f13:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_f400;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = nondet();
    int C_ = nondet();
    int B_ = nondet();
    int A_ = nondet();
    int E_ = 1;
    if (0 >= B_ && C_ >= 1 && A_ >= 1) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f23;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int C_ = nondet();
    int B_ = nondet();
    int A_ = nondet();
    int F_ = 0;
    int E_ = 1;
    if (B_ >= 1 && C_ >= 1 && A_ >= 1) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     goto loc_f23;
    }
   }
  goto end;
 }
 loc_f23:
 {
 __cost++;
   if (nondet_bool()) {
    if (E >= C) {
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int F_ = 1;
    if (C >= 1 + E) {
     F = F_;
     goto loc_f4;
    }
   }
  goto end;
 }
 loc_f33:
 {
 __cost++;
   if (nondet_bool()) {
    if (E >= C) {
     goto loc_f6;
    }
   }
   if (nondet_bool()) {
    int F_ = 1;
    if (C >= 1 + E) {
     F = F_;
     goto loc_f6;
    }
   }
  goto end;
 }
 loc_f4:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= A) {
     goto loc_f14;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int C_ = nondet();
    int E_ = C;
    int A_ = -1 + A;
    if (A >= 1 && 0 >= B && C_ >= C) {
     A = A_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f33;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int C_ = nondet();
    int F_ = 0;
    int E_ = C;
    int A_ = -1 + A;
    if (A >= 1 && B >= 1 && C_ >= C) {
     A = A_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     goto loc_f33;
    }
   }
  goto end;
 }
 loc_f43:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = C;
    if (C == E) {
     E = E_;
     goto loc_f6;
    }
   }
   if (nondet_bool()) {
    int F_ = 1;
    if (C >= 1 + E) {
     F = F_;
     goto loc_f6;
    }
   }
  goto end;
 }
 loc_f53:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = C;
    int B_ = A;
    if (E >= C) {
     B = B_;
     E = E_;
     goto loc_f61;
    }
   }
   if (nondet_bool()) {
    int F_ = 1;
    int E_ = C;
    int B_ = A;
    if (C >= 1 + E) {
     B = B_;
     E = E_;
     F = F_;
     goto loc_f61;
    }
   }
  goto end;
 }
 loc_f6:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = nondet();
    int C_ = nondet();
    int E_ = C;
    if (0 >= B && 0 >= C && C_ >= C) {
     C = C_;
     D = D_;
     E = E_;
     goto loc_f43;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int C_ = nondet();
    int F_ = 0;
    int E_ = C;
    if (B >= 1 && 0 >= C && C_ >= C) {
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     goto loc_f43;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int C_ = nondet();
    int E_ = -1 + C;
    if (0 >= B && C >= 1 && 1 + C_ >= C) {
     C = C_;
     D = D_;
     E = E_;
     goto loc_f53;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int C_ = nondet();
    int F_ = 0;
    int E_ = -1 + C;
    if (B >= 1 && C >= 1 && 1 + C_ >= C) {
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     goto loc_f53;
    }
   }
  goto end;
 }
 loc_f61:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = nondet();
    int C_ = nondet();
    if (C_ >= E && 0 >= B) {
     C = C_;
     D = D_;
     goto loc_f63;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int C_ = nondet();
    int F_ = 0;
    if (C_ >= E && B >= 1) {
     C = C_;
     D = D_;
     F = F_;
     goto loc_f63;
    }
   }
  goto end;
 }
 loc_f63:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = C;
    int D_ = 1 + D;
    if (E >= C) {
     D = D_;
     E = E_;
     goto loc_f71;
    }
   }
   if (nondet_bool()) {
    int F_ = 1;
    int E_ = C;
    int D_ = 1 + D;
    if (C >= 1 + E) {
     D = D_;
     E = E_;
     F = F_;
     goto loc_f71;
    }
   }
  goto end;
 }
 loc_f71:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = nondet();
    int C_ = nondet();
    if (C_ >= E && 0 >= B) {
     C = C_;
     D = D_;
     goto loc_f73;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int C_ = nondet();
    int F_ = 0;
    if (C_ >= E && B >= 1) {
     C = C_;
     D = D_;
     F = F_;
     goto loc_f73;
    }
   }
  goto end;
 }
 loc_f73:
 {
 __cost++;
   if (nondet_bool()) {
    if (E >= C) {
     goto loc_f13;
    }
   }
   if (nondet_bool()) {
    int F_ = 1;
    if (C >= 1 + E) {
     F = F_;
     goto loc_f13;
    }
   }
  goto end;
 }
 loc_f14:
 loc_f400:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
