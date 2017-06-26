int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R) {
 goto loc_f2;

 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 2) {
     goto loc_f5;
    }
   }
  goto end;
 }
 loc_f26:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (A >= D) {
     D = D_;
     goto loc_f26;
    }
   }
   if (nondet_bool()) {
    if (D >= 1 + A) {
     goto loc_f32;
    }
   }
  goto end;
 }
 loc_f32:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = 0;
    int H_ = 0;
    int D_ = 1 + D;
    if (A >= D) {
     D = D_;
     H = H_;
     I = I_;
     goto loc_f32;
    }
   }
   if (nondet_bool()) {
    int H_ = nondet();
    int J_ = J + H_*H_;
    int I_ = H_*H_;
    int D_ = 1 + D;
    if (A >= D && 0 >= 1 + H_) {
     D = D_;
     H = H_;
     I = I_;
     J = J_;
     goto loc_f32;
    }
   }
   if (nondet_bool()) {
    int H_ = nondet();
    int J_ = J + H_*H_;
    int I_ = H_*H_;
    int D_ = 1 + D;
    if (A >= D && H_ >= 1) {
     D = D_;
     H = H_;
     I = I_;
     J = J_;
     goto loc_f32;
    }
   }
   if (nondet_bool()) {
    int O_ = nondet();
    int N_ = nondet();
    int M_ = nondet();
    if (D >= 1 + A) {
     M = M_;
     N = N_;
     O = O_;
     goto loc_f52;
    }
   }
   if (nondet_bool()) {
    int Q_ = nondet();
    int P_ = nondet();
    int O_ = -Q_;
    if (D >= 1 + A) {
     O = O_;
     P = P_;
     Q = Q_;
     goto loc_f52;
    }
   }
  goto end;
 }
 loc_f5:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 0;
    if (A >= 1 + B) {
     C = C_;
     goto loc_f9;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (0 >= 1 + Y_0 && B >= A) {
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (Y_0 >= 1 && B >= A) {
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    if (B >= A) {
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f52:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= K) {
     goto loc_f55;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (K >= 1 + A) {
     B = B_;
     goto loc_f5;
    }
   }
  goto end;
 }
 loc_f55:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int J_ = J + Y_0 * Y_1;
    int D_ = 1 + D;
    if (A >= D) {
     D = D_;
     J = J_;
     goto loc_f55;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Y_2 = nondet();
    int L_ = nondet();
    if (D >= 1 + A && L_ >= Y_0 && Y_0 * Y_1 + Y_0 >= 1 + J && J >= Y_0 * Y_1 && Y_2 >= L_ && Y_1 * Y_2 + Y_2 >= 1 + J && J >= Y_1 * Y_2) {
     L = L_;
     goto loc_f62;
    }
   }
  goto end;
 }
 loc_f62:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (A >= D) {
     D = D_;
     goto loc_f62;
    }
   }
   if (nondet_bool()) {
    int K_ = 1 + K;
    if (D >= 1 + A) {
     K = K_;
     goto loc_f52;
    }
   }
  goto end;
 }
 loc_f9:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = nondet();
    int E_ = C;
    int D_ = 1 + D;
    if (A >= D && C >= 1 + G_) {
     D = D_;
     E = E_;
     F = F_;
     G = G_;
     goto loc_f9;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = nondet();
    int C_ = nondet();
    int E_ = C;
    int D_ = 1 + D;
    if (A >= D && G_ >= C) {
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     G = G_;
     goto loc_f9;
    }
   }
   if (nondet_bool()) {
    int R_ = 0;
    int C_ = 0;
    int B_ = 1 + B;
    if (C == 0 && D >= 1 + A) {
     B = B_;
     C = C_;
     R = R_;
     goto loc_f5;
    }
   }
   if (nondet_bool()) {
    if (D >= 1 + A && 0 >= 1 + C) {
     goto loc_f26;
    }
   }
   if (nondet_bool()) {
    if (D >= 1 + A && C >= 1) {
     goto loc_f26;
    }
   }
  goto end;
 }
 loc_f1:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
