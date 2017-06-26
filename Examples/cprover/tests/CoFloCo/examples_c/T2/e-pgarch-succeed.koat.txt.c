int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int L_ = nondet();
    int M_ = K;
    int A_ = 1;
    if (0 >= K) {
     A = A_;
     L = L_;
     M = M_;
     goto loc_f21;
    }
   }
   if (nondet_bool()) {
    int Q_ = nondet();
    int P_ = nondet();
    int O_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    int N_ = 1;
    int M_ = 0;
    int A_ = 1;
    if (K >= 1 && Q_ >= 1) {
     A = A_;
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     O = O_;
     P = P_;
     Q = Q_;
     goto loc_f21;
    }
   }
   if (nondet_bool()) {
    int Q_ = nondet();
    int P_ = nondet();
    int O_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    int N_ = 1;
    int M_ = 0;
    int A_ = 1;
    if (K >= 1 && 0 >= Q_) {
     A = A_;
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     O = O_;
     P = P_;
     Q = Q_;
     goto loc_f41;
    }
   }
  goto end;
 }
 loc_f21:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= A) {
     goto loc_f29;
    }
   }
   if (nondet_bool()) {
    int J_ = nondet();
    int I_ = nondet();
    int C_ = nondet();
    int H_ = 0;
    int A_ = 0;
    if (A >= 1) {
     A = A_;
     C = C_;
     H = H_;
     I = I_;
     J = J_;
     goto loc_f29;
    }
   }
  goto end;
 }
 loc_f29:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = nondet();
    if (A >= 1) {
     B = B_;
     goto loc_f41;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = nondet();
    int E_ = nondet();
    int B_ = nondet();
    int D_ = 0;
    if (999 + C >= G_ && 0 >= A) {
     B = B_;
     D = D_;
     E = E_;
     F = F_;
     G = G_;
     goto loc_f41;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = nondet();
    int E_ = nondet();
    int B_ = nondet();
    int D_ = 0;
    int A_ = 1;
    if (G_ >= 1000 + C && 0 >= A) {
     A = A_;
     B = B_;
     D = D_;
     E = E_;
     F = F_;
     G = G_;
     goto loc_f41;
    }
   }
  goto end;
 }
 loc_f41:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f41;
    }
   }
  goto end;
 }
 loc_f43:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
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
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
