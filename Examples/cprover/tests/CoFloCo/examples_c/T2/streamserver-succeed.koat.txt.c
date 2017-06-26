int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T, int U, int V, int W, int X, int Y) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_ = nondet();
    int X_ = nondet();
    int E_ = nondet();
    int D_ = nondet();
    int A_ = nondet();
    int W_ = 1;
    int V_ = 3;
    int U_ = 0;
    int I_ = 0;
    if (V == 3 && X_ >= 1 && D_ >= 0) {
     A = A_;
     D = D_;
     E = E_;
     I = I_;
     U = U_;
     V = V_;
     W = W_;
     X = X_;
     Y = Y_;
     goto loc_f20;
    }
   }
   if (nondet_bool()) {
    int Y_ = nondet();
    int X_ = nondet();
    int E_ = nondet();
    int D_ = nondet();
    int A_ = nondet();
    int W_ = 1;
    int U_ = 0;
    int I_ = 0;
    if (X_ >= 1 && D_ >= 0 && 2 >= V) {
     A = A_;
     D = D_;
     E = E_;
     I = I_;
     U = U_;
     W = W_;
     X = X_;
     Y = Y_;
     goto loc_f20;
    }
   }
   if (nondet_bool()) {
    int Y_ = nondet();
    int X_ = nondet();
    int E_ = nondet();
    int D_ = nondet();
    int A_ = nondet();
    int W_ = 1;
    int U_ = 0;
    int I_ = 0;
    if (X_ >= 1 && D_ >= 0 && V >= 4) {
     A = A_;
     D = D_;
     E = E_;
     I = I_;
     U = U_;
     W = W_;
     X = X_;
     Y = Y_;
     goto loc_f20;
    }
   }
  goto end;
 }
 loc_f20:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = D;
    int B_ = 1;
    if (A >= 1) {
     B = B_;
     C = C_;
     goto loc_f26;
    }
   }
   if (nondet_bool()) {
    int C_ = D;
    int B_ = 1;
    if (0 >= A && 0 >= D) {
     B = B_;
     C = C_;
     goto loc_f26;
    }
   }
   if (nondet_bool()) {
    int C_ = D;
    int B_ = 0;
    if (0 >= A && D >= 1) {
     B = B_;
     C = C_;
     goto loc_f26;
    }
   }
  goto end;
 }
 loc_f26:
 {
 __cost++;
   if (nondet_bool()) {
    if (C >= E) {
     goto loc_f68;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = nondet();
    if (E >= 1 + C && F_ >= 1) {
     F = F_;
     G = G_;
     goto loc_f33;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = nondet();
    if (0 >= G_ && 0 >= F_ && E >= 1 + C) {
     F = F_;
     G = G_;
     goto loc_f33;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = nondet();
    int C_ = 1 + C;
    if (G_ >= 1 && 0 >= F_ && E >= 1 + C) {
     C = C_;
     F = F_;
     G = G_;
     goto loc_f26;
    }
   }
  goto end;
 }
 loc_f33:
 {
 __cost++;
   if (nondet_bool()) {
    if (H >= 1 + I) {
     goto loc_f39;
    }
   }
   if (nondet_bool()) {
    int J_ = -1;
    if (1 + J == 0 && I >= H) {
     J = J_;
     goto loc_f39;
    }
   }
   if (nondet_bool()) {
    int I_ = 1 + I;
    if (0 >= 2 + J && I >= H) {
     I = I_;
     goto loc_f33;
    }
   }
   if (nondet_bool()) {
    int I_ = 1 + I;
    if (J >= 0 && I >= H) {
     I = I_;
     goto loc_f33;
    }
   }
  goto end;
 }
 loc_f39:
 {
 __cost++;
   if (nondet_bool()) {
    if (I >= H) {
     goto loc_f68;
    }
   }
   if (nondet_bool()) {
    int O_ = nondet();
    int N_ = nondet();
    int M_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    if (0 >= L_ && H >= 1 + I && 0 >= M_) {
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     O = O_;
     goto loc_f52;
    }
   }
   if (nondet_bool()) {
    int P_ = nondet();
    int O_ = nondet();
    int N_ = nondet();
    int M_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    if (0 >= L_ && H >= 1 + I && M_ >= 1 && 0 >= P_) {
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     O = O_;
     P = P_;
     goto loc_f52;
    }
   }
   if (nondet_bool()) {
    int L_ = nondet();
    int K_ = nondet();
    int C_ = 1 + C;
    if (L_ >= 1 && H >= 1 + I) {
     C = C_;
     K = K_;
     L = L_;
     goto loc_f26;
    }
   }
   if (nondet_bool()) {
    int P_ = nondet();
    int M_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    int C_ = 1 + C;
    if (0 >= L_ && H >= 1 + I && M_ >= 1 && P_ >= 1) {
     C = C_;
     K = K_;
     L = L_;
     M = M_;
     P = P_;
     goto loc_f26;
    }
   }
  goto end;
 }
 loc_f52:
 {
 __cost++;
   if (nondet_bool()) {
    int R_ = nondet();
    int Q_ = 3;
    if (Q == 3 && 0 >= R_ && O >= 0) {
     Q = Q_;
     R = R_;
     goto loc_f68;
    }
   }
   if (nondet_bool()) {
    int R_ = nondet();
    int Q_ = 3;
    if (Q == 3 && R_ >= 2 && O >= 0) {
     Q = Q_;
     R = R_;
     goto loc_f68;
    }
   }
   if (nondet_bool()) {
    int S_ = nondet();
    if (2 >= Q && O >= 0) {
     S = S_;
     goto loc_f59;
    }
   }
   if (nondet_bool()) {
    int S_ = nondet();
    if (Q >= 4 && O >= 0) {
     S = S_;
     goto loc_f59;
    }
   }
   if (nondet_bool()) {
    int S_ = nondet();
    int R_ = 1;
    int Q_ = 3;
    if (Q == 3 && O >= 0) {
     Q = Q_;
     R = R_;
     S = S_;
     goto loc_f59;
    }
   }
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (0 >= 1 + O) {
     C = C_;
     goto loc_f26;
    }
   }
  goto end;
 }
 loc_f59:
 {
 __cost++;
   if (nondet_bool()) {
    int T_ = nondet();
    int O_ = nondet();
    if (10 >= S) {
     O = O_;
     T = T_;
     goto loc_f63;
    }
   }
   if (nondet_bool()) {
    int T_ = nondet();
    int O_ = nondet();
    int S_ = 10;
    if (S >= 11) {
     O = O_;
     S = S_;
     T = T_;
     goto loc_f63;
    }
   }
  goto end;
 }
 loc_f63:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (0 >= 1 + O) {
     C = C_;
     goto loc_f26;
    }
   }
   if (nondet_bool()) {
    int U_ = 1 + U;
    int J_ = L;
    int C_ = 1 + C;
    if (O >= 0) {
     C = C_;
     J = J_;
     U = U_;
     goto loc_f26;
    }
   }
  goto end;
 }
 loc_f68:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 0;
    if (0 >= U) {
     A = A_;
     goto loc_f71;
    }
   }
   if (nondet_bool()) {
    int A_ = 1;
    if (U >= 1) {
     A = A_;
     goto loc_f71;
    }
   }
  goto end;
 }
 loc_f71:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f71;
    }
   }
  goto end;
 }
 loc_f73:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f76;
    }
   }
  goto end;
 }
 loc_f76:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
