int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = nondet();
    int C_ = nondet();
    int J_ = 0;
    int I_ = 0;
    int H_ = 0;
    int G_ = 0;
    int F_ = 0;
    int D_ = 0;
    int B_ = 4;
    int A_ = 1;
    if (E_ >= 0 && C_ >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     goto loc_f15;
    }
   }
  goto end;
 }
 loc_f15:
 {
 __cost++;
   if (nondet_bool()) {
    int J_ = 0;
    int I_ = 0;
    int H_ = 0;
    int G_ = 0;
    int F_ = 0;
    int D_ = 1 + D;
    if (I == 0 && B >= 1 && 0 >= C && 0 >= D) {
     D = D_;
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     goto loc_f36;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int J_ = 0;
    int I_ = 0;
    int H_ = 0;
    int G_ = 0;
    int D_ = 0;
    int B_ = -1 + B;
    if (I == 0 && 1 >= F_ && B >= 1 && F_ >= 0 && 0 >= C && D >= 1) {
     B = B_;
     D = D_;
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     goto loc_f36;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int C_ = nondet();
    int J_ = 0;
    int I_ = 0;
    int H_ = 0;
    int G_ = 0;
    int D_ = 0;
    int B_ = 4 + A;
    int A_ = 1 + A;
    if (I == 0 && C_ >= 0 && 0 >= B && 1 >= F_ && F_ >= 0 && 0 >= C) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     goto loc_f36;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int J_ = 0;
    int I_ = 0;
    int H_ = 0;
    int G_ = 0;
    int C_ = -1 + C;
    if (I == 0 && F_ >= 0 && 1 >= F_ && C >= 1) {
     C = C_;
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     goto loc_f36;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + I) {
     goto loc_f81;
    }
   }
   if (nondet_bool()) {
    if (I >= 1) {
     goto loc_f81;
    }
   }
  goto end;
 }
 loc_f36:
 {
 __cost++;
   if (nondet_bool()) {
    if (J >= 1 + E && 0 >= H) {
     goto loc_f77;
    }
   }
   if (nondet_bool()) {
    if (H >= 1) {
     goto loc_f77;
    }
   }
   if (nondet_bool()) {
    int G_ = 0;
    int D_ = 1 + D;
    if (B >= 1 && 0 >= C && 0 >= D && 0 >= H && E >= J) {
     D = D_;
     G = G_;
     goto loc_f48;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int D_ = 0;
    int B_ = -1 + B;
    if (1 >= G_ && B >= 1 && G_ >= 0 && 0 >= C && D >= 1 && 0 >= H && E >= J) {
     B = B_;
     D = D_;
     G = G_;
     goto loc_f48;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int C_ = nondet();
    int D_ = 0;
    int B_ = 4 + A;
    int A_ = 1 + A;
    if (C_ >= 0 && 0 >= B && 1 >= G_ && G_ >= 0 && 0 >= C && 0 >= H && E >= J) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     G = G_;
     goto loc_f48;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int C_ = -1 + C;
    if (G_ >= 0 && 1 >= G_ && C >= 1 && 0 >= H && E >= J) {
     C = C_;
     G = G_;
     goto loc_f48;
    }
   }
  goto end;
 }
 loc_f48:
 {
 __cost++;
   if (nondet_bool()) {
    int J_ = 1 + J;
    if (0 >= G) {
     J = J_;
     goto loc_f36;
    }
   }
   if (nondet_bool()) {
    int J_ = 1 + J;
    if (G >= 1 && F >= 1) {
     J = J_;
     goto loc_f36;
    }
   }
   if (nondet_bool()) {
    int J_ = 1 + J;
    int H_ = 0;
    int D_ = 1 + D;
    if (B >= 1 && 0 >= C && 0 >= D && 1 + E >= A && 0 >= F && G >= 1) {
     D = D_;
     H = H_;
     J = J_;
     goto loc_f36;
    }
   }
   if (nondet_bool()) {
    int H_ = nondet();
    int J_ = 1 + J;
    int D_ = 0;
    int B_ = -1 + B;
    if (1 >= H_ && B >= 1 && H_ >= 0 && 0 >= C && D >= 1 && 1 + E >= A && 0 >= F && G >= 1) {
     B = B_;
     D = D_;
     H = H_;
     J = J_;
     goto loc_f36;
    }
   }
   if (nondet_bool()) {
    int H_ = nondet();
    int C_ = nondet();
    int J_ = 1 + J;
    int D_ = 0;
    int B_ = 4 + A;
    int A_ = 1 + A;
    if (C_ >= 0 && 0 >= B && 1 >= H_ && H_ >= 0 && 0 >= C && E >= A && 0 >= F && G >= 1) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     H = H_;
     J = J_;
     goto loc_f36;
    }
   }
   if (nondet_bool()) {
    int H_ = nondet();
    int J_ = 1 + J;
    int C_ = -1 + C;
    if (H_ >= 0 && 1 >= H_ && C >= 1 && 1 + E >= A && 0 >= F && G >= 1) {
     C = C_;
     H = H_;
     J = J_;
     goto loc_f36;
    }
   }
  goto end;
 }
 loc_f77:
 {
 __cost++;
   if (nondet_bool()) {
    if (H >= 1) {
     goto loc_f15;
    }
   }
   if (nondet_bool()) {
    int I_ = 1;
    if (0 >= H) {
     I = I_;
     goto loc_f15;
    }
   }
  goto end;
 }
 loc_f81:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
