int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T, int U, int V) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int V_ = nondet();
    int E_ = nondet();
    int D_ = nondet();
    int A_ = nondet();
    int U_ = 1;
    int T_ = 3;
    int S_ = 0;
    int H_ = 0;
    if (T == 3 && V_ >= 1 && D_ >= 0) {
     A = A_;
     D = D_;
     E = E_;
     H = H_;
     S = S_;
     T = T_;
     U = U_;
     V = V_;
     goto loc_f20;
    }
   }
   if (nondet_bool()) {
    int V_ = nondet();
    int E_ = nondet();
    int D_ = nondet();
    int A_ = nondet();
    int U_ = 1;
    int S_ = 0;
    int H_ = 0;
    if (V_ >= 1 && D_ >= 0 && 2 >= T) {
     A = A_;
     D = D_;
     E = E_;
     H = H_;
     S = S_;
     U = U_;
     V = V_;
     goto loc_f20;
    }
   }
   if (nondet_bool()) {
    int V_ = nondet();
    int E_ = nondet();
    int D_ = nondet();
    int A_ = nondet();
    int U_ = 1;
    int S_ = 0;
    int H_ = 0;
    if (V_ >= 1 && D_ >= 0 && T >= 4) {
     A = A_;
     D = D_;
     E = E_;
     H = H_;
     S = S_;
     U = U_;
     V = V_;
     goto loc_f20;
    }
   }
  goto end;
 }
 loc_f20:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = D;
    int B_ = 1;
    if (0 >= 1 + A) {
     B = B_;
     C = C_;
     goto loc_f26;
    }
   }
   if (nondet_bool()) {
    int C_ = D;
    int B_ = 1;
    if (A >= 1) {
     B = B_;
     C = C_;
     goto loc_f26;
    }
   }
   if (nondet_bool()) {
    int D_ = 0;
    int C_ = 0;
    int B_ = 1;
    int A_ = 0;
    if (A == 0 && D == 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     goto loc_f26;
    }
   }
   if (nondet_bool()) {
    int C_ = D;
    int B_ = 0;
    int A_ = 0;
    if (A == 0 && 0 >= 1 + D) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f26;
    }
   }
   if (nondet_bool()) {
    int C_ = D;
    int B_ = 0;
    int A_ = 0;
    if (A == 0 && D >= 1) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f26;
    }
   }
  goto end;
 }
 loc_f26:
 {
 __cost++;
   if (nondet_bool()) {
    if (C >= E) {
     goto loc_f69;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    if (E >= 1 + C && 0 >= 1 + F_) {
     F = F_;
     goto loc_f33;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    if (E >= 1 + C && F_ >= 1) {
     F = F_;
     goto loc_f33;
    }
   }
   if (nondet_bool()) {
    int G_ = 0;
    int F_ = 0;
    if (E >= 1 + C) {
     F = F_;
     G = G_;
     goto loc_f33;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = 0;
    int C_ = 1 + C;
    if (E >= 1 + C && 0 >= 1 + G_) {
     C = C_;
     F = F_;
     G = G_;
     goto loc_f26;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = 0;
    int C_ = 1 + C;
    if (E >= 1 + C && G_ >= 1) {
     C = C_;
     F = F_;
     G = G_;
     goto loc_f26;
    }
   }
  goto end;
 }
 loc_f33:
 {
 __cost++;
   if (nondet_bool()) {
    if (H >= I) {
     goto loc_f39;
    }
   }
   if (nondet_bool()) {
    int J_ = -1;
    if (1 + J == 0 && I >= 1 + H) {
     J = J_;
     goto loc_f39;
    }
   }
   if (nondet_bool()) {
    int H_ = 1 + H;
    if (0 >= 2 + J && I >= 1 + H) {
     H = H_;
     goto loc_f33;
    }
   }
   if (nondet_bool()) {
    int H_ = 1 + H;
    if (J >= 0 && I >= 1 + H) {
     H = H_;
     goto loc_f33;
    }
   }
  goto end;
 }
 loc_f39:
 {
 __cost++;
   if (nondet_bool()) {
    if (H >= I) {
     goto loc_f69;
    }
   }
   if (nondet_bool()) {
    int M_ = nondet();
    int L_ = nondet();
    int K_ = 0;
    if (0 >= 1 + L_ && I >= 1 + H) {
     K = K_;
     L = L_;
     M = M_;
     goto loc_f47;
    }
   }
   if (nondet_bool()) {
    int M_ = nondet();
    int L_ = nondet();
    int K_ = 0;
    if (L_ >= 1 && I >= 1 + H) {
     K = K_;
     L = L_;
     M = M_;
     goto loc_f47;
    }
   }
   if (nondet_bool()) {
    int N_ = nondet();
    int L_ = 0;
    int K_ = 0;
    if (I >= 1 + H) {
     K = K_;
     L = L_;
     N = N_;
     goto loc_f50;
    }
   }
   if (nondet_bool()) {
    int K_ = nondet();
    int C_ = 1 + C;
    if (0 >= 1 + K_ && I >= 1 + H) {
     C = C_;
     K = K_;
     goto loc_f26;
    }
   }
   if (nondet_bool()) {
    int K_ = nondet();
    int C_ = 1 + C;
    if (K_ >= 1 && I >= 1 + H) {
     C = C_;
     K = K_;
     goto loc_f26;
    }
   }
  goto end;
 }
 loc_f47:
 {
 __cost++;
   if (nondet_bool()) {
    int N_ = nondet();
    int M_ = 0;
    if (M == 0) {
     M = M_;
     N = N_;
     goto loc_f50;
    }
   }
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (0 >= 1 + M) {
     C = C_;
     goto loc_f26;
    }
   }
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (M >= 1) {
     C = C_;
     goto loc_f26;
    }
   }
  goto end;
 }
 loc_f50:
 {
 __cost++;
   if (nondet_bool()) {
    int Q_ = nondet();
    int P_ = 1;
    int O_ = 3;
    if (O == 3 && 0 >= Q_ && N >= 0) {
     O = O_;
     P = P_;
     Q = Q_;
     goto loc_f69;
    }
   }
   if (nondet_bool()) {
    int Q_ = nondet();
    int P_ = 1;
    int O_ = 3;
    if (O == 3 && Q_ >= 2 && N >= 0) {
     O = O_;
     P = P_;
     Q = Q_;
     goto loc_f69;
    }
   }
   if (nondet_bool()) {
    int R_ = nondet();
    if (2 >= O && N >= 0) {
     R = R_;
     goto loc_f59;
    }
   }
   if (nondet_bool()) {
    int R_ = nondet();
    if (O >= 4 && N >= 0) {
     R = R_;
     goto loc_f59;
    }
   }
   if (nondet_bool()) {
    int R_ = nondet();
    int Q_ = 1;
    int O_ = 3;
    if (O == 3 && N >= 0) {
     O = O_;
     Q = Q_;
     R = R_;
     goto loc_f59;
    }
   }
   if (nondet_bool()) {
    int P_ = 1;
    int C_ = 1 + C;
    if (0 >= 1 + N) {
     C = C_;
     P = P_;
     goto loc_f26;
    }
   }
  goto end;
 }
 loc_f59:
 {
 __cost++;
   if (nondet_bool()) {
    int N_ = nondet();
    if (10 >= R) {
     N = N_;
     goto loc_f62;
    }
   }
   if (nondet_bool()) {
    int N_ = nondet();
    int R_ = 10;
    if (R >= 11) {
     N = N_;
     R = R_;
     goto loc_f62;
    }
   }
  goto end;
 }
 loc_f62:
 {
 __cost++;
   if (nondet_bool()) {
    int P_ = 1;
    int C_ = 1 + C;
    if (0 >= 1 + N) {
     C = C_;
     P = P_;
     goto loc_f26;
    }
   }
   if (nondet_bool()) {
    int S_ = 1 + S;
    int J_ = K;
    int C_ = 1 + C;
    if (N >= 0) {
     C = C_;
     J = J_;
     S = S_;
     goto loc_f26;
    }
   }
  goto end;
 }
 loc_f69:
 {
 __cost++;
   if (nondet_bool()) {
    int S_ = 0;
    int A_ = 0;
    if (S == 0) {
     A = A_;
     S = S_;
     goto loc_f71;
    }
   }
   if (nondet_bool()) {
    int A_ = 0;
    if (0 >= 1 + S) {
     A = A_;
     goto loc_f74;
    }
   }
   if (nondet_bool()) {
    int A_ = 0;
    if (S >= 1) {
     A = A_;
     goto loc_f74;
    }
   }
  goto end;
 }
 loc_f71:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f71;
    }
   }
  goto end;
 }
 loc_f73:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     goto loc_f74;
    }
   }
  goto end;
 }
 loc_f74:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f74;
    }
   }
  goto end;
 }
 loc_f76:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f78;
    }
   }
  goto end;
 }
 loc_f78:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
