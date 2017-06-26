int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T, int U, int V, int W, int X, int Y, int Z, int A1) {
 goto loc_start;

 loc_f10:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = nondet();
    int G_ = nondet();
    int F_ = nondet();
    int E_ = nondet();
    int D_ = nondet();
    int I_ = 1;
    if (B >= 1) {
     D = D_;
     E = E_;
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     goto loc_f19;
    }
   }
   if (nondet_bool()) {
    if (0 >= B) {
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f19:
 {
 __cost++;
   if (nondet_bool()) {
    if (I >= 1 + J && G >= J) {
     goto loc_f23;
    }
   }
   if (nondet_bool()) {
    int O_ = nondet();
    if (J >= I && G >= J) {
     O = O_;
     goto loc_f41;
    }
   }
   if (nondet_bool()) {
    int P_ = F;
    if (J >= 1 + G) {
     P = P_;
     goto loc_f53;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = nondet();
    int B_ = 1 + B;
    if (A >= B) {
     B = B_;
     C = C_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int A1_ = 1;
    if (B >= 1 + A) {
     A1 = A1_;
     goto loc_f10;
    }
   }
  goto end;
 }
 loc_f23:
 {
 __cost++;
   if (nondet_bool()) {
    if (J + F >= 2 + K) {
     goto loc_f26;
    }
   }
   if (nondet_bool()) {
    int O_ = nondet();
    if (1 + K >= J + F) {
     O = O_;
     goto loc_f41;
    }
   }
  goto end;
 }
 loc_f26:
 {
 __cost++;
   if (nondet_bool()) {
    int N_ = nondet();
    int M_ = I + L + -J;
    int L_ = L + G;
    if (H >= L) {
     L = L_;
     M = M_;
     N = N_;
     goto loc_f26;
    }
   }
   if (nondet_bool()) {
    int K_ = 2 + K;
    if (L >= 1 + H) {
     K = K_;
     goto loc_f23;
    }
   }
  goto end;
 }
 loc_f41:
 {
 __cost++;
   if (nondet_bool()) {
    int O_ = nondet();
    int I_ = I + -O;
    if (I >= 1 + O && O >= F) {
     I = I_;
     O = O_;
     goto loc_f41;
    }
   }
   if (nondet_bool()) {
    int J_ = J + F;
    int I_ = I + O;
    if (O >= I && O >= F) {
     I = I_;
     J = J_;
     goto loc_f19;
    }
   }
   if (nondet_bool()) {
    int J_ = J + F;
    int I_ = I + O;
    if (F >= 1 + O) {
     I = I_;
     J = J_;
     goto loc_f19;
    }
   }
  goto end;
 }
 loc_f53:
 {
 __cost++;
   if (nondet_bool()) {
    int U_ = nondet();
    int T_ = nondet();
    int S_ = nondet();
    int R_ = nondet();
    int Q_ = nondet();
    int W_ = 0;
    int V_ = 1;
    if (G >= 1 + P) {
     Q = Q_;
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     V = V_;
     W = W_;
     goto loc_f63;
    }
   }
   if (nondet_bool()) {
    int A1_ = nondet();
    int B_ = -1 + B;
    if (P >= G) {
     B = B_;
     A1 = A1_;
     goto loc_f10;
    }
   }
  goto end;
 }
 loc_f63:
 {
 __cost++;
   if (nondet_bool()) {
    if (P >= L) {
     goto loc_f66;
    }
   }
   if (nondet_bool()) {
    int P_ = Q;
    if (L >= 1 + P) {
     P = P_;
     goto loc_f53;
    }
   }
  goto end;
 }
 loc_f66:
 {
 __cost++;
   if (nondet_bool()) {
    if (L + F >= 2 + K) {
     goto loc_f69;
    }
   }
   if (nondet_bool()) {
    int W_ = nondet();
    int V_ = nondet();
    int S_ = V;
    int L_ = L + F;
    if (1 + K >= L + F) {
     L = L_;
     S = S_;
     V = V_;
     W = W_;
     goto loc_f63;
    }
   }
  goto end;
 }
 loc_f69:
 {
 __cost++;
   if (nondet_bool()) {
    int Z_ = nondet();
    int N_ = nondet();
    int Y_ = J + P;
    int X_ = J;
    int J_ = J + Q;
    if (H >= J) {
     J = J_;
     N = N_;
     X = X_;
     Y = Y_;
     Z = Z_;
     goto loc_f69;
    }
   }
   if (nondet_bool()) {
    int K_ = 2 + K;
    if (J >= 1 + H) {
     K = K_;
     goto loc_f66;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f2;
    }
   }
  goto end;
 }
 loc_f1:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
