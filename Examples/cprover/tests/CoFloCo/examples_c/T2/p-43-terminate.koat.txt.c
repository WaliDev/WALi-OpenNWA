int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J) {
 goto loc_f8;

 loc_f13:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = nondet();
    int D_ = -1 + C;
    int C_ = -1 + C;
    if (0 >= 1 + B_) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     goto loc_f31;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = nondet();
    int D_ = -1 + C;
    int C_ = -1 + C;
    if (B_ >= 1) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     goto loc_f31;
    }
   }
   if (nondet_bool()) {
    int A_ = nondet();
    int D_ = -1 + D;
    int C_ = -2 + D;
    int B_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     goto loc_f29;
    }
   }
  goto end;
 }
 loc_f19:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = nondet();
    int D_ = -1 + C;
    int C_ = -1 + C;
    if (0 >= 1 + B_) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     goto loc_f31;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = nondet();
    int D_ = -1 + C;
    int C_ = -1 + C;
    if (B_ >= 1) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     goto loc_f31;
    }
   }
   if (nondet_bool()) {
    int A_ = nondet();
    int D_ = -1 + D;
    int C_ = -2 + D;
    int B_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     goto loc_f29;
    }
   }
  goto end;
 }
 loc_f27:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = nondet();
    int G_ = 0;
    int E_ = 0;
    int D_ = -1 + D;
    int C_ = -2 + D;
    int B_ = 0;
    if (E == 0 && C >= 1 && C + D >= 1 && D >= 1) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     G = G_;
     goto loc_f27;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = nondet();
    int G_ = 0;
    int E_ = 0;
    int D_ = -1 + C;
    int C_ = -1 + C;
    if (E == 0 && C + D >= 1 && 0 >= 1 + B_ && C >= 1 && D >= 1) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     G = G_;
     goto loc_f27;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = nondet();
    int G_ = 0;
    int E_ = 0;
    int D_ = -1 + C;
    int C_ = -1 + C;
    if (E == 0 && C + D >= 1 && B_ >= 1 && C >= 1 && D >= 1) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     G = G_;
     goto loc_f27;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int B_ = nondet();
    int A_ = nondet();
    int I_ = D;
    int H_ = C;
    int E_ = 1;
    int D_ = -1 + C;
    int C_ = -1 + C;
    if (E == 0 && 0 >= 1 + G_ && 0 >= 1 + B_ && C + D >= 1 && C >= 1 && D >= 1) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     G = G_;
     H = H_;
     I = I_;
     goto loc_f30;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int B_ = nondet();
    int A_ = nondet();
    int I_ = D;
    int H_ = C;
    int E_ = 1;
    int D_ = -1 + C;
    int C_ = -1 + C;
    if (E == 0 && G_ >= 1 && 0 >= 1 + B_ && C + D >= 1 && C >= 1 && D >= 1) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     G = G_;
     H = H_;
     I = I_;
     goto loc_f30;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int B_ = nondet();
    int A_ = nondet();
    int I_ = D;
    int H_ = C;
    int E_ = 1;
    int D_ = -1 + C;
    int C_ = -1 + C;
    if (E == 0 && 0 >= 1 + G_ && B_ >= 1 && C + D >= 1 && C >= 1 && D >= 1) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     G = G_;
     H = H_;
     I = I_;
     goto loc_f30;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int B_ = nondet();
    int A_ = nondet();
    int I_ = D;
    int H_ = C;
    int E_ = 1;
    int D_ = -1 + C;
    int C_ = -1 + C;
    if (E == 0 && G_ >= 1 && B_ >= 1 && C + D >= 1 && C >= 1 && D >= 1) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     G = G_;
     H = H_;
     I = I_;
     goto loc_f30;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int A_ = nondet();
    int I_ = D;
    int H_ = C;
    int E_ = 1;
    int D_ = -1 + D;
    int C_ = -2 + D;
    int B_ = 0;
    if (E == 0 && C + D >= 1 && 0 >= 1 + G_ && C >= 1 && D >= 1) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     G = G_;
     H = H_;
     I = I_;
     goto loc_f28;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int A_ = nondet();
    int I_ = D;
    int H_ = C;
    int E_ = 1;
    int D_ = -1 + D;
    int C_ = -2 + D;
    int B_ = 0;
    if (E == 0 && C + D >= 1 && G_ >= 1 && C >= 1 && D >= 1) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     G = G_;
     H = H_;
     I = I_;
     goto loc_f28;
    }
   }
   if (nondet_bool()) {
    int J_ = nondet();
    if (0 >= C) {
     J = J_;
     goto loc_f9;
    }
   }
   if (nondet_bool()) {
    int J_ = nondet();
    if (0 >= D && C >= 1) {
     J = J_;
     goto loc_f9;
    }
   }
  goto end;
 }
 loc_f28:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = 1;
    int E_ = 1;
    if (E == 1 && C >= 1 && C + D >= 1 && D >= 1 && C + D >= H + I && C >= H && D >= I) {
     E = E_;
     F = F_;
     goto loc_f300;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = nondet();
    int E_ = 1;
    int D_ = -1 + C;
    int C_ = -1 + C;
    if (E == 1 && C + D >= 1 && 0 >= 1 + B_ && C >= 1 && D >= 1 && H >= 1 + C) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f30;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = nondet();
    int E_ = 1;
    int D_ = -1 + C;
    int C_ = -1 + C;
    if (E == 1 && C + D >= 1 && B_ >= 1 && C >= 1 && D >= 1 && H >= 1 + C) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f30;
    }
   }
   if (nondet_bool()) {
    int A_ = nondet();
    int E_ = 1;
    int D_ = -1 + D;
    int C_ = -2 + D;
    int B_ = 0;
    if (E == 1 && C >= 1 && C + D >= 1 && D >= 1 && H >= 1 + C) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f28;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = nondet();
    int E_ = 1;
    int D_ = -1 + C;
    int C_ = -1 + C;
    if (E == 1 && C + D >= 1 && 0 >= 1 + B_ && C >= 1 && D >= 1 && C >= H && I >= 1 + D) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f30;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = nondet();
    int E_ = 1;
    int D_ = -1 + C;
    int C_ = -1 + C;
    if (E == 1 && C + D >= 1 && B_ >= 1 && C >= 1 && D >= 1 && C >= H && I >= 1 + D) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f30;
    }
   }
   if (nondet_bool()) {
    int A_ = nondet();
    int E_ = 1;
    int D_ = -1 + D;
    int C_ = -2 + D;
    int B_ = 0;
    if (E == 1 && C >= 1 && C + D >= 1 && D >= 1 && C >= H && I >= 1 + D) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f28;
    }
   }
   if (nondet_bool()) {
    int J_ = nondet();
    if (0 >= C) {
     J = J_;
     goto loc_f9;
    }
   }
   if (nondet_bool()) {
    int J_ = nondet();
    if (0 >= D && C >= 1) {
     J = J_;
     goto loc_f9;
    }
   }
  goto end;
 }
 loc_f29:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = 1;
    int E_ = 1;
    if (E == 1 && C >= 1 && C + D >= 1 && D >= 1 && C + D >= H + I && C >= H && D >= I) {
     E = E_;
     F = F_;
     goto loc_f300;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = nondet();
    int E_ = 1;
    int D_ = -1 + C;
    int C_ = -1 + C;
    if (E == 1 && C + D >= 1 && 0 >= 1 + B_ && C >= 1 && D >= 1 && H >= 1 + C) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f31;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = nondet();
    int E_ = 1;
    int D_ = -1 + C;
    int C_ = -1 + C;
    if (E == 1 && C + D >= 1 && B_ >= 1 && C >= 1 && D >= 1 && H >= 1 + C) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f31;
    }
   }
   if (nondet_bool()) {
    int A_ = nondet();
    int E_ = 1;
    int D_ = -1 + D;
    int C_ = -2 + D;
    int B_ = 0;
    if (E == 1 && C >= 1 && C + D >= 1 && D >= 1 && H >= 1 + C) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f29;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = nondet();
    int E_ = 1;
    int D_ = -1 + C;
    int C_ = -1 + C;
    if (E == 1 && C + D >= 1 && 0 >= 1 + B_ && C >= 1 && D >= 1 && C >= H && I >= 1 + D) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f31;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = nondet();
    int E_ = 1;
    int D_ = -1 + C;
    int C_ = -1 + C;
    if (E == 1 && C + D >= 1 && B_ >= 1 && C >= 1 && D >= 1 && C >= H && I >= 1 + D) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f31;
    }
   }
   if (nondet_bool()) {
    int A_ = nondet();
    int E_ = 1;
    int D_ = -1 + D;
    int C_ = -2 + D;
    int B_ = 0;
    if (E == 1 && C >= 1 && C + D >= 1 && D >= 1 && C >= H && I >= 1 + D) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f29;
    }
   }
   if (nondet_bool()) {
    int J_ = nondet();
    if (0 >= C) {
     J = J_;
     goto loc_f9;
    }
   }
   if (nondet_bool()) {
    int J_ = nondet();
    if (0 >= D && C >= 1) {
     J = J_;
     goto loc_f9;
    }
   }
  goto end;
 }
 loc_f30:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = 1;
    int E_ = 1;
    if (E == 1 && C >= 1 && C + D >= 1 && D >= 1 && C + D >= H + I && C >= H && D >= I) {
     E = E_;
     F = F_;
     goto loc_f300;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = nondet();
    int E_ = 1;
    int D_ = -1 + C;
    int C_ = -1 + C;
    if (E == 1 && C + D >= 1 && 0 >= 1 + B_ && C >= 1 && D >= 1 && H >= 1 + C) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f30;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = nondet();
    int E_ = 1;
    int D_ = -1 + C;
    int C_ = -1 + C;
    if (E == 1 && C + D >= 1 && B_ >= 1 && C >= 1 && D >= 1 && H >= 1 + C) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f30;
    }
   }
   if (nondet_bool()) {
    int A_ = nondet();
    int E_ = 1;
    int D_ = -1 + D;
    int C_ = -2 + D;
    int B_ = 0;
    if (E == 1 && C >= 1 && C + D >= 1 && D >= 1 && H >= 1 + C) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f28;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = nondet();
    int E_ = 1;
    int D_ = -1 + C;
    int C_ = -1 + C;
    if (E == 1 && C + D >= 1 && 0 >= 1 + B_ && C >= 1 && D >= 1 && C >= H && I >= 1 + D) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f30;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = nondet();
    int E_ = 1;
    int D_ = -1 + C;
    int C_ = -1 + C;
    if (E == 1 && C + D >= 1 && B_ >= 1 && C >= 1 && D >= 1 && C >= H && I >= 1 + D) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f30;
    }
   }
   if (nondet_bool()) {
    int A_ = nondet();
    int E_ = 1;
    int D_ = -1 + D;
    int C_ = -2 + D;
    int B_ = 0;
    if (E == 1 && C >= 1 && C + D >= 1 && D >= 1 && C >= H && I >= 1 + D) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f28;
    }
   }
   if (nondet_bool()) {
    int J_ = nondet();
    if (0 >= C) {
     J = J_;
     goto loc_f9;
    }
   }
  goto end;
 }
 loc_f31:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = 1;
    int E_ = 1;
    if (E == 1 && C >= 1 && C + D >= 1 && D >= 1 && C + D >= H + I && C >= H && D >= I) {
     E = E_;
     F = F_;
     goto loc_f300;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = nondet();
    int E_ = 1;
    int D_ = -1 + C;
    int C_ = -1 + C;
    if (E == 1 && C + D >= 1 && 0 >= 1 + B_ && C >= 1 && D >= 1 && H >= 1 + C) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f31;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = nondet();
    int E_ = 1;
    int D_ = -1 + C;
    int C_ = -1 + C;
    if (E == 1 && C + D >= 1 && B_ >= 1 && C >= 1 && D >= 1 && H >= 1 + C) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f31;
    }
   }
   if (nondet_bool()) {
    int A_ = nondet();
    int E_ = 1;
    int D_ = -1 + D;
    int C_ = -2 + D;
    int B_ = 0;
    if (E == 1 && C >= 1 && C + D >= 1 && D >= 1 && H >= 1 + C) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f29;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = nondet();
    int E_ = 1;
    int D_ = -1 + C;
    int C_ = -1 + C;
    if (E == 1 && C + D >= 1 && 0 >= 1 + B_ && C >= 1 && D >= 1 && C >= H && I >= 1 + D) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f31;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = nondet();
    int E_ = 1;
    int D_ = -1 + C;
    int C_ = -1 + C;
    if (E == 1 && C + D >= 1 && B_ >= 1 && C >= 1 && D >= 1 && C >= H && I >= 1 + D) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f31;
    }
   }
   if (nondet_bool()) {
    int A_ = nondet();
    int E_ = 1;
    int D_ = -1 + D;
    int C_ = -2 + D;
    int B_ = 0;
    if (E == 1 && C >= 1 && C + D >= 1 && D >= 1 && C >= H && I >= 1 + D) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f29;
    }
   }
   if (nondet_bool()) {
    int J_ = nondet();
    if (0 >= C) {
     J = J_;
     goto loc_f9;
    }
   }
  goto end;
 }
 loc_f8:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = 0;
    int E_ = 0;
    if (1 >= 0) {
     E = E_;
     F = F_;
     goto loc_f27;
    }
   }
  goto end;
 }
 loc_f9:
 loc_f300:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
