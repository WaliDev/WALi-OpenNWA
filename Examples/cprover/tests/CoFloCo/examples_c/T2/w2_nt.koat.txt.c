int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T, int U, int V, int W, int X, int Y, int Z, int A1) {
 goto loc_f23;

 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int L_ = nondet();
    int K_ = nondet();
    int F_ = nondet();
    int M_ = -1 + G;
    int J_ = -1 + G;
    int I_ = 1 + H;
    int H_ = 1 + H;
    int G_ = -1 + G;
    if (H >= 0 && G >= 1 && E >= 1 + F) {
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int L_ = nondet();
    int K_ = nondet();
    int F_ = nondet();
    int M_ = -1 + G;
    int J_ = -1 + G;
    int I_ = 1 + H;
    int H_ = 1 + H;
    int G_ = -1 + G;
    if (H >= 0 && G >= 1 && F >= 1 + E) {
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int A1_ = nondet();
    int U_ = nondet();
    int Q_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int B_ = nondet();
    int E_ = F;
    if (F == E && H >= 0 && G >= 1 && A >= 1 + D_) {
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     Q = Q_;
     U = U_;
     A1 = A1_;
     goto loc_f8;
    }
   }
   if (nondet_bool()) {
    int A1_ = nondet();
    int U_ = nondet();
    int Q_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int B_ = nondet();
    int E_ = F;
    if (F == E && H >= 0 && G >= 1 && D_ >= 1 + A) {
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     Q = Q_;
     U = U_;
     A1 = A1_;
     goto loc_f8;
    }
   }
  goto end;
 }
 loc_f23:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_ = nondet();
    int W_ = nondet();
    int V_ = nondet();
    int U_ = nondet();
    int T_ = nondet();
    int S_ = nondet();
    int R_ = nondet();
    int Q_ = nondet();
    int P_ = nondet();
    int O_ = nondet();
    int F_ = nondet();
    int Z_ = 0;
    int X_ = 2;
    int N_ = 2;
    int J_ = 2;
    int I_ = 1;
    int H_ = 1;
    int G_ = 2;
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
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_ = nondet();
    int W_ = nondet();
    int V_ = nondet();
    int U_ = nondet();
    int T_ = nondet();
    int S_ = nondet();
    int R_ = nondet();
    int Q_ = nondet();
    int P_ = nondet();
    int O_ = nondet();
    int F_ = nondet();
    int Z_ = 0;
    int X_ = 2;
    int N_ = 2;
    int J_ = 2;
    int I_ = 1;
    int H_ = 1;
    int G_ = 2;
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
     goto loc_f2;
    }
   }
  goto end;
 }
 loc_f8:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = B;
    int C_ = B;
    if (A >= 1 + B) {
     C = C_;
     D = D_;
     goto loc_f8;
    }
   }
   if (nondet_bool()) {
    int D_ = B;
    int C_ = B;
    if (B >= 1 + A) {
     C = C_;
     D = D_;
     goto loc_f8;
    }
   }
  goto end;
 }
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
