int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T, int U, int V, int W, int X, int Y, int Z, int A1, int B1, int C1, int D1, int E1, int F1) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = 0;
    int F_ = 1;
    int E_ = 0;
    int D_ = 40;
    int C_ = 0;
    int B_ = 40;
    if (0 >= 1 + A) {
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     G = G_;
     goto loc_f36;
    }
   }
   if (nondet_bool()) {
    int G_ = 0;
    int F_ = 1;
    int E_ = 0;
    int D_ = 40;
    int C_ = 0;
    int B_ = 40;
    if (A >= 1) {
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     G = G_;
     goto loc_f36;
    }
   }
   if (nondet_bool()) {
    int H_ = 0;
    int G_ = 0;
    int F_ = 1;
    int E_ = 0;
    int D_ = 40;
    int C_ = 0;
    int B_ = 40;
    int A_ = 1;
    if (A == 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     G = G_;
     H = H_;
     goto loc_f17;
    }
   }
  goto end;
 }
 loc_f17:
 {
 __cost++;
   if (nondet_bool()) {
    int K_ = nondet();
    int I_ = nondet();
    int L_ = 0;
    int J_ = 0;
    if (255 >= H) {
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     goto loc_f23;
    }
   }
   if (nondet_bool()) {
    if (H >= 256) {
     goto loc_f36;
    }
   }
  goto end;
 }
 loc_f23:
 {
 __cost++;
   if (nondet_bool()) {
    int K_ = nondet();
    int L_ = 1 + L;
    if (7 >= L) {
     K = K_;
     L = L_;
     goto loc_f23;
    }
   }
   if (nondet_bool()) {
    int F1_ = K;
    int H_ = 1 + H;
    if (L >= 8) {
     H = H_;
     F1 = F1_;
     goto loc_f17;
    }
   }
  goto end;
 }
 loc_f36:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = nondet();
    int H_ = 1;
    if (E >= 0) {
     G = G_;
     H = H_;
     goto loc_f42;
    }
   }
   if (nondet_bool()) {
    int H_ = 1;
    if (F >= 0 && 0 >= 1 + E) {
     H = H_;
     goto loc_f42;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int H_ = 1;
    if (0 >= 1 + F && 0 >= 1 + E) {
     G = G_;
     H = H_;
     goto loc_f42;
    }
   }
  goto end;
 }
 loc_f42:
 {
 __cost++;
   if (nondet_bool()) {
    int M_ = nondet();
    int G_ = nondet();
    int H_ = 1 + H;
    if (D >= H && 0 >= 1 + F) {
     G = G_;
     H = H_;
     M = M_;
     goto loc_f42;
    }
   }
   if (nondet_bool()) {
    int M_ = nondet();
    int G_ = nondet();
    int H_ = 1 + H;
    if (D >= H && F >= 0) {
     G = G_;
     H = H_;
     M = M_;
     goto loc_f42;
    }
   }
   if (nondet_bool()) {
    int E1_ = G;
    int O_ = G;
    if (H >= 1 + D && F >= 0) {
     O = O_;
     E1 = E1_;
     goto loc_f56;
    }
   }
   if (nondet_bool()) {
    int E1_ = nondet();
    int O_ = nondet();
    if (H >= 1 + D && 0 >= 1 + F) {
     O = O_;
     E1 = E1_;
     goto loc_f56;
    }
   }
  goto end;
 }
 loc_f56:
 {
 __cost++;
   if (nondet_bool()) {
    int T_ = O;
    int S_ = 1;
    int R_ = 0;
    int Q_ = 2 + B;
    int P_ = O;
    int N_ = O;
    if (0 >= 1 + A) {
     N = N_;
     P = P_;
     Q = Q_;
     R = R_;
     S = S_;
     T = T_;
     goto loc_f85;
    }
   }
   if (nondet_bool()) {
    int T_ = O;
    int S_ = 1;
    int R_ = 0;
    int Q_ = 2 + B;
    int P_ = O;
    int N_ = O;
    if (A >= 1) {
     N = N_;
     P = P_;
     Q = Q_;
     R = R_;
     S = S_;
     T = T_;
     goto loc_f85;
    }
   }
   if (nondet_bool()) {
    int U_ = 0;
    int T_ = O;
    int S_ = 1;
    int R_ = 0;
    int Q_ = 2 + B;
    int P_ = O;
    int N_ = O;
    int A_ = 1;
    if (A == 0) {
     A = A_;
     N = N_;
     P = P_;
     Q = Q_;
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     goto loc_f66;
    }
   }
  goto end;
 }
 loc_f66:
 {
 __cost++;
   if (nondet_bool()) {
    int X_ = nondet();
    int V_ = nondet();
    int Y_ = 0;
    int W_ = 0;
    if (255 >= U) {
     V = V_;
     W = W_;
     X = X_;
     Y = Y_;
     goto loc_f72;
    }
   }
   if (nondet_bool()) {
    if (U >= 256) {
     goto loc_f85;
    }
   }
  goto end;
 }
 loc_f72:
 {
 __cost++;
   if (nondet_bool()) {
    int X_ = nondet();
    int Y_ = 1 + Y;
    if (7 >= Y) {
     X = X_;
     Y = Y_;
     goto loc_f72;
    }
   }
   if (nondet_bool()) {
    int D1_ = X;
    int U_ = 1 + U;
    if (Y >= 8) {
     U = U_;
     D1 = D1_;
     goto loc_f66;
    }
   }
  goto end;
 }
 loc_f85:
 {
 __cost++;
   if (nondet_bool()) {
    int T_ = nondet();
    int U_ = 1;
    if (R >= 0) {
     T = T_;
     U = U_;
     goto loc_f91;
    }
   }
   if (nondet_bool()) {
    int U_ = 1;
    if (S >= 0 && 0 >= 1 + R) {
     U = U_;
     goto loc_f91;
    }
   }
   if (nondet_bool()) {
    int T_ = nondet();
    int U_ = 1;
    if (0 >= 1 + S && 0 >= 1 + R) {
     T = T_;
     U = U_;
     goto loc_f91;
    }
   }
  goto end;
 }
 loc_f91:
 {
 __cost++;
   if (nondet_bool()) {
    int Z_ = nondet();
    int T_ = nondet();
    int U_ = 1 + U;
    if (Q >= U && 0 >= 1 + S) {
     T = T_;
     U = U_;
     Z = Z_;
     goto loc_f91;
    }
   }
   if (nondet_bool()) {
    int Z_ = nondet();
    int T_ = nondet();
    int U_ = 1 + U;
    if (Q >= U && S >= 0) {
     T = T_;
     U = U_;
     Z = Z_;
     goto loc_f91;
    }
   }
   if (nondet_bool()) {
    int C1_ = T;
    int B1_ = T;
    int A1_ = T;
    if (U >= 1 + Q && S >= 0) {
     A1 = A1_;
     B1 = B1_;
     C1 = C1_;
     goto loc_f106;
    }
   }
   if (nondet_bool()) {
    int C1_ = nondet();
    int B1_ = nondet();
    int A1_ = nondet();
    if (U >= 1 + Q && 0 >= 1 + S) {
     A1 = A1_;
     B1 = B1_;
     C1 = C1_;
     goto loc_f106;
    }
   }
  goto end;
 }
 loc_f106:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
