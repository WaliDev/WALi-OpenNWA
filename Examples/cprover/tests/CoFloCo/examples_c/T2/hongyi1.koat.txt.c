int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q) {
 goto loc_f2;

 loc_f10:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = 1;
    if (0 >= G && D >= 1 + G) {
     H = H_;
     goto loc_f14;
    }
   }
   if (nondet_bool()) {
    int H_ = 0;
    if (G >= 1 && D >= 1 + G) {
     H = H_;
     goto loc_f14;
    }
   }
   if (nondet_bool()) {
    if (G >= D) {
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f14:
 {
 __cost++;
   if (nondet_bool()) {
    int K_ = L;
    int J_ = 1;
    if (1 >= I && 1 + G >= D && I >= 0) {
     J = J_;
     K = K_;
     goto loc_f23;
    }
   }
   if (nondet_bool()) {
    int K_ = L;
    int J_ = 0;
    if (1 >= I && D >= 2 + G && I >= 0) {
     J = J_;
     K = K_;
     goto loc_f23;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = 0;
    int F_ = 1;
    int E_ = 0;
    if (D >= 1 && C >= 0) {
     E = E_;
     F = F_;
     G = G_;
     goto loc_f10;
    }
   }
  goto end;
 }
 loc_f23:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 1 + E;
    if (0 >= E && 0 >= C && I >= 1) {
     E = E_;
     goto loc_f27;
    }
   }
   if (nondet_bool()) {
    int E_ = 1 + E;
    if (E == 1 && 0 >= C && I >= 1) {
     E = E_;
     goto loc_f27;
    }
   }
   if (nondet_bool()) {
    int F_ = 1 + F;
    int E_ = 0;
    if (C == 0 && E >= 2) {
     E = E_;
     F = F_;
     goto loc_f27;
    }
   }
   if (nondet_bool()) {
    int C_ = -1 + C;
    if (C >= 1) {
     C = C_;
     goto loc_f27;
    }
   }
  goto end;
 }
 loc_f27:
 {
 __cost++;
   if (nondet_bool()) {
    int N_ = K;
    int M_ = 1;
    int B_ = J;
    int A_ = H;
    if (I >= 1 && M >= 1) {
     A = A_;
     B = B_;
     M = M_;
     N = N_;
     goto loc_f42;
    }
   }
   if (nondet_bool()) {
    int O_ = K;
    int N_ = K;
    int M_ = 1;
    int B_ = J;
    int A_ = H;
    if (I >= 1 && 0 >= M) {
     A = A_;
     B = B_;
     M = M_;
     N = N_;
     O = O_;
     goto loc_f42;
    }
   }
   if (nondet_bool()) {
    if (I == 0 && D >= F) {
     goto loc_f23;
    }
   }
  goto end;
 }
 loc_f42:
 {
 __cost++;
   if (nondet_bool()) {
    int O_ = N;
    if (N == O && 0 >= 1 + A) {
     O = O_;
     goto loc_f44;
    }
   }
   if (nondet_bool()) {
    int O_ = N;
    if (N == O && A >= 1) {
     O = O_;
     goto loc_f44;
    }
   }
   if (nondet_bool()) {
    int O_ = N;
    int A_ = 0;
    if (N == O && A == 0) {
     A = A_;
     O = O_;
     goto loc_f48;
    }
   }
   if (nondet_bool()) {
    if (1 >= Q && N >= 1 + O && Q >= 0) {
     goto loc_f59;
    }
   }
   if (nondet_bool()) {
    if (1 >= Q && O >= 1 + N && Q >= 0) {
     goto loc_f59;
    }
   }
  goto end;
 }
 loc_f44:
 {
 __cost++;
   if (nondet_bool()) {
    int P_ = 1;
    int B_ = 0;
    if (B == 0) {
     B = B_;
     P = P_;
     goto loc_f46;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + B) {
     goto loc_f48;
    }
   }
   if (nondet_bool()) {
    if (B >= 1) {
     goto loc_f48;
    }
   }
  goto end;
 }
 loc_f46:
 {
 __cost++;
   if (nondet_bool()) {
    int O_ = 0;
    if (1 >= Q && O >= 1 && Q >= 0) {
     O = O_;
     goto loc_f59;
    }
   }
   if (nondet_bool()) {
    int O_ = 1;
    if (1 >= Q && 0 >= O && Q >= 0) {
     O = O_;
     goto loc_f59;
    }
   }
  goto end;
 }
 loc_f48:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1) {
     goto loc_f52;
    }
   }
   if (nondet_bool()) {
    int P_ = 2;
    if (0 >= A && 0 >= B) {
     P = P_;
     goto loc_f46;
    }
   }
   if (nondet_bool()) {
    if (0 >= A && B >= 1) {
     goto loc_f52;
    }
   }
  goto end;
 }
 loc_f52:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= B) {
     goto loc_f46;
    }
   }
   if (nondet_bool()) {
    int P_ = 3;
    if (B >= 1) {
     P = P_;
     goto loc_f46;
    }
   }
  goto end;
 }
 loc_f59:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 1 + E;
    if (0 >= E && 0 >= C && Q >= 1) {
     E = E_;
     goto loc_f63;
    }
   }
   if (nondet_bool()) {
    int E_ = 1 + E;
    if (E == 1 && 0 >= C && Q >= 1) {
     E = E_;
     goto loc_f63;
    }
   }
   if (nondet_bool()) {
    int F_ = 1 + F;
    int E_ = 0;
    if (C == 0 && E >= 2) {
     E = E_;
     F = F_;
     goto loc_f63;
    }
   }
   if (nondet_bool()) {
    int C_ = -1 + C;
    if (C >= 1) {
     C = C_;
     goto loc_f63;
    }
   }
  goto end;
 }
 loc_f63:
 {
 __cost++;
   if (nondet_bool()) {
    if (D >= F && 0 >= Q && 1 >= I && I >= 0) {
     goto loc_f23;
    }
   }
   if (nondet_bool()) {
    if (D >= F && Q >= 2 && 1 >= I && I >= 0) {
     goto loc_f23;
    }
   }
   if (nondet_bool()) {
    int Q_ = 1;
    int L_ = 0;
    int G_ = 1 + G;
    if (Q == 1 && D >= F && L >= 1) {
     G = G_;
     L = L_;
     Q = Q_;
     goto loc_f10;
    }
   }
   if (nondet_bool()) {
    int Q_ = 1;
    int L_ = 1;
    int G_ = 1 + G;
    if (Q == 1 && D >= F && 0 >= L) {
     G = G_;
     L = L_;
     Q = Q_;
     goto loc_f10;
    }
   }
  goto end;
 }
 loc_f1:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
