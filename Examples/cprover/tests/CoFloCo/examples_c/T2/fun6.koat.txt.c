int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T, int U, int V, int W, int X, int Y, int Z, int A1, int B1, int C1, int D1, int E1, int F1, int G1, int H1, int I1, int J1) {
 goto loc_f30;

 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = nondet();
    if (1 >= 0) {
     A = A_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int F_ = nondet();
    if (1 >= 0) {
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     goto loc_f5;
    }
   }
  goto end;
 }
 loc_f12:
 {
 __cost++;
   if (nondet_bool()) {
    int L_ = M;
    int I_ = 1 + I;
    int F_ = I;
    if (1 >= 0) {
     F = F_;
     I = I_;
     L = L_;
     goto loc_f16;
    }
   }
  goto end;
 }
 loc_f16:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = nondet();
    if (F >= 0 && I >= H) {
     F = F_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_2 = nondet();
    int Y_1 = nondet();
    int H1_ = nondet();
    int G1_ = nondet();
    int F1_ = nondet();
    int E1_ = nondet();
    int D1_ = nondet();
    int C1_ = nondet();
    int B1_ = nondet();
    int A1_ = nondet();
    int M_ = nondet();
    int L_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int I_ = 1 + Y_2;
    int F_ = 1 + Y_0;
    if (Y_0 >= 0 && H >= 1 + I && Y_1 >= 0 && F >= 0) {
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     L = L_;
     M = M_;
     A1 = A1_;
     B1 = B1_;
     C1 = C1_;
     D1 = D1_;
     E1 = E1_;
     F1 = F1_;
     G1 = G1_;
     H1 = H1_;
     goto loc_f16;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 0;
    int B_ = 0;
    if (1 >= 0) {
     B = B_;
     C = C_;
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    int T_ = M;
    if (1 >= 0) {
     T = T_;
     goto loc_f22;
    }
   }
  goto end;
 }
 loc_f23:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Z_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int L_ = 0;
    if (0 >= Y_0) {
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     L = L_;
     Z = Z_;
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int J1_ = nondet();
    int I1_ = nondet();
    int F1_ = nondet();
    int E1_ = nondet();
    int D1_ = nondet();
    int C1_ = nondet();
    int B1_ = nondet();
    int A1_ = nondet();
    int M_ = nondet();
    int L_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int F_ = nondet();
    int I_ = 1 + F_;
    if (Y_0 >= 1) {
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     L = L_;
     M = M_;
     A1 = A1_;
     B1 = B1_;
     C1 = C1_;
     D1 = D1_;
     E1 = E1_;
     F1 = F1_;
     I1 = I1_;
     J1 = J1_;
     goto loc_f16;
    }
   }
  goto end;
 }
 loc_f3:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    int B_ = 1 + B;
    if (A >= 1 + B) {
     B = B_;
     C = C_;
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    if (B >= A) {
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int F_ = nondet();
    int R_ = O;
    int Q_ = 0;
    int P_ = 0;
    int N_ = O;
    int M_ = O;
    if (N == 0 && F >= 1 && Y_0 >= 1) {
     F = F_;
     M = M_;
     N = N_;
     P = P_;
     Q = Q_;
     R = R_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int Q_ = 0;
    int P_ = N;
    if (F_ >= 1 && N >= 1 && F >= 1) {
     F = F_;
     P = P_;
     Q = Q_;
     goto loc_f25;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int Q_ = 0;
    int P_ = N;
    if (F_ >= 1 && 0 >= 1 + N && F >= 1) {
     F = F_;
     P = P_;
     Q = Q_;
     goto loc_f25;
    }
   }
  goto end;
 }
 loc_f30:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f4:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = C;
    int D_ = 0;
    if (1 >= 0) {
     D = D_;
     E = E_;
     goto loc_f5;
    }
   }
   if (nondet_bool()) {
    int T_ = 0;
    int S_ = J;
    if (J >= 1) {
     S = S_;
     T = T_;
     goto loc_f17;
    }
   }
   if (nondet_bool()) {
    int T_ = 0;
    int S_ = J;
    if (0 >= 1 + J) {
     S = S_;
     T = T_;
     goto loc_f17;
    }
   }
   if (nondet_bool()) {
    int M_ = nondet();
    int U_ = 0;
    int T_ = 0;
    int S_ = 0;
    int J_ = 0;
    if (J == 0) {
     J = J_;
     M = M_;
     S = S_;
     T = T_;
     U = U_;
     goto loc_f21;
    }
   }
  goto end;
 }
 loc_f5:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = -1 + E;
    int D_ = 1 + D;
    if (E >= 1) {
     D = D_;
     E = E_;
     goto loc_f5;
    }
   }
   if (nondet_bool()) {
    int L_ = M;
    int I_ = 1 + I;
    int F_ = 1 + F;
    if (F >= 0 && 0 >= E) {
     F = F_;
     I = I_;
     L = L_;
     goto loc_f16;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int T_ = 0;
    int S_ = 0;
    int J_ = 0;
    if (J == 0 && F >= 0 && F_ >= 0) {
     F = F_;
     J = J_;
     S = S_;
     T = T_;
     goto loc_f18;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_ = nondet();
    int X_ = nondet();
    int W_ = nondet();
    int S_ = nondet();
    int O_ = nondet();
    int N_ = nondet();
    int L_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int F_ = nondet();
    int V_ = J;
    int T_ = 0;
    if (Y_0 >= 0 && J >= 1 && F >= 0) {
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     L = L_;
     N = N_;
     O = O_;
     S = S_;
     T = T_;
     V = V_;
     W = W_;
     X = X_;
     Y = Y_;
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_ = nondet();
    int X_ = nondet();
    int W_ = nondet();
    int S_ = nondet();
    int O_ = nondet();
    int N_ = nondet();
    int L_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int F_ = nondet();
    int V_ = J;
    int T_ = 0;
    if (Y_0 >= 0 && 0 >= 1 + J && F >= 0) {
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     L = L_;
     N = N_;
     O = O_;
     S = S_;
     T = T_;
     V = V_;
     W = W_;
     X = X_;
     Y = Y_;
     goto loc_f3;
    }
   }
  goto end;
 }
 loc_f25:
 loc_f17:
 loc_f18:
 loc_f22:
 loc_f21:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
