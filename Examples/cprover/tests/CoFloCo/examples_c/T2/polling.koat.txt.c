int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T, int U, int V, int W, int X, int Y, int Z, int A1, int B1, int C1, int D1, int E1) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int D1_ = nondet();
    int C1_ = nondet();
    int B1_ = nondet();
    int A1_ = nondet();
    int Z_ = nondet();
    int Y_ = nondet();
    int V_ = nondet();
    int C_ = 1;
    if (0 >= B1_) {
     C = C_;
     V = V_;
     Y = Y_;
     Z = Z_;
     A1 = A1_;
     B1 = B1_;
     C1 = C1_;
     D1 = D1_;
     goto loc_f91;
    }
   }
   if (nondet_bool()) {
    int E1_ = nondet();
    int D1_ = nondet();
    int C1_ = nondet();
    int B1_ = nondet();
    int A1_ = nondet();
    int Y_ = nondet();
    int V_ = nondet();
    int C_ = nondet();
    int B_ = nondet();
    int Z_ = 5;
    if (4 >= E1_ && B1_ >= 1) {
     B = B_;
     C = C_;
     V = V_;
     Y = Y_;
     Z = Z_;
     A1 = A1_;
     B1 = B1_;
     C1 = C1_;
     D1 = D1_;
     E1 = E1_;
     goto loc_f34;
    }
   }
   if (nondet_bool()) {
    int E1_ = nondet();
    int D1_ = nondet();
    int C1_ = nondet();
    int B1_ = nondet();
    int A1_ = nondet();
    int Z_ = nondet();
    int Y_ = nondet();
    int V_ = nondet();
    int C_ = nondet();
    int B_ = nondet();
    if (E1_ >= 5 && 20 >= E1_ && B1_ >= 1) {
     B = B_;
     C = C_;
     V = V_;
     Y = Y_;
     Z = Z_;
     A1 = A1_;
     B1 = B1_;
     C1 = C1_;
     D1 = D1_;
     E1 = E1_;
     goto loc_f34;
    }
   }
   if (nondet_bool()) {
    int E1_ = nondet();
    int D1_ = nondet();
    int C1_ = nondet();
    int B1_ = nondet();
    int A1_ = nondet();
    int Y_ = nondet();
    int V_ = nondet();
    int C_ = nondet();
    int B_ = nondet();
    int Z_ = 20;
    if (E1_ >= 21 && B1_ >= 1) {
     B = B_;
     C = C_;
     V = V_;
     Y = Y_;
     Z = Z_;
     A1 = A1_;
     B1 = B1_;
     C1 = C1_;
     D1 = D1_;
     E1 = E1_;
     goto loc_f34;
    }
   }
  goto end;
 }
 loc_f34:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = nondet();
    int D_ = B;
    int C_ = 1;
    if (0 >= 1 + B) {
     C = C_;
     D = D_;
     E = E_;
     goto loc_f39;
    }
   }
   if (nondet_bool()) {
    int E_ = nondet();
    int D_ = B;
    int C_ = 1;
    if (B >= 1) {
     C = C_;
     D = D_;
     E = E_;
     goto loc_f39;
    }
   }
   if (nondet_bool()) {
    int D_ = 0;
    int C_ = 1;
    int B_ = 0;
    if (B == 0) {
     B = B_;
     C = C_;
     D = D_;
     goto loc_f91;
    }
   }
  goto end;
 }
 loc_f39:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = nondet();
    int G_ = 10;
    int F_ = E;
    if (0 >= 1 + E) {
     F = F_;
     G = G_;
     H = H_;
     goto loc_f44;
    }
   }
   if (nondet_bool()) {
    int H_ = nondet();
    int G_ = 10;
    int F_ = E;
    if (E >= 1) {
     F = F_;
     G = G_;
     H = H_;
     goto loc_f44;
    }
   }
   if (nondet_bool()) {
    int F_ = 0;
    int E_ = 0;
    int C_ = 1;
    if (E == 0) {
     C = C_;
     E = E_;
     F = F_;
     goto loc_f91;
    }
   }
  goto end;
 }
 loc_f44:
 {
 __cost++;
   if (nondet_bool()) {
    int X_ = 0;
    int W_ = 0;
    int N_ = H;
    if (0 >= 1 + V) {
     N = N_;
     W = W_;
     X = X_;
     goto loc_f51;
    }
   }
   if (nondet_bool()) {
    int X_ = 0;
    int W_ = 0;
    int N_ = H;
    if (V >= 1) {
     N = N_;
     W = W_;
     X = X_;
     goto loc_f51;
    }
   }
   if (nondet_bool()) {
    int H_ = nondet();
    int X_ = 0;
    int W_ = 0;
    int V_ = 0;
    int N_ = H;
    if (V == 0) {
     H = H_;
     N = N_;
     V = V_;
     W = W_;
     X = X_;
     goto loc_f44;
    }
   }
   if (nondet_bool()) {
    int X_ = nondet();
    int W_ = nondet();
    int N_ = H;
    int C_ = 1;
    if (0 >= 1 + X_) {
     C = C_;
     N = N_;
     W = W_;
     X = X_;
     goto loc_f91;
    }
   }
   if (nondet_bool()) {
    int X_ = nondet();
    int W_ = nondet();
    int N_ = H;
    int C_ = 1;
    if (X_ >= 1) {
     C = C_;
     N = N_;
     W = W_;
     X = X_;
     goto loc_f91;
    }
   }
  goto end;
 }
 loc_f51:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = nondet();
    if (N >= 0) {
     H = H_;
     goto loc_f44;
    }
   }
   if (nondet_bool()) {
    int H_ = nondet();
    if (0 >= 2 + N) {
     H = H_;
     goto loc_f44;
    }
   }
   if (nondet_bool()) {
    int U_ = nondet();
    int T_ = nondet();
    int I_ = nondet();
    int N_ = -1;
    if (1 + N == 0 && 0 >= 1 + U_) {
     I = I_;
     N = N_;
     T = T_;
     U = U_;
     goto loc_f59;
    }
   }
   if (nondet_bool()) {
    int U_ = nondet();
    int T_ = nondet();
    int I_ = nondet();
    int N_ = -1;
    if (1 + N == 0 && U_ >= 1) {
     I = I_;
     N = N_;
     T = T_;
     U = U_;
     goto loc_f59;
    }
   }
   if (nondet_bool()) {
    int H_ = nondet();
    int U_ = 0;
    int T_ = 0;
    int N_ = -1;
    if (1 + N == 0) {
     H = H_;
     N = N_;
     T = T_;
     U = U_;
     goto loc_f44;
    }
   }
  goto end;
 }
 loc_f59:
 {
 __cost++;
   if (nondet_bool()) {
    int L_ = nondet();
    int K_ = 0;
    int J_ = I;
    if (0 >= 1 + I) {
     J = J_;
     K = K_;
     L = L_;
     goto loc_f64;
    }
   }
   if (nondet_bool()) {
    int L_ = nondet();
    int K_ = 0;
    int J_ = I;
    if (I >= 1) {
     J = J_;
     K = K_;
     L = L_;
     goto loc_f64;
    }
   }
   if (nondet_bool()) {
    int H_ = nondet();
    int J_ = 0;
    int I_ = 0;
    if (I == 0) {
     H = H_;
     I = I_;
     J = J_;
     goto loc_f44;
    }
   }
  goto end;
 }
 loc_f64:
 {
 __cost++;
   if (nondet_bool()) {
    int Q_ = nondet();
    int P_ = nondet();
    int O_ = L;
    if (0 >= 1 + Q_) {
     O = O_;
     P = P_;
     Q = Q_;
     goto loc_f82;
    }
   }
   if (nondet_bool()) {
    int Q_ = nondet();
    int P_ = nondet();
    int O_ = L;
    if (Q_ >= 1) {
     O = O_;
     P = P_;
     Q = Q_;
     goto loc_f82;
    }
   }
   if (nondet_bool()) {
    int S_ = 0;
    int R_ = 0;
    int Q_ = 0;
    int P_ = 0;
    int O_ = L;
    if (1 >= 0) {
     O = O_;
     P = P_;
     Q = Q_;
     R = R_;
     S = S_;
     goto loc_f78;
    }
   }
   if (nondet_bool()) {
    int S_ = nondet();
    int R_ = nondet();
    int Q_ = 0;
    int P_ = 0;
    int O_ = L;
    if (0 >= 1 + S_) {
     O = O_;
     P = P_;
     Q = Q_;
     R = R_;
     S = S_;
     goto loc_f75;
    }
   }
   if (nondet_bool()) {
    int S_ = nondet();
    int R_ = nondet();
    int Q_ = 0;
    int P_ = 0;
    int O_ = L;
    if (S_ >= 1) {
     O = O_;
     P = P_;
     Q = Q_;
     R = R_;
     S = S_;
     goto loc_f75;
    }
   }
  goto end;
 }
 loc_f75:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + M) {
     goto loc_f76;
    }
   }
   if (nondet_bool()) {
    if (M >= 1) {
     goto loc_f76;
    }
   }
   if (nondet_bool()) {
    int M_ = 0;
    if (M == 0) {
     M = M_;
     goto loc_f78;
    }
   }
  goto end;
 }
 loc_f76:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 0;
    if (A == 0) {
     A = A_;
     goto loc_f78;
    }
   }
   if (nondet_bool()) {
    int K_ = 1;
    if (0 >= 1 + A) {
     K = K_;
     goto loc_f78;
    }
   }
   if (nondet_bool()) {
    int K_ = 1;
    if (A >= 1) {
     K = K_;
     goto loc_f78;
    }
   }
  goto end;
 }
 loc_f78:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + K) {
     goto loc_f82;
    }
   }
   if (nondet_bool()) {
    if (K >= 1) {
     goto loc_f82;
    }
   }
   if (nondet_bool()) {
    int K_ = 0;
    if (K == 0) {
     K = K_;
     goto loc_f82;
    }
   }
  goto end;
 }
 loc_f82:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = nondet();
    int K_ = 0;
    if (K == 0) {
     H = H_;
     K = K_;
     goto loc_f44;
    }
   }
   if (nondet_bool()) {
    int H_ = nondet();
    if (0 >= 1 + K) {
     H = H_;
     goto loc_f44;
    }
   }
   if (nondet_bool()) {
    int H_ = nondet();
    if (K >= 1) {
     H = H_;
     goto loc_f44;
    }
   }
  goto end;
 }
 loc_f91:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f91;
    }
   }
  goto end;
 }
 loc_f93:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f95;
    }
   }
  goto end;
 }
 loc_f95:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
