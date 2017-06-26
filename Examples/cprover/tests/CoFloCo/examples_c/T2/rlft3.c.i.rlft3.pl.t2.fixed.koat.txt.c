int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T, int U, int V, int W) {
 goto loc_f2;

 loc_f13:
 {
 __cost++;
   if (nondet_bool()) {
    if (H >= I) {
     goto loc_f16;
    }
   }
   if (nondet_bool()) {
    if (I >= 1 + H) {
     goto loc_f27;
    }
   }
  goto end;
 }
 loc_f16:
 {
 __cost++;
   if (nondet_bool()) {
    int L_ = 2 + L;
    int K_ = 1 + K;
    if (J >= K) {
     K = K_;
     L = L_;
     goto loc_f16;
    }
   }
   if (nondet_bool()) {
    int I_ = 1 + I;
    if (K >= 1 + J) {
     I = I_;
     goto loc_f13;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = nondet();
    int E_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int B_ = nondet();
    int A_ = 1;
    if (A == 1) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     G = G_;
     goto loc_f13;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = nondet();
    int E_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int B_ = nondet();
    if (0 >= A) {
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     G = G_;
     goto loc_f27;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = nondet();
    int E_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int B_ = nondet();
    if (A >= 2) {
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     G = G_;
     goto loc_f27;
    }
   }
  goto end;
 }
 loc_f27:
 {
 __cost++;
   if (nondet_bool()) {
    int O_ = 0;
    int N_ = 1;
    int M_ = 2 + H + -I;
    if (H >= I && 0 >= I) {
     M = M_;
     N = N_;
     O = O_;
     goto loc_f35;
    }
   }
   if (nondet_bool()) {
    int O_ = 0;
    int N_ = 1;
    int M_ = 2 + H + -I;
    if (H >= I && I >= 2) {
     M = M_;
     N = N_;
     O = O_;
     goto loc_f35;
    }
   }
   if (nondet_bool()) {
    int O_ = 0;
    int N_ = 1;
    int M_ = 1;
    int I_ = 1;
    if (I == 1 && H >= 1) {
     I = I_;
     M = M_;
     N = N_;
     O = O_;
     goto loc_f35;
    }
   }
   if (nondet_bool()) {
    if (I >= 1 + H && 0 >= 2 + A) {
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    if (I >= 1 + H && A >= 0) {
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int A_ = -1;
    if (1 + A == 0 && I >= 1 + H) {
     A = A_;
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f35:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (1 + Y_0 >= Q && 3 * Y_0 >= 1 + P && P >= 2 * Y_0) {
     goto loc_f38;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int I_ = 1 + I;
    if (Q >= 2 + Y_0 && 3 * Y_0 >= 1 + P && P >= 2 * Y_0) {
     I = I_;
     goto loc_f27;
    }
   }
  goto end;
 }
 loc_f38:
 {
 __cost++;
   if (nondet_bool()) {
    if (J >= K && 0 >= Q) {
     goto loc_f53;
    }
   }
   if (nondet_bool()) {
    if (J >= K && Q >= 2) {
     goto loc_f53;
    }
   }
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
    int U_ = -C * Y_7 + -C * Y_8;
    int T_ = C * Y_5 + -C * Y_6;
    int S_ = B * Y_3 + -B * Y_4;
    int R_ = B * Y_1 + B * Y_2;
    int Q_ = 1;
    int L_ = 3 + Y_0;
    int K_ = 1 + K;
    if (Q == 1 && J >= K && 0 >= K && 5 * Y_0 + K >= 1 + J && J >= K + 4 * Y_0) {
     K = K_;
     L = L_;
     Q = Q_;
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     goto loc_f38;
    }
   }
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
    int U_ = -C * Y_7 + -C * Y_8;
    int T_ = C * Y_5 + -C * Y_6;
    int S_ = B * Y_3 + -B * Y_4;
    int R_ = B * Y_1 + B * Y_2;
    int Q_ = 1;
    int L_ = 3 + Y_0;
    int K_ = 1 + K;
    if (Q == 1 && K >= 2 && J >= K && 5 * Y_0 + K >= 1 + J && J >= K + 4 * Y_0) {
     K = K_;
     L = L_;
     Q = Q_;
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     goto loc_f38;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Y_2 = nondet();
    int Y_3 = nondet();
    int Y_4 = nondet();
    int Y_5 = nondet();
    int Y_6 = nondet();
    int Y_7 = nondet();
    int U_ = -C * Y_6 + -C * Y_7;
    int T_ = C * Y_4 + -C * Y_5;
    int S_ = B * Y_2 + -B * Y_3;
    int R_ = B * Y_0 + B * Y_1;
    int Q_ = 1;
    int L_ = 1;
    int K_ = 2;
    if (Q == 1 && K == 1 && J >= 1) {
     K = K_;
     L = L_;
     Q = Q_;
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     goto loc_f38;
    }
   }
   if (nondet_bool()) {
    int W_ = 2 + W;
    int Q_ = 1 + Q;
    int O_ = O * F + N * G + O;
    int N_ = N * F + -O * G + N;
    int E_ = N;
    if (K >= 1 + J) {
     E = E_;
     N = N_;
     O = O_;
     Q = Q_;
     W = W_;
     goto loc_f35;
    }
   }
  goto end;
 }
 loc_f53:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_1 = nondet();
    int Y_2 = nondet();
    int Y_3 = nondet();
    int Y_4 = nondet();
    int Y_5 = nondet();
    int Y_6 = nondet();
    int Y_7 = nondet();
    int Y_8 = nondet();
    int Y_0 = nondet();
    int V_ = 3 + P + -Y_0;
    int U_ = -C * Y_7 + -C * Y_8;
    int T_ = C * Y_5 + -C * Y_6;
    int S_ = B * Y_3 + -B * Y_4;
    int R_ = B * Y_1 + B * Y_2;
    int L_ = 2 + J + -K;
    int K_ = 1 + K;
    if (0 >= K && 3 * Y_0 >= 1 + Q && Q >= 2 * Y_0) {
     K = K_;
     L = L_;
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     V = V_;
     goto loc_f38;
    }
   }
   if (nondet_bool()) {
    int Y_1 = nondet();
    int Y_2 = nondet();
    int Y_3 = nondet();
    int Y_4 = nondet();
    int Y_5 = nondet();
    int Y_6 = nondet();
    int Y_7 = nondet();
    int Y_8 = nondet();
    int Y_0 = nondet();
    int V_ = 3 + P + -Y_0;
    int U_ = -C * Y_7 + -C * Y_8;
    int T_ = C * Y_5 + -C * Y_6;
    int S_ = B * Y_3 + -B * Y_4;
    int R_ = B * Y_1 + B * Y_2;
    int L_ = 2 + J + -K;
    int K_ = 1 + K;
    if (K >= 2 && 3 * Y_0 >= 1 + Q && Q >= 2 * Y_0) {
     K = K_;
     L = L_;
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     V = V_;
     goto loc_f38;
    }
   }
   if (nondet_bool()) {
    int Y_1 = nondet();
    int Y_2 = nondet();
    int Y_3 = nondet();
    int Y_4 = nondet();
    int Y_5 = nondet();
    int Y_6 = nondet();
    int Y_7 = nondet();
    int Y_8 = nondet();
    int Y_0 = nondet();
    int V_ = 3 + P + -Y_0;
    int U_ = -C * Y_7 + -C * Y_8;
    int T_ = C * Y_5 + -C * Y_6;
    int S_ = B * Y_3 + -B * Y_4;
    int R_ = B * Y_1 + B * Y_2;
    int L_ = 1;
    int K_ = 2;
    if (K == 1 && 3 * Y_0 >= 1 + Q && Q >= 2 * Y_0) {
     K = K_;
     L = L_;
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     V = V_;
     goto loc_f38;
    }
   }
  goto end;
 }
 loc_f1:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
