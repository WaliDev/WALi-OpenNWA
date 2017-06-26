int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T, int U, int V, int W) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int W_ = 0;
    int F_ = 0;
    if (1 >= 0) {
     F = F_;
     W = W_;
     goto loc_f11;
    }
   }
  goto end;
 }
 loc_f11:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_f45;
    }
   }
   if (nondet_bool()) {
    int O_ = nondet();
    int N_ = nondet();
    int M_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int H_ = nondet();
    int L_ = D;
    if (B >= 1 + A && O_ >= 1) {
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     O = O_;
     goto loc_f45;
    }
   }
   if (nondet_bool()) {
    int U_ = nondet();
    int T_ = nondet();
    int S_ = nondet();
    int P_ = nondet();
    int O_ = nondet();
    int N_ = nondet();
    int M_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int H_ = nondet();
    int C_ = nondet();
    int V_ = 0;
    int R_ = 0;
    int Q_ = R;
    int L_ = D;
    if (U_ >= 2 && 0 >= P_ && B >= 1 + A) {
     C = C_;
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     O = O_;
     P = P_;
     Q = Q_;
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     V = V_;
     goto loc_f37;
    }
   }
   if (nondet_bool()) {
    int U_ = nondet();
    int T_ = nondet();
    int S_ = nondet();
    int P_ = nondet();
    int O_ = nondet();
    int N_ = nondet();
    int M_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int H_ = nondet();
    int C_ = nondet();
    int V_ = 0;
    int R_ = 0;
    int Q_ = R;
    int L_ = D;
    if (0 >= U_ && 0 >= P_ && B >= 1 + A) {
     C = C_;
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     O = O_;
     P = P_;
     Q = Q_;
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     V = V_;
     goto loc_f37;
    }
   }
   if (nondet_bool()) {
    int P_ = nondet();
    int O_ = nondet();
    int N_ = nondet();
    int M_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int H_ = nondet();
    int V_ = 0;
    int U_ = 1;
    int T_ = 1;
    int S_ = 1;
    int Q_ = R;
    int L_ = D;
    int C_ = 1;
    int A_ = 1 + A;
    if (B >= 1 + A && 0 >= P_) {
     A = A_;
     C = C_;
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     O = O_;
     P = P_;
     Q = Q_;
     S = S_;
     T = T_;
     U = U_;
     V = V_;
     goto loc_f11;
    }
   }
  goto end;
 }
 loc_f37:
 {
 __cost++;
   if (nondet_bool()) {
    if (C >= 3) {
     goto loc_f45;
    }
   }
   if (nondet_bool()) {
    if (1 >= C) {
     goto loc_f45;
    }
   }
   if (nondet_bool()) {
    int G_ = H;
    int D_ = 1 + D;
    int C_ = 2;
    if (C == 2) {
     C = C_;
     D = D_;
     G = G_;
     goto loc_f45;
    }
   }
  goto end;
 }
 loc_f45:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = nondet();
    int D_ = 0;
    if (0 >= E_) {
     D = D_;
     E = E_;
     goto loc_f53;
    }
   }
   if (nondet_bool()) {
    int E_ = nondet();
    int F_ = 0;
    int D_ = 0;
    if (E_ >= 1) {
     D = D_;
     E = E_;
     F = F_;
     goto loc_f53;
    }
   }
  goto end;
 }
 loc_f53:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f53;
    }
   }
  goto end;
 }
 loc_f55:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f58;
    }
   }
  goto end;
 }
 loc_f58:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
