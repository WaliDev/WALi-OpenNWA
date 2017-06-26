int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = nondet();
    int E_ = 0;
    int D_ = 1;
    int C_ = 3;
    int A_ = 3;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f12;
    }
   }
  goto end;
 }
 loc_f12:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = nondet();
    int E_ = 1 + E;
    if (C >= 1 + E) {
     D = D_;
     E = E_;
     goto loc_f12;
    }
   }
   if (nondet_bool()) {
    int S_ = nondet();
    int R_ = D;
    int Q_ = D;
    int H_ = 1;
    int G_ = 0;
    int F_ = A;
    if (E >= C) {
     F = F_;
     G = G_;
     H = H_;
     Q = Q_;
     R = R_;
     S = S_;
     goto loc_f24;
    }
   }
  goto end;
 }
 loc_f24:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = nondet();
    int G_ = 1 + G;
    if (F >= 1 + G) {
     G = G_;
     H = H_;
     goto loc_f24;
    }
   }
   if (nondet_bool()) {
    int P_ = nondet();
    int O_ = H;
    int N_ = H;
    int K_ = 1;
    int J_ = 0;
    int I_ = A;
    if (G >= F) {
     I = I_;
     J = J_;
     K = K_;
     N = N_;
     O = O_;
     P = P_;
     goto loc_f36;
    }
   }
  goto end;
 }
 loc_f36:
 {
 __cost++;
   if (nondet_bool()) {
    int K_ = nondet();
    int J_ = 1 + J;
    if (I >= 1 + J) {
     J = J_;
     K = K_;
     goto loc_f36;
    }
   }
   if (nondet_bool()) {
    int M_ = K;
    int L_ = K;
    if (J >= I) {
     L = L_;
     M = M_;
     goto loc_f46;
    }
   }
  goto end;
 }
 loc_f46:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
