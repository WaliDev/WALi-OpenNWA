int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T, int U, int V, int W, int X, int Y, int Z, int A1, int B1, int C1, int D1, int E1, int F1, int G1, int H1, int I1, int J1, int K1, int L1) {
 goto loc_f9;

 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    int U_ = nondet();
    int S_ = nondet();
    int V_ = A;
    int T_ = S;
    int R_ = S;
    int A_ = 1 + A;
    if (A >= 0 && Q >= 1 + A) {
     A = A_;
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     V = V_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int Z_ = nondet();
    int Y_ = nondet();
    int X_ = nondet();
    int W_ = nondet();
    int T_ = nondet();
    int S_ = nondet();
    int R_ = nondet();
    int Q_ = nondet();
    int C_ = nondet();
    int M_ = R;
    int D_ = R;
    int B_ = 0;
    int A_ = G;
    if (B == 0 && G >= C_ && Z_ >= C_ && C_ >= 2 && A >= 0 && A >= Q) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     M = M_;
     Q = Q_;
     R = R_;
     S = S_;
     T = T_;
     W = W_;
     X = X_;
     Y = Y_;
     Z = Z_;
     goto loc_f16;
    }
   }
  goto end;
 }
 loc_f12:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int H_ = nondet();
    int E_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int I_ = G;
    int F_ = 1 + G;
    int B_ = 1;
    if (B == 1 && Y_0 >= C_ && C_ >= 2 && A >= 0) {
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     H = H_;
     I = I_;
     goto loc_f16;
    }
   }
  goto end;
 }
 loc_f13:
 {
 __cost++;
   if (nondet_bool()) {
    int K_ = nondet();
    int J_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    if (I >= 0 && C_ >= 2) {
     C = C_;
     D = D_;
     J = J_;
     K = K_;
     goto loc_f16;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int C_ = nondet();
    int G1_ = D;
    int F1_ = D;
    int E1_ = D;
    int D1_ = 0;
    int C1_ = D;
    int B1_ = 0;
    int A1_ = H1;
    int M_ = 0;
    int I_ = 0;
    int B_ = 1 + H1;
    if (B == 1 && I == 0 && M == 0 && C_ >= 2 && Y_0 >= 2) {
     B = B_;
     C = C_;
     I = I_;
     M = M_;
     A1 = A1_;
     B1 = B1_;
     C1 = C1_;
     D1 = D1_;
     E1 = E1_;
     F1 = F1_;
     G1 = G1_;
     goto loc_f8;
    }
   }
  goto end;
 }
 loc_f16:
 {
 __cost++;
   if (nondet_bool()) {
    int N_ = nondet();
    int E_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int P_ = -1 + G;
    int O_ = 1 + B;
    int L_ = M;
    int G_ = -1 + G;
    int B_ = 1 + B;
    if (C_ >= 2 && G >= 0 && B >= 0) {
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     G = G_;
     L = L_;
     N = N_;
     O = O_;
     P = P_;
     goto loc_f16;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int C_ = nondet();
    int G1_ = D;
    int F1_ = D;
    int E1_ = D;
    int D1_ = 0;
    int C1_ = D;
    int B1_ = 0;
    int A1_ = H1;
    int M_ = 0;
    int B_ = 1 + H1;
    if (M == 0 && G >= 0 && B >= 0 && C_ >= 2 && Y_0 >= 2) {
     B = B_;
     C = C_;
     M = M_;
     A1 = A1_;
     B1 = B1_;
     C1 = C1_;
     D1 = D1_;
     E1 = E1_;
     F1 = F1_;
     G1 = G1_;
     goto loc_f8;
    }
   }
  goto end;
 }
 loc_f7:
 {
 __cost++;
   if (nondet_bool()) {
    int E1_ = nondet();
    int C1_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int F1_ = G1;
    int D1_ = 0;
    int B1_ = 0;
    int M_ = 0;
    if (B1 == 0 && A1 >= 0 && C_ >= 2) {
     C = C_;
     D = D_;
     M = M_;
     B1 = B1_;
     C1 = C1_;
     D1 = D1_;
     E1 = E1_;
     F1 = F1_;
     goto loc_f8;
    }
   }
   if (nondet_bool()) {
    int G1_ = nondet();
    int F1_ = nondet();
    int E1_ = nondet();
    int D1_ = nondet();
    int C1_ = nondet();
    int B1_ = nondet();
    int Y_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    if (B1 == G1 && A1 >= 0 && C_ >= 2) {
     C = C_;
     D = D_;
     Y = Y_;
     B1 = B1_;
     C1 = C1_;
     D1 = D1_;
     E1 = E1_;
     F1 = F1_;
     G1 = G1_;
     goto loc_f10;
    }
   }
  goto end;
 }
 loc_f8:
 {
 __cost++;
   if (nondet_bool()) {
    int E1_ = nondet();
    int C1_ = nondet();
    int E_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int I1_ = -1 + H1;
    int H1_ = -1 + H1;
    int F1_ = G1;
    int D1_ = 0;
    int B1_ = 0;
    int M_ = 0;
    if (B1 == 0 && H1 >= 0 && C_ >= 2) {
     C = C_;
     D = D_;
     E = E_;
     M = M_;
     B1 = B1_;
     C1 = C1_;
     D1 = D1_;
     E1 = E1_;
     F1 = F1_;
     H1 = H1_;
     I1 = I1_;
     goto loc_f8;
    }
   }
   if (nondet_bool()) {
    int G1_ = nondet();
    int F1_ = nondet();
    int E1_ = nondet();
    int D1_ = nondet();
    int C1_ = nondet();
    int B1_ = nondet();
    int Y_ = nondet();
    int C_ = nondet();
    if (B1 == G1 && H1 >= 0 && C_ >= 2) {
     C = C_;
     Y = Y_;
     B1 = B1_;
     C1 = C1_;
     D1 = D1_;
     E1 = E1_;
     F1 = F1_;
     G1 = G1_;
     goto loc_f10;
    }
   }
  goto end;
 }
 loc_f9:
 {
 __cost++;
   if (nondet_bool()) {
    int K1_ = nondet();
    int J1_ = nondet();
    int W_ = nondet();
    int T_ = nondet();
    int S_ = nondet();
    int R_ = nondet();
    int Q_ = nondet();
    int C_ = nondet();
    int L1_ = 2;
    int A_ = 2;
    if (Q_ >= 2) {
     A = A_;
     C = C_;
     Q = Q_;
     R = R_;
     S = S_;
     T = T_;
     W = W_;
     J1 = J1_;
     K1 = K1_;
     L1 = L1_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Y_2 = nondet();
    int J1_ = nondet();
    int G1_ = nondet();
    int F1_ = nondet();
    int E1_ = nondet();
    int D1_ = nondet();
    int C1_ = nondet();
    int B1_ = nondet();
    int Y_ = nondet();
    int X_ = nondet();
    int W_ = nondet();
    int T_ = nondet();
    int S_ = nondet();
    int R_ = nondet();
    int Q_ = nondet();
    int C_ = nondet();
    int A_ = nondet();
    int M_ = 0;
    int D_ = 0;
    if (0 >= Y_0 && 0 >= C_ && 0 >= Y_1 && 0 >= Y_2) {
     A = A_;
     C = C_;
     D = D_;
     M = M_;
     Q = Q_;
     R = R_;
     S = S_;
     T = T_;
     W = W_;
     X = X_;
     Y = Y_;
     B1 = B1_;
     C1 = C1_;
     D1 = D1_;
     E1 = E1_;
     F1 = F1_;
     G1 = G1_;
     J1 = J1_;
     goto loc_f10;
    }
   }
   if (nondet_bool()) {
    int J1_ = nondet();
    int G1_ = nondet();
    int F1_ = nondet();
    int E1_ = nondet();
    int D1_ = nondet();
    int C1_ = nondet();
    int B1_ = nondet();
    int Y_ = nondet();
    int X_ = nondet();
    int W_ = nondet();
    int T_ = nondet();
    int S_ = nondet();
    int R_ = nondet();
    int Q_ = nondet();
    int D_ = nondet();
    int A_ = nondet();
    int M_ = 0;
    int C_ = 1;
    if (S == 0) {
     A = A_;
     C = C_;
     D = D_;
     M = M_;
     Q = Q_;
     R = R_;
     S = S_;
     T = T_;
     W = W_;
     X = X_;
     Y = Y_;
     B1 = B1_;
     C1 = C1_;
     D1 = D1_;
     E1 = E1_;
     F1 = F1_;
     G1 = G1_;
     J1 = J1_;
     goto loc_f10;
    }
   }
  goto end;
 }
 loc_f10:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
