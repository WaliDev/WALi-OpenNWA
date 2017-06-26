int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = nondet();
    int G_ = nondet();
    int F_ = nondet();
    int D_ = nondet();
    int E_ = 0;
    int C_ = 1;
    int B_ = 0;
    int A_ = 1;
    if (F_ >= 0 && D_ >= 0 && F_ >= G_ && G_ >= 0 && G_ >= H_ && H_ >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     G = G_;
     H = H_;
     goto loc_f16;
    }
   }
  goto end;
 }
 loc_f16:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = 0;
    int E_ = 1 + E;
    if (0 >= D && 0 >= E && H >= 1) {
     E = E_;
     I = I_;
     goto loc_f25;
    }
   }
   if (nondet_bool()) {
    int I_ = nondet();
    if (I_ >= 0 && 1 >= I_ && 0 >= D && E >= 2 && H >= 1) {
     I = I_;
     goto loc_f25;
    }
   }
   if (nondet_bool()) {
    int I_ = 1;
    int B_ = 1 + B;
    if (E == 1 && 0 >= B && 0 >= D && H >= 1) {
     B = B_;
     I = I_;
     goto loc_f25;
    }
   }
   if (nondet_bool()) {
    int I_ = 0;
    int C_ = -1 + C;
    int B_ = 0;
    if (B == 1 && E == 1 && C >= 1 && 0 >= D && H >= 1) {
     B = B_;
     C = C_;
     I = I_;
     goto loc_f25;
    }
   }
   if (nondet_bool()) {
    int I_ = nondet();
    int D_ = nondet();
    int E_ = 0;
    int C_ = 1 + A;
    int B_ = 0;
    int A_ = 1 + A;
    if (E == 1 && D_ >= 0 && B >= 2 && 1 >= I_ && I_ >= 0 && 0 >= D && H >= 1) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     I = I_;
     goto loc_f25;
    }
   }
   if (nondet_bool()) {
    int I_ = nondet();
    int D_ = nondet();
    int E_ = 0;
    int C_ = 1 + A;
    int B_ = 0;
    int A_ = 1 + A;
    if (B == 1 && E == 1 && D_ >= 0 && 1 >= I_ && I_ >= 0 && 0 >= C && 0 >= D && H >= 1) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     I = I_;
     goto loc_f25;
    }
   }
   if (nondet_bool()) {
    int I_ = nondet();
    int D_ = -1 + D;
    if (I_ >= 0 && 1 >= I_ && D >= 1 && H >= 1) {
     D = D_;
     I = I_;
     goto loc_f25;
    }
   }
   if (nondet_bool()) {
    if (0 >= H) {
     goto loc_f74;
    }
   }
  goto end;
 }
 loc_f25:
 {
 __cost++;
   if (nondet_bool()) {
    int J_ = 0;
    int E_ = 1 + E;
    if (0 >= D && 0 >= E && I >= 1) {
     E = E_;
     J = J_;
     goto loc_f50;
    }
   }
   if (nondet_bool()) {
    int J_ = nondet();
    if (J_ >= 0 && 1 >= J_ && 0 >= D && E >= 2 && I >= 1) {
     J = J_;
     goto loc_f50;
    }
   }
   if (nondet_bool()) {
    int J_ = 1;
    int B_ = 1 + B;
    if (E == 1 && 0 >= B && 0 >= D && I >= 1) {
     B = B_;
     J = J_;
     goto loc_f50;
    }
   }
   if (nondet_bool()) {
    int J_ = 0;
    int C_ = -1 + C;
    int B_ = 0;
    if (B == 1 && E == 1 && C >= 1 && 0 >= D && I >= 1) {
     B = B_;
     C = C_;
     J = J_;
     goto loc_f50;
    }
   }
   if (nondet_bool()) {
    int J_ = nondet();
    int D_ = nondet();
    int E_ = 0;
    int C_ = 1 + A;
    int B_ = 0;
    int A_ = 1 + A;
    if (E == 1 && D_ >= 0 && B >= 2 && 1 >= J_ && J_ >= 0 && 0 >= D && I >= 1) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     J = J_;
     goto loc_f50;
    }
   }
   if (nondet_bool()) {
    int J_ = nondet();
    int D_ = nondet();
    int E_ = 0;
    int C_ = 1 + A;
    int B_ = 0;
    int A_ = 1 + A;
    if (B == 1 && E == 1 && D_ >= 0 && 1 >= J_ && J_ >= 0 && 0 >= C && 0 >= D && I >= 1) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     J = J_;
     goto loc_f50;
    }
   }
   if (nondet_bool()) {
    int J_ = nondet();
    int D_ = -1 + D;
    if (J_ >= 0 && 1 >= J_ && D >= 1 && I >= 1) {
     D = D_;
     J = J_;
     goto loc_f50;
    }
   }
   if (nondet_bool()) {
    if (0 >= I && 1 + F >= A) {
     goto loc_f16;
    }
   }
  goto end;
 }
 loc_f50:
 {
 __cost++;
   if (nondet_bool()) {
    if (H >= G && 1 + F >= A && J >= 1) {
     goto loc_f16;
    }
   }
   if (nondet_bool()) {
    int H_ = 1 + H;
    if (G >= 1 + H && 1 + F >= A && J >= 1) {
     H = H_;
     goto loc_f16;
    }
   }
   if (nondet_bool()) {
    int H_ = -1 + H;
    if (0 >= J && 1 + F >= A) {
     H = H_;
     goto loc_f16;
    }
   }
  goto end;
 }
 loc_f74:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
