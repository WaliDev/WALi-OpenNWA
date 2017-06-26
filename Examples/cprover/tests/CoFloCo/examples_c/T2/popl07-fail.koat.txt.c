int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int T_ = 0;
    int S_ = 0;
    if (1 >= 0) {
     S = S_;
     T = T_;
     goto loc_f11;
    }
   }
  goto end;
 }
 loc_f11:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 0;
    if (A >= B) {
     C = C_;
     goto loc_f47;
    }
   }
   if (nondet_bool()) {
    int L_ = nondet();
    int K_ = nondet();
    int I_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int F_ = nondet();
    int J_ = C;
    int C_ = 0;
    if (B >= 1 + A && L_ >= 1) {
     C = C_;
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     goto loc_f47;
    }
   }
   if (nondet_bool()) {
    int Q_ = nondet();
    int P_ = nondet();
    int M_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    int I_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int F_ = nondet();
    int D_ = nondet();
    int R_ = 0;
    int O_ = 0;
    int N_ = O;
    int J_ = C;
    if (Q_ >= 2 && 0 >= M_ && B >= 1 + A) {
     D = D_;
     F = F_;
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
     goto loc_f35;
    }
   }
   if (nondet_bool()) {
    int Q_ = nondet();
    int P_ = nondet();
    int M_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    int I_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int F_ = nondet();
    int D_ = nondet();
    int R_ = 0;
    int O_ = 0;
    int N_ = O;
    int J_ = C;
    if (0 >= Q_ && 0 >= M_ && B >= 1 + A) {
     D = D_;
     F = F_;
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
     goto loc_f35;
    }
   }
   if (nondet_bool()) {
    int M_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    int I_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int F_ = nondet();
    int R_ = 0;
    int Q_ = 1;
    int P_ = 1;
    int N_ = O;
    int J_ = C;
    int D_ = 1;
    int A_ = 1 + A;
    if (B >= 1 + A && 0 >= M_) {
     A = A_;
     D = D_;
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     P = P_;
     Q = Q_;
     R = R_;
     goto loc_f11;
    }
   }
  goto end;
 }
 loc_f35:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 0;
    if (D >= 3) {
     C = C_;
     goto loc_f47;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    if (1 >= D) {
     C = C_;
     goto loc_f47;
    }
   }
   if (nondet_bool()) {
    int E_ = F;
    int D_ = 2;
    int C_ = 0;
    if (D == 2) {
     C = C_;
     D = D_;
     E = E_;
     goto loc_f47;
    }
   }
  goto end;
 }
 loc_f47:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f47;
    }
   }
  goto end;
 }
 loc_f49:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f52;
    }
   }
  goto end;
 }
 loc_f52:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
