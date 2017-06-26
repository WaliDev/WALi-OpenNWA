int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = nondet();
    int C_ = nondet();
    int G_ = 0;
    int F_ = 1;
    int E_ = 0;
    if (D_ >= 0 && C_ >= 1) {
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     G = G_;
     goto loc_f10;
    }
   }
  goto end;
 }
 loc_f10:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = 1;
    if (0 >= G && C >= 1 + G) {
     H = H_;
     goto loc_f14;
    }
   }
   if (nondet_bool()) {
    int H_ = 0;
    if (G >= 1 && C >= 1 + G) {
     H = H_;
     goto loc_f14;
    }
   }
   if (nondet_bool()) {
    if (G >= C) {
     goto loc_f81;
    }
   }
  goto end;
 }
 loc_f14:
 {
 __cost++;
   if (nondet_bool()) {
    int L_ = nondet();
    int J_ = K;
    int I_ = 1;
    if (L_ >= 0 && 1 >= L_ && 1 + G >= C) {
     I = I_;
     J = J_;
     L = L_;
     goto loc_f22;
    }
   }
   if (nondet_bool()) {
    int L_ = nondet();
    int J_ = K;
    int I_ = 0;
    if (L_ >= 0 && 1 >= L_ && C >= 2 + G) {
     I = I_;
     J = J_;
     L = L_;
     goto loc_f22;
    }
   }
  goto end;
 }
 loc_f22:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 1 + E;
    if (0 >= E && 0 >= D && L >= 1) {
     E = E_;
     goto loc_f26;
    }
   }
   if (nondet_bool()) {
    int E_ = 1 + E;
    if (E == 1 && 0 >= D && L >= 1) {
     E = E_;
     goto loc_f26;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int F_ = 1 + F;
    int E_ = 0;
    if (D_ >= 0 && 0 >= D && E >= 2) {
     D = D_;
     E = E_;
     F = F_;
     goto loc_f26;
    }
   }
   if (nondet_bool()) {
    int D_ = -1 + D;
    if (D >= 1) {
     D = D_;
     goto loc_f26;
    }
   }
  goto end;
 }
 loc_f26:
 {
 __cost++;
   if (nondet_bool()) {
    int N_ = J;
    int M_ = 1;
    int B_ = I;
    int A_ = H;
    if (L >= 1 && M >= 1) {
     A = A_;
     B = B_;
     M = M_;
     N = N_;
     goto loc_f41;
    }
   }
   if (nondet_bool()) {
    int O_ = J;
    int N_ = J;
    int M_ = 1;
    int B_ = I;
    int A_ = H;
    if (L >= 1 && 0 >= M) {
     A = A_;
     B = B_;
     M = M_;
     N = N_;
     O = O_;
     goto loc_f41;
    }
   }
   if (nondet_bool()) {
    int L_ = nondet();
    if (L_ >= 0 && 1 >= L_ && C >= F && 0 >= L) {
     L = L_;
     goto loc_f22;
    }
   }
  goto end;
 }
 loc_f41:
 {
 __cost++;
   if (nondet_bool()) {
    int O_ = N;
    if (N == O && 0 >= 1 + A) {
     O = O_;
     goto loc_f43;
    }
   }
   if (nondet_bool()) {
    int O_ = N;
    if (N == O && A >= 1) {
     O = O_;
     goto loc_f43;
    }
   }
   if (nondet_bool()) {
    int O_ = N;
    int A_ = 0;
    if (N == O && A == 0) {
     A = A_;
     O = O_;
     goto loc_f47;
    }
   }
   if (nondet_bool()) {
    int Q_ = nondet();
    if (1 >= Q_ && N >= 1 + O && Q_ >= 0) {
     Q = Q_;
     goto loc_f58;
    }
   }
   if (nondet_bool()) {
    int Q_ = nondet();
    if (1 >= Q_ && O >= 1 + N && Q_ >= 0) {
     Q = Q_;
     goto loc_f58;
    }
   }
  goto end;
 }
 loc_f43:
 {
 __cost++;
   if (nondet_bool()) {
    int P_ = 1;
    int B_ = 0;
    if (B == 0) {
     B = B_;
     P = P_;
     goto loc_f45;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + B) {
     goto loc_f47;
    }
   }
   if (nondet_bool()) {
    if (B >= 1) {
     goto loc_f47;
    }
   }
  goto end;
 }
 loc_f45:
 {
 __cost++;
   if (nondet_bool()) {
    int Q_ = nondet();
    int O_ = 0;
    if (1 >= Q_ && O >= 1 && Q_ >= 0) {
     O = O_;
     Q = Q_;
     goto loc_f58;
    }
   }
   if (nondet_bool()) {
    int Q_ = nondet();
    int O_ = 1;
    if (1 >= Q_ && 0 >= O && Q_ >= 0) {
     O = O_;
     Q = Q_;
     goto loc_f58;
    }
   }
  goto end;
 }
 loc_f47:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1) {
     goto loc_f51;
    }
   }
   if (nondet_bool()) {
    int P_ = 2;
    if (0 >= A && 0 >= B) {
     P = P_;
     goto loc_f45;
    }
   }
   if (nondet_bool()) {
    if (0 >= A && B >= 1) {
     goto loc_f51;
    }
   }
  goto end;
 }
 loc_f51:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= B) {
     goto loc_f45;
    }
   }
   if (nondet_bool()) {
    int P_ = 3;
    if (B >= 1) {
     P = P_;
     goto loc_f45;
    }
   }
  goto end;
 }
 loc_f58:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 1 + E;
    if (Q >= 1 && 0 >= D && 0 >= E) {
     E = E_;
     goto loc_f62;
    }
   }
   if (nondet_bool()) {
    int E_ = 1 + E;
    if (E == 1 && Q >= 1 && 0 >= D) {
     E = E_;
     goto loc_f62;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int F_ = 1 + F;
    int E_ = 0;
    if (D_ >= 0 && 0 >= D && E >= 2) {
     D = D_;
     E = E_;
     F = F_;
     goto loc_f62;
    }
   }
   if (nondet_bool()) {
    int D_ = -1 + D;
    if (D >= 1) {
     D = D_;
     goto loc_f62;
    }
   }
  goto end;
 }
 loc_f62:
 {
 __cost++;
   if (nondet_bool()) {
    int L_ = nondet();
    if (1 >= L_ && 0 >= Q && L_ >= 0 && C >= F) {
     L = L_;
     goto loc_f22;
    }
   }
   if (nondet_bool()) {
    int L_ = nondet();
    if (1 >= L_ && Q >= 2 && L_ >= 0 && C >= F) {
     L = L_;
     goto loc_f22;
    }
   }
   if (nondet_bool()) {
    int Q_ = 1;
    int K_ = 0;
    int G_ = 1 + G;
    if (Q == 1 && C >= F && K >= 1) {
     G = G_;
     K = K_;
     Q = Q_;
     goto loc_f10;
    }
   }
   if (nondet_bool()) {
    int Q_ = 1;
    int K_ = 1;
    int G_ = 1 + G;
    if (Q == 1 && C >= F && 0 >= K) {
     G = G_;
     K = K_;
     Q = Q_;
     goto loc_f10;
    }
   }
  goto end;
 }
 loc_f81:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
