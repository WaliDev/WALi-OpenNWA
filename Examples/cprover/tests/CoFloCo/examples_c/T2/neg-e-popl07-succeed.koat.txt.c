int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T, int U) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = nondet();
    int U_ = 0;
    int F_ = 0;
    if (0 >= D_) {
     D = D_;
     F = F_;
     U = U_;
     goto loc_f13;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int U_ = 0;
    int F_ = 0;
    if (D_ >= 1) {
     D = D_;
     F = F_;
     U = U_;
     goto loc_f13;
    }
   }
  goto end;
 }
 loc_f13:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_f45;
    }
   }
   if (nondet_bool()) {
    int N_ = nondet();
    int M_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int H_ = nondet();
    int L_ = E;
    if (B >= 1 + A && N_ >= 1) {
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     goto loc_f45;
    }
   }
   if (nondet_bool()) {
    int S_ = nondet();
    int R_ = nondet();
    int O_ = nondet();
    int N_ = nondet();
    int M_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int H_ = nondet();
    int C_ = nondet();
    int T_ = 0;
    int Q_ = 0;
    int P_ = Q;
    int L_ = E;
    if (S_ >= 2 && 0 >= O_ && B >= 1 + A) {
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
     goto loc_f37;
    }
   }
   if (nondet_bool()) {
    int S_ = nondet();
    int R_ = nondet();
    int O_ = nondet();
    int N_ = nondet();
    int M_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int H_ = nondet();
    int C_ = nondet();
    int T_ = 0;
    int Q_ = 0;
    int P_ = Q;
    int L_ = E;
    if (0 >= S_ && 0 >= O_ && B >= 1 + A) {
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
     goto loc_f37;
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
    int T_ = 0;
    int S_ = 1;
    int R_ = 1;
    int P_ = Q;
    int L_ = E;
    int C_ = 1;
    int A_ = 1 + A;
    if (B >= 1 + A && 0 >= O_) {
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
     R = R_;
     S = S_;
     T = T_;
     goto loc_f13;
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
    int E_ = 1 + E;
    int C_ = 2;
    if (C == 2) {
     C = C_;
     E = E_;
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
    int E_ = 0;
    if (D >= 1) {
     E = E_;
     goto loc_f52;
    }
   }
   if (nondet_bool()) {
    int F_ = 0;
    int E_ = 0;
    if (0 >= D) {
     E = E_;
     F = F_;
     goto loc_f52;
    }
   }
  goto end;
 }
 loc_f52:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f52;
    }
   }
  goto end;
 }
 loc_f54:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f57;
    }
   }
  goto end;
 }
 loc_f57:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
