int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 3;
    int C_ = 0;
    int B_ = 3;
    int A_ = 3;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     goto loc_f18;
    }
   }
  goto end;
 }
 loc_f10:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= C && 0 >= A) {
     goto loc_f10;
    }
   }
   if (nondet_bool()) {
    int B_ = B + A;
    if (A >= C && A >= 1) {
     B = B_;
     goto loc_f10;
    }
   }
   if (nondet_bool()) {
    int D_ = B;
    if (C >= 1 + A) {
     D = D_;
     goto loc_f18;
    }
   }
  goto end;
 }
 loc_f18:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = 0;
    int H_ = 4;
    int G_ = -3;
    int F_ = D;
    int E_ = D;
    if (1 >= 0) {
     E = E_;
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     goto loc_f25;
    }
   }
  goto end;
 }
 loc_f25:
 {
 __cost++;
   if (nondet_bool()) {
    if (G >= I && 0 >= G) {
     goto loc_f25;
    }
   }
   if (nondet_bool()) {
    int H_ = H + G;
    if (G >= I && G >= 1) {
     H = H_;
     goto loc_f25;
    }
   }
   if (nondet_bool()) {
    int K_ = H;
    if (I >= 1 + G) {
     K = K_;
     goto loc_f33;
    }
   }
  goto end;
 }
 loc_f33:
 {
 __cost++;
   if (nondet_bool()) {
    int O_ = 0;
    int N_ = -6;
    int M_ = 3;
    int L_ = K;
    int J_ = K;
    if (1 >= 0) {
     J = J_;
     L = L_;
     M = M_;
     N = N_;
     O = O_;
     goto loc_f40;
    }
   }
  goto end;
 }
 loc_f40:
 {
 __cost++;
   if (nondet_bool()) {
    if (M >= O && 0 >= M) {
     goto loc_f40;
    }
   }
   if (nondet_bool()) {
    int N_ = N + M;
    if (M >= O && M >= 1) {
     N = N_;
     goto loc_f40;
    }
   }
   if (nondet_bool()) {
    int Q_ = N;
    if (O >= 1 + M) {
     Q = Q_;
     goto loc_f48;
    }
   }
  goto end;
 }
 loc_f48:
 {
 __cost++;
   if (nondet_bool()) {
    int R_ = Q;
    int P_ = Q;
    if (4 >= F) {
     P = P_;
     R = R_;
     goto loc_f52;
    }
   }
   if (nondet_bool()) {
    int R_ = Q;
    int P_ = Q;
    if (F >= 5) {
     P = P_;
     R = R_;
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
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
