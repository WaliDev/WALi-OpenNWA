int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int E_ = nondet();
    int F_ = 0;
    int D_ = 1;
    int C_ = 0;
    int B_ = 1;
    if (G_ >= 0 && E_ >= 0 && G_ >= H_ && H_ >= 0 && H_ >= I_ && I_ >= 0) {
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     goto loc_f16;
    }
   }
  goto end;
 }
 loc_f16:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = 1 + F;
    int A_ = 0;
    if (0 >= E && 0 >= F && I >= 1) {
     A = A_;
     F = F_;
     goto loc_f25;
    }
   }
   if (nondet_bool()) {
    int A_ = nondet();
    if (A_ >= 0 && 1 >= A_ && 0 >= E && F >= 2 && I >= 1) {
     A = A_;
     goto loc_f25;
    }
   }
   if (nondet_bool()) {
    int C_ = 1 + C;
    int A_ = 1;
    if (F == 1 && 0 >= C && 0 >= E && I >= 1) {
     A = A_;
     C = C_;
     goto loc_f25;
    }
   }
   if (nondet_bool()) {
    int D_ = -1 + D;
    int C_ = 0;
    int A_ = 0;
    if (C == 1 && F == 1 && D >= 1 && 0 >= E && I >= 1) {
     A = A_;
     C = C_;
     D = D_;
     goto loc_f25;
    }
   }
   if (nondet_bool()) {
    int E_ = nondet();
    int A_ = nondet();
    int F_ = 0;
    int D_ = 1 + B;
    int C_ = 0;
    int B_ = 1 + B;
    if (F == 1 && E_ >= 0 && C >= 2 && 1 >= A_ && A_ >= 0 && 0 >= E && I >= 1) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     goto loc_f25;
    }
   }
   if (nondet_bool()) {
    int E_ = nondet();
    int A_ = nondet();
    int F_ = 0;
    int D_ = 1 + B;
    int C_ = 0;
    int B_ = 1 + B;
    if (C == 1 && F == 1 && E_ >= 0 && 1 >= A_ && A_ >= 0 && 0 >= D && 0 >= E && I >= 1) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     goto loc_f25;
    }
   }
   if (nondet_bool()) {
    int A_ = nondet();
    int E_ = -1 + E;
    if (A_ >= 0 && 1 >= A_ && E >= 1 && I >= 1) {
     A = A_;
     E = E_;
     goto loc_f25;
    }
   }
   if (nondet_bool()) {
    if (0 >= I) {
     goto loc_f73;
    }
   }
  goto end;
 }
 loc_f25:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= A) {
     goto loc_f16;
    }
   }
   if (nondet_bool()) {
    int J_ = 0;
    int F_ = 1 + F;
    if (0 >= E && 0 >= F && A >= 1) {
     F = F_;
     J = J_;
     goto loc_f50;
    }
   }
   if (nondet_bool()) {
    int J_ = nondet();
    if (J_ >= 0 && 1 >= J_ && 0 >= E && F >= 2 && A >= 1) {
     J = J_;
     goto loc_f50;
    }
   }
   if (nondet_bool()) {
    int J_ = 1;
    int C_ = 1 + C;
    if (F == 1 && 0 >= C && 0 >= E && A >= 1) {
     C = C_;
     J = J_;
     goto loc_f50;
    }
   }
   if (nondet_bool()) {
    int J_ = 0;
    int D_ = -1 + D;
    int C_ = 0;
    if (C == 1 && F == 1 && D >= 1 && 0 >= E && A >= 1) {
     C = C_;
     D = D_;
     J = J_;
     goto loc_f50;
    }
   }
   if (nondet_bool()) {
    int J_ = nondet();
    int E_ = nondet();
    int F_ = 0;
    int D_ = 1 + B;
    int C_ = 0;
    int B_ = 1 + B;
    if (F == 1 && E_ >= 0 && C >= 2 && 1 >= J_ && J_ >= 0 && 0 >= E && A >= 1) {
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     J = J_;
     goto loc_f50;
    }
   }
   if (nondet_bool()) {
    int J_ = nondet();
    int E_ = nondet();
    int F_ = 0;
    int D_ = 1 + B;
    int C_ = 0;
    int B_ = 1 + B;
    if (C == 1 && F == 1 && E_ >= 0 && 1 >= J_ && J_ >= 0 && 0 >= D && 0 >= E && A >= 1) {
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     J = J_;
     goto loc_f50;
    }
   }
   if (nondet_bool()) {
    int J_ = nondet();
    int E_ = -1 + E;
    if (J_ >= 0 && 1 >= J_ && E >= 1 && A >= 1) {
     E = E_;
     J = J_;
     goto loc_f50;
    }
   }
  goto end;
 }
 loc_f50:
 {
 __cost++;
   if (nondet_bool()) {
    if (J >= 1 && I >= H) {
     goto loc_f16;
    }
   }
   if (nondet_bool()) {
    int I_ = 1 + I;
    if (J >= 1 && H >= 1 + I) {
     I = I_;
     goto loc_f16;
    }
   }
   if (nondet_bool()) {
    int I_ = -1 + I;
    if (0 >= J) {
     I = I_;
     goto loc_f16;
    }
   }
  goto end;
 }
 loc_f73:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
