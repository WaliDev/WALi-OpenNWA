int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T, int U, int V, int W) {
 goto loc_f2;

 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int P_ = nondet();
    int O_ = nondet();
    int N_ = nondet();
    int M_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    if (0 >= 1 + J && 0 >= Y_0) {
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     O = O_;
     P = P_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int S_ = nondet();
    int R_ = nondet();
    int Q_ = nondet();
    int N_ = nondet();
    int M_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    if (Y_0 >= 2 && Y_1 >= 1 && 0 >= 1 + J) {
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     Q = Q_;
     R = R_;
     S = S_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int V_ = nondet();
    int U_ = nondet();
    int T_ = nondet();
    int P_ = nondet();
    int O_ = nondet();
    int N_ = nondet();
    int K_ = nondet();
    if (J >= 1 && 0 >= 2 + Y_0) {
     K = K_;
     N = N_;
     O = O_;
     P = P_;
     T = T_;
     U = U_;
     V = V_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int V_ = nondet();
    int U_ = nondet();
    int T_ = nondet();
    int S_ = nondet();
    int R_ = nondet();
    int Q_ = nondet();
    int N_ = nondet();
    int K_ = nondet();
    if (1 + Y_0 >= 0 && Y_1 >= 0 && J >= 1) {
     K = K_;
     N = N_;
     Q = Q_;
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     V = V_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int W_ = nondet();
    int Q_ = nondet();
    int N_ = nondet();
    int M_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    if (Y_0 >= 1 && 0 >= 1 + J && 1 >= Y_1) {
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     Q = Q_;
     W = W_;
     goto loc_f300;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int W_ = nondet();
    int V_ = nondet();
    int U_ = nondet();
    int T_ = nondet();
    int Q_ = nondet();
    int N_ = nondet();
    int K_ = nondet();
    if (1 + Y_0 >= 0 && J >= 1 && 0 >= 1 + Y_1) {
     K = K_;
     N = N_;
     Q = Q_;
     T = T_;
     U = U_;
     V = V_;
     W = W_;
     goto loc_f300;
    }
   }
   if (nondet_bool()) {
    int W_ = nondet();
    int T_ = nondet();
    int K_ = nondet();
    if (J == 0) {
     K = K_;
     T = T_;
     W = W_;
     goto loc_f300;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int F_ = nondet();
    int E_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int B_ = nondet();
    int A_ = nondet();
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f300:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
