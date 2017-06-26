int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T, int U, int V, int W, int X, int Y, int Z, int A1) {
 goto loc_start;

 loc_f10:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Y_2 = nondet();
    int Y_3 = nondet();
    int Y_4 = nondet();
    int Y_5 = nondet();
    int Y_6 = nondet();
    int Y_7 = nondet();
    int Y_8 = nondet();
    int Y_9 = nondet();
    int Y_10 = nondet();
    int Y_11 = nondet();
    int I_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int F_ = nondet();
    int E_ = nondet();
    int J_ = 1;
    if (F_ >= Y_0 && D * E_ * Y_0 + Y_0 >= 1 + C && C >= D * E_ * Y_0 && Y_1 >= F_ && D * E_ * Y_1 + Y_1 >= 1 + C && C >= D * E_ * Y_1 && B >= 1 && I_ >= Y_2 && 2 * E_ * Y_2 * Y_3 + Y_2 >= 1 + D * E_ * Y_4 && D * E_ * Y_4 >= 2 * E_ * Y_2 * Y_3 && D * E_ * Y_3 + Y_3 >= 1 + C && C >= D * E_ * Y_3 && D * E_ * Y_4 + Y_4 >= 1 + C && C >= D * E_ * Y_4 && Y_5 >= I_ && 2 * E_ * Y_5 * Y_6 + Y_5 >= 1 + D * E_ * Y_7 && D * E_ * Y_7 >= 2 * E_ * Y_5 * Y_6 && D * E_ * Y_6 + Y_6 >= 1 + C && C >= D * E_ * Y_6 && D * E_ * Y_7 + Y_7 >= 1 + C && C >= D * E_ * Y_7 && H_ >= Y_8 && 2 * E_ * Y_8 + Y_8 >= 1 + D * E_ && D * E_ >= 2 * E_ * Y_8 && Y_9 >= H_ && 2 * E_ * Y_9 + Y_9 >= 1 + D * E_ && D * E_ >= 2 * E_ * Y_9 && G_ >= Y_10 && 3 * Y_10 >= 1 + D && D >= 2 * Y_10 && Y_11 >= G_ && 3 * Y_11 >= 1 + D && D >= 2 * Y_11) {
     E = E_;
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
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
    if (J >= 1 + K && H >= K) {
     goto loc_f23;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int P_ = nondet();
    if (H >= K && P_ >= Y_0 && 3 * Y_0 >= 1 + H && H >= 2 * Y_0 && Y_1 >= P_ && 3 * Y_1 >= 1 + H && H >= 2 * Y_1 && K >= J) {
     P = P_;
     goto loc_f41;
    }
   }
   if (nondet_bool()) {
    int Q_ = G;
    if (K >= 1 + H) {
     Q = Q_;
     goto loc_f53;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int C_ = C * Y_0;
    int B_ = 1 + B;
    if (A >= B) {
     B = B_;
     C = C_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int D_ = 1;
    if (B >= 1 + A) {
     D = D_;
     goto loc_f10;
    }
   }
  goto end;
 }
 loc_f23:
 {
 __cost++;
   if (nondet_bool()) {
    if (K + G >= 2 + L) {
     goto loc_f26;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int P_ = nondet();
    if (P_ >= Y_0 && 3 * Y_0 >= 1 + H && H >= 2 * Y_0 && Y_1 >= P_ && 3 * Y_1 >= 1 + H && H >= 2 * Y_1 && 1 + L >= K + G) {
     P = P_;
     goto loc_f41;
    }
   }
  goto end;
 }
 loc_f26:
 {
 __cost++;
   if (nondet_bool()) {
    int O_ = nondet();
    int N_ = J + M + -K;
    int M_ = M + H;
    if (I >= M) {
     M = M_;
     N = N_;
     O = O_;
     goto loc_f26;
    }
   }
   if (nondet_bool()) {
    int L_ = 2 + L;
    if (M >= 1 + I) {
     L = L_;
     goto loc_f23;
    }
   }
  goto end;
 }
 loc_f41:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int P_ = nondet();
    int J_ = J + -P;
    if (P >= G && P_ >= Y_0 && 3 * Y_0 >= 1 + P && P >= 2 * Y_0 && Y_1 >= P_ && 3 * Y_1 >= 1 + P && P >= 2 * Y_1 && J >= 1 + P) {
     J = J_;
     P = P_;
     goto loc_f41;
    }
   }
   if (nondet_bool()) {
    int K_ = K + G;
    int J_ = J + P;
    if (P >= J && P >= G) {
     J = J_;
     K = K_;
     goto loc_f19;
    }
   }
   if (nondet_bool()) {
    int K_ = K + G;
    int J_ = J + P;
    if (G >= 1 + P) {
     J = J_;
     K = K_;
     goto loc_f19;
    }
   }
  goto end;
 }
 loc_f53:
 {
 __cost++;
   if (nondet_bool()) {
    int V_ = nondet();
    int U_ = nondet();
    int T_ = nondet();
    int S_ = nondet();
    int X_ = 0;
    int W_ = 1;
    int R_ = 2 * Q;
    if (H >= 1 + Q) {
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     V = V_;
     W = W_;
     X = X_;
     goto loc_f63;
    }
   }
   if (nondet_bool()) {
    int D_ = D * E;
    int B_ = -1 + B;
    if (Q >= H) {
     B = B_;
     D = D_;
     goto loc_f10;
    }
   }
  goto end;
 }
 loc_f63:
 {
 __cost++;
   if (nondet_bool()) {
    if (Q >= M) {
     goto loc_f66;
    }
   }
   if (nondet_bool()) {
    int Q_ = R;
    if (M >= 1 + Q) {
     Q = Q_;
     goto loc_f53;
    }
   }
  goto end;
 }
 loc_f66:
 {
 __cost++;
   if (nondet_bool()) {
    if (M + G >= 2 + L) {
     goto loc_f69;
    }
   }
   if (nondet_bool()) {
    int X_ = U * X + V * W + X;
    int W_ = U * W + -V * X + W;
    int T_ = W;
    int M_ = M + G;
    if (1 + L >= M + G) {
     M = M_;
     T = T_;
     W = W_;
     X = X_;
     goto loc_f63;
    }
   }
  goto end;
 }
 loc_f69:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Y_2 = nondet();
    int Y_3 = nondet();
    int A1_ = W * Y_2 + X * Y_3;
    int Z_ = K + Q;
    int Y_ = K;
    int O_ = W * Y_0 + -X * Y_1;
    int K_ = K + R;
    if (I >= K) {
     K = K_;
     O = O_;
     Y = Y_;
     Z = Z_;
     A1 = A1_;
     goto loc_f69;
    }
   }
   if (nondet_bool()) {
    int L_ = 2 + L;
    if (K >= 1 + I) {
     L = L_;
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
