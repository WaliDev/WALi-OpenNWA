int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T, int U, int V, int W, int X, int Y, int Z, int A1, int B1, int C1, int D1) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 0;
    int A_ = 100;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_f18;
    }
   }
   if (nondet_bool()) {
    int B_ = 10;
    int A_ = 100;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_f18;
    }
   }
  goto end;
 }
 loc_f101:
 {
 __cost++;
   if (nondet_bool()) {
    int P_ = 0;
    if (1 >= 0) {
     P = P_;
     goto loc_f107;
    }
   }
   if (nondet_bool()) {
    int P_ = 10;
    if (1 >= 0) {
     P = P_;
     goto loc_f107;
    }
   }
  goto end;
 }
 loc_f107:
 {
 __cost++;
   if (nondet_bool()) {
    int Q_ = 0;
    int P_ = 0;
    if (P == 0) {
     P = P_;
     Q = Q_;
     goto loc_f126;
    }
   }
   if (nondet_bool()) {
    int R_ = 0;
    int Q_ = P;
    if (0 >= 1 + P) {
     Q = Q_;
     R = R_;
     goto loc_f110;
    }
   }
   if (nondet_bool()) {
    int R_ = 0;
    int Q_ = P;
    if (P >= 1) {
     Q = Q_;
     R = R_;
     goto loc_f110;
    }
   }
  goto end;
 }
 loc_f110:
 {
 __cost++;
   if (nondet_bool()) {
    int R_ = 1 + R;
    if (M >= 1 + R) {
     R = R_;
     goto loc_f110;
    }
   }
   if (nondet_bool()) {
    if (R >= M) {
     goto loc_f126;
    }
   }
  goto end;
 }
 loc_f126:
 {
 __cost++;
   if (nondet_bool()) {
    int T_ = 0;
    int S_ = 20;
    if (1 >= 0) {
     S = S_;
     T = T_;
     goto loc_f138;
    }
   }
   if (nondet_bool()) {
    int T_ = 10;
    int S_ = 20;
    if (1 >= 0) {
     S = S_;
     T = T_;
     goto loc_f138;
    }
   }
  goto end;
 }
 loc_f138:
 {
 __cost++;
   if (nondet_bool()) {
    int U_ = T;
    if (0 >= 1 + T) {
     U = U_;
     goto loc_f141;
    }
   }
   if (nondet_bool()) {
    int U_ = T;
    if (T >= 1) {
     U = U_;
     goto loc_f141;
    }
   }
   if (nondet_bool()) {
    int U_ = 0;
    int T_ = 0;
    if (T == 0) {
     T = T_;
     U = U_;
     goto loc_f166;
    }
   }
  goto end;
 }
 loc_f141:
 {
 __cost++;
   if (nondet_bool()) {
    int V_ = 0;
    if (1 >= 0) {
     V = V_;
     goto loc_f147;
    }
   }
   if (nondet_bool()) {
    int V_ = 10;
    if (1 >= 0) {
     V = V_;
     goto loc_f147;
    }
   }
  goto end;
 }
 loc_f147:
 {
 __cost++;
   if (nondet_bool()) {
    int W_ = 0;
    int V_ = 0;
    if (V == 0) {
     V = V_;
     W = W_;
     goto loc_f166;
    }
   }
   if (nondet_bool()) {
    int X_ = 0;
    int W_ = V;
    if (0 >= 1 + V) {
     W = W_;
     X = X_;
     goto loc_f150;
    }
   }
   if (nondet_bool()) {
    int X_ = 0;
    int W_ = V;
    if (V >= 1) {
     W = W_;
     X = X_;
     goto loc_f150;
    }
   }
  goto end;
 }
 loc_f150:
 {
 __cost++;
   if (nondet_bool()) {
    int X_ = 1 + X;
    if (S >= 1 + X) {
     X = X_;
     goto loc_f150;
    }
   }
   if (nondet_bool()) {
    if (X >= S) {
     goto loc_f166;
    }
   }
  goto end;
 }
 loc_f166:
 {
 __cost++;
   if (nondet_bool()) {
    int Z_ = 0;
    int Y_ = 200;
    if (1 >= 0) {
     Y = Y_;
     Z = Z_;
     goto loc_f178;
    }
   }
   if (nondet_bool()) {
    int Z_ = 10;
    int Y_ = 200;
    if (1 >= 0) {
     Y = Y_;
     Z = Z_;
     goto loc_f178;
    }
   }
  goto end;
 }
 loc_f178:
 {
 __cost++;
   if (nondet_bool()) {
    int A1_ = Z;
    if (0 >= 1 + Z) {
     A1 = A1_;
     goto loc_f181;
    }
   }
   if (nondet_bool()) {
    int A1_ = Z;
    if (Z >= 1) {
     A1 = A1_;
     goto loc_f181;
    }
   }
   if (nondet_bool()) {
    int A1_ = 0;
    int Z_ = 0;
    if (Z == 0) {
     Z = Z_;
     A1 = A1_;
     goto loc_f207;
    }
   }
  goto end;
 }
 loc_f18:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = B;
    if (0 >= 1 + B) {
     C = C_;
     goto loc_f21;
    }
   }
   if (nondet_bool()) {
    int C_ = B;
    if (B >= 1) {
     C = C_;
     goto loc_f21;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    int B_ = 0;
    if (B == 0) {
     B = B_;
     C = C_;
     goto loc_f46;
    }
   }
  goto end;
 }
 loc_f181:
 {
 __cost++;
   if (nondet_bool()) {
    int B1_ = 0;
    if (1 >= 0) {
     B1 = B1_;
     goto loc_f187;
    }
   }
   if (nondet_bool()) {
    int B1_ = 10;
    if (1 >= 0) {
     B1 = B1_;
     goto loc_f187;
    }
   }
  goto end;
 }
 loc_f187:
 {
 __cost++;
   if (nondet_bool()) {
    int D1_ = 0;
    int C1_ = B1;
    if (0 >= 1 + B1) {
     C1 = C1_;
     D1 = D1_;
     goto loc_f190;
    }
   }
   if (nondet_bool()) {
    int D1_ = 0;
    int C1_ = B1;
    if (B1 >= 1) {
     C1 = C1_;
     D1 = D1_;
     goto loc_f190;
    }
   }
   if (nondet_bool()) {
    int C1_ = 0;
    int B1_ = 0;
    if (B1 == 0) {
     B1 = B1_;
     C1 = C1_;
     goto loc_f207;
    }
   }
  goto end;
 }
 loc_f190:
 {
 __cost++;
   if (nondet_bool()) {
    int D1_ = 1 + D1;
    if (Y >= 1 + D1) {
     D1 = D1_;
     goto loc_f190;
    }
   }
   if (nondet_bool()) {
    if (D1 >= Y) {
     goto loc_f207;
    }
   }
  goto end;
 }
 loc_f21:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 0;
    if (1 >= 0) {
     D = D_;
     goto loc_f27;
    }
   }
   if (nondet_bool()) {
    int D_ = 10;
    if (1 >= 0) {
     D = D_;
     goto loc_f27;
    }
   }
  goto end;
 }
 loc_f27:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 0;
    int D_ = 0;
    if (D == 0) {
     D = D_;
     E = E_;
     goto loc_f46;
    }
   }
   if (nondet_bool()) {
    int F_ = 0;
    int E_ = D;
    if (0 >= 1 + D) {
     E = E_;
     F = F_;
     goto loc_f30;
    }
   }
   if (nondet_bool()) {
    int F_ = 0;
    int E_ = D;
    if (D >= 1) {
     E = E_;
     F = F_;
     goto loc_f30;
    }
   }
  goto end;
 }
 loc_f30:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = 1 + F;
    if (A >= 1 + F) {
     F = F_;
     goto loc_f30;
    }
   }
   if (nondet_bool()) {
    if (F >= A) {
     goto loc_f46;
    }
   }
  goto end;
 }
 loc_f46:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = 0;
    int G_ = 200;
    if (1 >= 0) {
     G = G_;
     H = H_;
     goto loc_f58;
    }
   }
   if (nondet_bool()) {
    int H_ = 10;
    int G_ = 200;
    if (1 >= 0) {
     G = G_;
     H = H_;
     goto loc_f58;
    }
   }
  goto end;
 }
 loc_f58:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = H;
    if (0 >= 1 + H) {
     I = I_;
     goto loc_f61;
    }
   }
   if (nondet_bool()) {
    int I_ = H;
    if (H >= 1) {
     I = I_;
     goto loc_f61;
    }
   }
   if (nondet_bool()) {
    int I_ = 0;
    int H_ = 0;
    if (H == 0) {
     H = H_;
     I = I_;
     goto loc_f86;
    }
   }
  goto end;
 }
 loc_f61:
 {
 __cost++;
   if (nondet_bool()) {
    int J_ = 0;
    if (1 >= 0) {
     J = J_;
     goto loc_f67;
    }
   }
   if (nondet_bool()) {
    int J_ = 10;
    if (1 >= 0) {
     J = J_;
     goto loc_f67;
    }
   }
  goto end;
 }
 loc_f67:
 {
 __cost++;
   if (nondet_bool()) {
    int K_ = 0;
    int J_ = 0;
    if (J == 0) {
     J = J_;
     K = K_;
     goto loc_f86;
    }
   }
   if (nondet_bool()) {
    int L_ = 0;
    int K_ = J;
    if (0 >= 1 + J) {
     K = K_;
     L = L_;
     goto loc_f70;
    }
   }
   if (nondet_bool()) {
    int L_ = 0;
    int K_ = J;
    if (J >= 1) {
     K = K_;
     L = L_;
     goto loc_f70;
    }
   }
  goto end;
 }
 loc_f70:
 {
 __cost++;
   if (nondet_bool()) {
    int L_ = 1 + L;
    if (G >= 1 + L) {
     L = L_;
     goto loc_f70;
    }
   }
   if (nondet_bool()) {
    if (L >= G) {
     goto loc_f86;
    }
   }
  goto end;
 }
 loc_f86:
 {
 __cost++;
   if (nondet_bool()) {
    int N_ = 0;
    int M_ = 50;
    if (1 >= 0) {
     M = M_;
     N = N_;
     goto loc_f98;
    }
   }
   if (nondet_bool()) {
    int N_ = 10;
    int M_ = 50;
    if (1 >= 0) {
     M = M_;
     N = N_;
     goto loc_f98;
    }
   }
  goto end;
 }
 loc_f98:
 {
 __cost++;
   if (nondet_bool()) {
    int O_ = N;
    if (0 >= 1 + N) {
     O = O_;
     goto loc_f101;
    }
   }
   if (nondet_bool()) {
    int O_ = N;
    if (N >= 1) {
     O = O_;
     goto loc_f101;
    }
   }
   if (nondet_bool()) {
    int O_ = 0;
    int N_ = 0;
    if (N == 0) {
     N = N_;
     O = O_;
     goto loc_f126;
    }
   }
  goto end;
 }
 loc_f207:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
