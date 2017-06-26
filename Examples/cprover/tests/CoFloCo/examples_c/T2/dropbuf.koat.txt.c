int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T, int U, int V, int W, int X) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int R_ = nondet();
    int H_ = nondet();
    int X_ = 1;
    int Q_ = 0;
    int G_ = 0;
    int C_ = 0;
    if (1 >= 0) {
     C = C_;
     G = G_;
     H = H_;
     Q = Q_;
     R = R_;
     X = X_;
     goto loc_f15;
    }
   }
   if (nondet_bool()) {
    int X_ = nondet();
    int R_ = nondet();
    int H_ = nondet();
    int Q_ = 0;
    int G_ = 0;
    int C_ = 1;
    if (0 >= X_) {
     C = C_;
     G = G_;
     H = H_;
     Q = Q_;
     R = R_;
     X = X_;
     goto loc_f36;
    }
   }
   if (nondet_bool()) {
    int X_ = nondet();
    int R_ = nondet();
    int H_ = nondet();
    int Q_ = 0;
    int G_ = 0;
    int C_ = 1;
    if (X_ >= 2) {
     C = C_;
     G = G_;
     H = H_;
     Q = Q_;
     R = R_;
     X = X_;
     goto loc_f36;
    }
   }
  goto end;
 }
 loc_f15:
 {
 __cost++;
   if (nondet_bool()) {
    int W_ = nondet();
    int V_ = nondet();
    int U_ = nondet();
    int T_ = nondet();
    int S_ = nondet();
    if (R >= 1 + C && 0 >= 1 + W_) {
     S = S_;
     T = T_;
     U = U_;
     V = V_;
     W = W_;
     goto loc_f25;
    }
   }
   if (nondet_bool()) {
    int W_ = nondet();
    int V_ = nondet();
    int U_ = nondet();
    int T_ = nondet();
    int S_ = nondet();
    if (R >= 1 + C && W_ >= 1) {
     S = S_;
     T = T_;
     U = U_;
     V = V_;
     W = W_;
     goto loc_f25;
    }
   }
   if (nondet_bool()) {
    int T_ = nondet();
    int S_ = nondet();
    int W_ = 0;
    int V_ = 0;
    int U_ = 0;
    int C_ = 1 + C;
    if (R >= 1 + C) {
     C = C_;
     S = S_;
     T = T_;
     U = U_;
     V = V_;
     W = W_;
     goto loc_f15;
    }
   }
   if (nondet_bool()) {
    if (C >= R) {
     goto loc_f31;
    }
   }
  goto end;
 }
 loc_f25:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (A >= 1 + B) {
     C = C_;
     goto loc_f15;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int F_ = 1;
    int E_ = 0;
    int C_ = 1 + C;
    if (B >= A) {
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     goto loc_f15;
    }
   }
  goto end;
 }
 loc_f31:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f31;
    }
   }
  goto end;
 }
 loc_f33:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = 0;
    int C_ = 1;
    if (1 >= 0) {
     C = C_;
     G = G_;
     goto loc_f36;
    }
   }
  goto end;
 }
 loc_f36:
 {
 __cost++;
   if (nondet_bool()) {
    int M_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    if (H >= C) {
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f45;
    }
   }
   if (nondet_bool()) {
    int Q_ = 0;
    if (C >= 1 + H) {
     Q = Q_;
     goto loc_f61;
    }
   }
  goto end;
 }
 loc_f45:
 {
 __cost++;
   if (nondet_bool()) {
    int N_ = M;
    if (0 >= 1 + M) {
     N = N_;
     goto loc_f46;
    }
   }
   if (nondet_bool()) {
    int N_ = M;
    if (M >= 1) {
     N = N_;
     goto loc_f46;
    }
   }
   if (nondet_bool()) {
    int N_ = 0;
    int M_ = 0;
    int C_ = 1 + C;
    if (M == 0) {
     C = C_;
     M = M_;
     N = N_;
     goto loc_f36;
    }
   }
  goto end;
 }
 loc_f46:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (A >= 1 + B) {
     C = C_;
     goto loc_f36;
    }
   }
   if (nondet_bool()) {
    int O_ = nondet();
    int D_ = nondet();
    int E_ = 0;
    int C_ = 1 + C;
    if (B >= A) {
     C = C_;
     D = D_;
     E = E_;
     O = O_;
     goto loc_f36;
    }
   }
   if (nondet_bool()) {
    int P_ = nondet();
    int M_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    if (B >= A) {
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     P = P_;
     goto loc_f45;
    }
   }
  goto end;
 }
 loc_f61:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f61;
    }
   }
  goto end;
 }
 loc_f63:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f65;
    }
   }
  goto end;
 }
 loc_f65:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
