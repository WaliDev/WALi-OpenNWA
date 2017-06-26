int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T, int U, int V, int W, int X, int Y, int Z, int A1, int B1, int C1, int D1, int E1) {
 goto loc_f26;

 loc_f11:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = B;
    int C_ = B;
    if (A >= 1 + B) {
     C = C_;
     D = D_;
     goto loc_f11;
    }
   }
   if (nondet_bool()) {
    int D_ = B;
    int C_ = B;
    if (B >= 1 + A) {
     C = C_;
     D = D_;
     goto loc_f11;
    }
   }
  goto end;
 }
 loc_f26:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int C1_ = nondet();
    int B1_ = nondet();
    int A1_ = nondet();
    int Z_ = nondet();
    int Y_ = nondet();
    int X_ = nondet();
    int W_ = nondet();
    int V_ = nondet();
    int T_ = nondet();
    int R_ = nondet();
    int Q_ = nondet();
    int N_ = nondet();
    int F_ = nondet();
    int D1_ = 4;
    int U_ = 0;
    int S_ = 4;
    int P_ = 3;
    int O_ = 2;
    int J_ = 4;
    int I_ = 1;
    int H_ = 4;
    int G_ = 1;
    if (E >= 1 + Y_0) {
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     N = N_;
     O = O_;
     P = P_;
     Q = Q_;
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     V = V_;
     W = W_;
     X = X_;
     Y = Y_;
     Z = Z_;
     A1 = A1_;
     B1 = B1_;
     C1 = C1_;
     D1 = D1_;
     goto loc_f6;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int C1_ = nondet();
    int B1_ = nondet();
    int A1_ = nondet();
    int Z_ = nondet();
    int Y_ = nondet();
    int X_ = nondet();
    int W_ = nondet();
    int V_ = nondet();
    int T_ = nondet();
    int R_ = nondet();
    int Q_ = nondet();
    int N_ = nondet();
    int F_ = nondet();
    int D1_ = 4;
    int U_ = 0;
    int S_ = 4;
    int P_ = 3;
    int O_ = 2;
    int J_ = 4;
    int I_ = 1;
    int H_ = 4;
    int G_ = 1;
    if (Y_0 >= 1 + E) {
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     N = N_;
     O = O_;
     P = P_;
     Q = Q_;
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     V = V_;
     W = W_;
     X = X_;
     Y = Y_;
     Z = Z_;
     A1 = A1_;
     B1 = B1_;
     C1 = C1_;
     D1 = D1_;
     goto loc_f6;
    }
   }
  goto end;
 }
 loc_f6:
 {
 __cost++;
   if (nondet_bool()) {
    int L_ = nondet();
    int K_ = nondet();
    int F_ = nondet();
    int M_ = -1 + H;
    int J_ = -1 + H;
    int I_ = 1 + G;
    int H_ = -1 + H;
    int G_ = 1 + G;
    if (H >= 1 && G >= 0 && E >= 1 + F) {
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f6;
    }
   }
   if (nondet_bool()) {
    int L_ = nondet();
    int K_ = nondet();
    int F_ = nondet();
    int M_ = -1 + H;
    int J_ = -1 + H;
    int I_ = 1 + G;
    int H_ = -1 + H;
    int G_ = 1 + G;
    if (H >= 1 && G >= 0 && F >= 1 + E) {
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f6;
    }
   }
   if (nondet_bool()) {
    int E1_ = nondet();
    int Z_ = nondet();
    int N_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int B_ = nondet();
    int E_ = F;
    if (F == E && H >= 1 && G >= 0 && A >= 1 + D_) {
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     N = N_;
     Z = Z_;
     E1 = E1_;
     goto loc_f11;
    }
   }
   if (nondet_bool()) {
    int E1_ = nondet();
    int Z_ = nondet();
    int N_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int B_ = nondet();
    int E_ = F;
    if (F == E && H >= 1 && G >= 0 && D_ >= 1 + A) {
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     N = N_;
     Z = Z_;
     E1 = E1_;
     goto loc_f11;
    }
   }
  goto end;
 }
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
