int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 0;
    int B_ = 5;
    int A_ = 50;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f15;
    }
   }
  goto end;
 }
 loc_f15:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 0;
    int D_ = 0;
    if (B >= C) {
     D = D_;
     E = E_;
     goto loc_f19;
    }
   }
   if (nondet_bool()) {
    int M_ = A;
    int G_ = 0;
    int F_ = B;
    if (C >= 1 + B) {
     F = F_;
     G = G_;
     M = M_;
     goto loc_f33;
    }
   }
  goto end;
 }
 loc_f19:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int E_ = 1 + E;
    int D_ = D + Y_0;
    if (E >= 1 + C && B >= E) {
     D = D_;
     E = E_;
     goto loc_f19;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int E_ = 1 + E;
    int D_ = D + Y_0;
    if (B >= E && C >= 1 + E) {
     D = D_;
     E = E_;
     goto loc_f19;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int E_ = 1 + C;
    int D_ = D + Y_0;
    if (C == E && B >= E) {
     D = D_;
     E = E_;
     goto loc_f19;
    }
   }
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (E >= 1 + B) {
     C = C_;
     goto loc_f15;
    }
   }
  goto end;
 }
 loc_f33:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = 1 + G;
    if (F >= 1 + G) {
     H = H_;
     goto loc_f36;
    }
   }
   if (nondet_bool()) {
    int G_ = 1;
    if (G >= F) {
     G = G_;
     goto loc_f66;
    }
   }
  goto end;
 }
 loc_f36:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = nondet();
    int J_ = 0;
    if (F >= H && 0 >= 1 + G) {
     I = I_;
     J = J_;
     goto loc_f41;
    }
   }
   if (nondet_bool()) {
    int I_ = nondet();
    int J_ = 0;
    if (F >= H && G >= 1) {
     I = I_;
     J = J_;
     goto loc_f41;
    }
   }
   if (nondet_bool()) {
    int I_ = nondet();
    int H_ = 1 + H;
    int G_ = 0;
    if (G == 0 && F >= H) {
     G = G_;
     H = H_;
     I = I_;
     goto loc_f36;
    }
   }
   if (nondet_bool()) {
    int H_ = 1 + G;
    if (H >= 1 + F) {
     H = H_;
     goto loc_f50;
    }
   }
  goto end;
 }
 loc_f41:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = nondet();
    int J_ = 1 + J;
    if (G >= 1 + J) {
     I = I_;
     J = J_;
     goto loc_f41;
    }
   }
   if (nondet_bool()) {
    int H_ = 1 + H;
    if (J >= G) {
     H = H_;
     goto loc_f36;
    }
   }
  goto end;
 }
 loc_f50:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = nondet();
    int J_ = 0;
    if (F >= H) {
     I = I_;
     J = J_;
     goto loc_f54;
    }
   }
   if (nondet_bool()) {
    int G_ = 1 + G;
    if (H >= 1 + F) {
     G = G_;
     goto loc_f33;
    }
   }
  goto end;
 }
 loc_f54:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = nondet();
    int J_ = 1 + J;
    if (G >= J) {
     I = I_;
     J = J_;
     goto loc_f54;
    }
   }
   if (nondet_bool()) {
    int H_ = 1 + H;
    if (J >= 1 + G) {
     H = H_;
     goto loc_f50;
    }
   }
  goto end;
 }
 loc_f66:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = nondet();
    int H_ = 0;
    if (F >= G) {
     H = H_;
     I = I_;
     goto loc_f70;
    }
   }
   if (nondet_bool()) {
    int G_ = -1 + F;
    if (G >= 1 + F) {
     G = G_;
     goto loc_f80;
    }
   }
  goto end;
 }
 loc_f70:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = nondet();
    int H_ = 1 + H;
    if (G >= 1 + H) {
     H = H_;
     I = I_;
     goto loc_f70;
    }
   }
   if (nondet_bool()) {
    int G_ = 1 + G;
    if (H >= G) {
     G = G_;
     goto loc_f66;
    }
   }
  goto end;
 }
 loc_f80:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = nondet();
    int H_ = 1 + G;
    if (G >= 0) {
     H = H_;
     I = I_;
     goto loc_f84;
    }
   }
   if (nondet_bool()) {
    int L_ = 0;
    int K_ = 0;
    if (0 >= 1 + G) {
     K = K_;
     L = L_;
     goto loc_f96;
    }
   }
  goto end;
 }
 loc_f84:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = nondet();
    int H_ = 1 + H;
    if (F >= H) {
     H = H_;
     I = I_;
     goto loc_f84;
    }
   }
   if (nondet_bool()) {
    int G_ = -1 + G;
    if (H >= 1 + F) {
     G = G_;
     goto loc_f80;
    }
   }
  goto end;
 }
 loc_f96:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
