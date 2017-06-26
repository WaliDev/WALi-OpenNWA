int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T, int U, int V) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int V_ = 0;
    int C_ = 0;
    if (1 >= 0) {
     C = C_;
     V = V_;
     goto loc_f11;
    }
   }
  goto end;
 }
 loc_f11:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 0;
    int C_ = 0;
    if (A >= B) {
     C = C_;
     D = D_;
     goto loc_f51;
    }
   }
   if (nondet_bool()) {
    int N_ = nondet();
    int M_ = nondet();
    int L_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int K_ = D;
    int D_ = 0;
    int C_ = 0;
    if (B >= 1 + A && N_ >= 1) {
     C = C_;
     D = D_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     goto loc_f51;
    }
   }
   if (nondet_bool()) {
    int T_ = nondet();
    int S_ = nondet();
    int R_ = nondet();
    int O_ = nondet();
    int N_ = nondet();
    int M_ = nondet();
    int L_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int E_ = nondet();
    int U_ = 0;
    int Q_ = 0;
    int P_ = Q;
    int K_ = D;
    if (T_ >= 2 && 0 >= O_ && B >= 1 + A) {
     E = E_;
     G = G_;
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
     goto loc_f37;
    }
   }
   if (nondet_bool()) {
    int T_ = nondet();
    int S_ = nondet();
    int R_ = nondet();
    int O_ = nondet();
    int N_ = nondet();
    int M_ = nondet();
    int L_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int E_ = nondet();
    int U_ = 0;
    int Q_ = 0;
    int P_ = Q;
    int K_ = D;
    if (0 >= T_ && 0 >= O_ && B >= 1 + A) {
     E = E_;
     G = G_;
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
     goto loc_f37;
    }
   }
   if (nondet_bool()) {
    int O_ = nondet();
    int N_ = nondet();
    int M_ = nondet();
    int L_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int U_ = 0;
    int T_ = 1;
    int S_ = 1;
    int R_ = 1;
    int P_ = Q;
    int K_ = D;
    int E_ = 1;
    int A_ = 1 + A;
    if (B >= 1 + A && 0 >= O_) {
     A = A_;
     E = E_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     O = O_;
     P = P_;
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     goto loc_f11;
    }
   }
  goto end;
 }
 loc_f37:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 0;
    int C_ = 0;
    if (E >= 3) {
     C = C_;
     D = D_;
     goto loc_f51;
    }
   }
   if (nondet_bool()) {
    int D_ = 0;
    int C_ = 0;
    if (1 >= E) {
     C = C_;
     D = D_;
     goto loc_f51;
    }
   }
   if (nondet_bool()) {
    int F_ = G;
    int E_ = 2;
    int D_ = 0;
    int C_ = 0;
    if (E == 2) {
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     goto loc_f51;
    }
   }
  goto end;
 }
 loc_f51:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f51;
    }
   }
  goto end;
 }
 loc_f53:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f56;
    }
   }
  goto end;
 }
 loc_f56:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
