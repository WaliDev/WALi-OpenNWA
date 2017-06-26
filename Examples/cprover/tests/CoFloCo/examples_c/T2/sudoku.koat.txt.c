int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T, int U, int V, int W) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = nondet();
    int D_ = nondet();
    int H_ = 0;
    int F_ = 1;
    int E_ = 1;
    int C_ = 3;
    int B_ = 1;
    int A_ = 1;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     G = G_;
     H = H_;
     goto loc_f26;
    }
   }
  goto end;
 }
 loc_f101:
 {
 __cost++;
   if (nondet_bool()) {
    int R_ = 0;
    if (C >= 1 + I) {
     R = R_;
     goto loc_f104;
    }
   }
   if (nondet_bool()) {
    int H_ = 1 + H;
    if (I >= C) {
     H = H_;
     goto loc_f98;
    }
   }
  goto end;
 }
 loc_f104:
 {
 __cost++;
   if (nondet_bool()) {
    int S_ = 0;
    if (C >= 1 + R) {
     S = S_;
     goto loc_f107;
    }
   }
   if (nondet_bool()) {
    int I_ = 1 + I;
    if (R >= C) {
     I = I_;
     goto loc_f101;
    }
   }
  goto end;
 }
 loc_f107:
 {
 __cost++;
   if (nondet_bool()) {
    int T_ = 0;
    if (C >= 1 + S) {
     T = T_;
     goto loc_f110;
    }
   }
   if (nondet_bool()) {
    int R_ = 1 + R;
    if (S >= C) {
     R = R_;
     goto loc_f104;
    }
   }
  goto end;
 }
 loc_f110:
 {
 __cost++;
   if (nondet_bool()) {
    int U_ = 0;
    if (C >= 1 + T) {
     U = U_;
     goto loc_f113;
    }
   }
   if (nondet_bool()) {
    int S_ = 1 + S;
    if (T >= C) {
     S = S_;
     goto loc_f107;
    }
   }
  goto end;
 }
 loc_f113:
 {
 __cost++;
   if (nondet_bool()) {
    if (C >= 1 + U && T * C + U >= 1 + R * C + S && 0 >= 1 + F) {
     goto loc_f117;
    }
   }
   if (nondet_bool()) {
    if (C >= 1 + U && T * C + U >= 1 + R * C + S && F >= 1) {
     goto loc_f117;
    }
   }
   if (nondet_bool()) {
    int U_ = 1 + U;
    if (C >= 1 + U && R * C + S >= T * C + U) {
     U = U_;
     goto loc_f113;
    }
   }
   if (nondet_bool()) {
    int V_ = 0;
    int U_ = 1 + U;
    int F_ = 0;
    if (F == 0 && C >= 1 + U && T * C + U >= 1 + R * C + S) {
     F = F_;
     U = U_;
     V = V_;
     goto loc_f113;
    }
   }
   if (nondet_bool()) {
    int T_ = 1 + T;
    if (U >= C) {
     T = T_;
     goto loc_f110;
    }
   }
  goto end;
 }
 loc_f117:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int V_ = 1;
    int U_ = 1 + U;
    int F_ = 1;
    if (Y_0 >= 1 + Y_1) {
     F = F_;
     U = U_;
     V = V_;
     goto loc_f113;
    }
   }
   if (nondet_bool()) {
    int V_ = 1;
    int U_ = 1 + U;
    int F_ = 1;
    if (1 >= 0) {
     F = F_;
     U = U_;
     V = V_;
     goto loc_f113;
    }
   }
   if (nondet_bool()) {
    int V_ = 0;
    int U_ = 1 + U;
    int F_ = 0;
    if (1 >= 0) {
     F = F_;
     U = U_;
     V = V_;
     goto loc_f113;
    }
   }
  goto end;
 }
 loc_f135:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + A) {
     goto loc_f136;
    }
   }
   if (nondet_bool()) {
    if (A >= 1) {
     goto loc_f136;
    }
   }
   if (nondet_bool()) {
    int W_ = 1;
    int A_ = 0;
    if (A == 0) {
     A = A_;
     W = W_;
     goto loc_f146;
    }
   }
  goto end;
 }
 loc_f136:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + B) {
     goto loc_f137;
    }
   }
   if (nondet_bool()) {
    if (B >= 1) {
     goto loc_f137;
    }
   }
   if (nondet_bool()) {
    int W_ = 1;
    int B_ = 0;
    if (B == 0) {
     B = B_;
     W = W_;
     goto loc_f146;
    }
   }
  goto end;
 }
 loc_f137:
 {
 __cost++;
   if (nondet_bool()) {
    int W_ = 0;
    if (0 >= 1 + F) {
     W = W_;
     goto loc_f146;
    }
   }
   if (nondet_bool()) {
    int W_ = 0;
    if (F >= 1) {
     W = W_;
     goto loc_f146;
    }
   }
   if (nondet_bool()) {
    int W_ = 1;
    int F_ = 0;
    if (F == 0) {
     F = F_;
     W = W_;
     goto loc_f146;
    }
   }
  goto end;
 }
 loc_f26:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = 0;
    if (D >= 1 + H) {
     I = I_;
     goto loc_f29;
    }
   }
   if (nondet_bool()) {
    int H_ = 0;
    if (H >= D) {
     H = H_;
     goto loc_f38;
    }
   }
  goto end;
 }
 loc_f29:
 {
 __cost++;
   if (nondet_bool()) {
    int J_ = nondet();
    int I_ = 1 + I;
    if (D >= 1 + I) {
     I = I_;
     J = J_;
     goto loc_f29;
    }
   }
   if (nondet_bool()) {
    int H_ = 1 + H;
    if (I >= D) {
     H = H_;
     goto loc_f26;
    }
   }
  goto end;
 }
 loc_f38:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = 0;
    if (D >= 1 + H) {
     I = I_;
     goto loc_f41;
    }
   }
   if (nondet_bool()) {
    int H_ = 0;
    if (H >= D) {
     H = H_;
     goto loc_f56;
    }
   }
  goto end;
 }
 loc_f41:
 {
 __cost++;
   if (nondet_bool()) {
    if (D >= 1 + I && 0 >= 1 + E) {
     goto loc_f44;
    }
   }
   if (nondet_bool()) {
    if (D >= 1 + I && E >= 1) {
     goto loc_f44;
    }
   }
   if (nondet_bool()) {
    int K_ = 0;
    int I_ = 1 + I;
    int E_ = 0;
    if (E == 0 && D >= 1 + I) {
     E = E_;
     I = I_;
     K = K_;
     goto loc_f41;
    }
   }
   if (nondet_bool()) {
    int H_ = 1 + H;
    if (I >= D) {
     H = H_;
     goto loc_f38;
    }
   }
  goto end;
 }
 loc_f44:
 {
 __cost++;
   if (nondet_bool()) {
    int K_ = 1;
    int I_ = 1 + I;
    int E_ = 1;
    if (1 >= 0) {
     E = E_;
     I = I_;
     K = K_;
     goto loc_f41;
    }
   }
   if (nondet_bool()) {
    int K_ = 0;
    int I_ = 1 + I;
    int E_ = 0;
    if (1 >= 0) {
     E = E_;
     I = I_;
     K = K_;
     goto loc_f41;
    }
   }
  goto end;
 }
 loc_f56:
 {
 __cost++;
   if (nondet_bool()) {
    int L_ = 0;
    if (D >= 1 + H) {
     L = L_;
     goto loc_f59;
    }
   }
   if (nondet_bool()) {
    int I_ = 0;
    if (H >= D) {
     I = I_;
     goto loc_f77;
    }
   }
  goto end;
 }
 loc_f59:
 {
 __cost++;
   if (nondet_bool()) {
    int M_ = 1 + L;
    if (D >= 2 + L) {
     M = M_;
     goto loc_f62;
    }
   }
   if (nondet_bool()) {
    int H_ = 1 + H;
    if (1 + L >= D) {
     H = H_;
     goto loc_f56;
    }
   }
  goto end;
 }
 loc_f62:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + A && D >= 1 + M) {
     goto loc_f65;
    }
   }
   if (nondet_bool()) {
    if (A >= 1 && D >= 1 + M) {
     goto loc_f65;
    }
   }
   if (nondet_bool()) {
    int N_ = 0;
    int M_ = 1 + M;
    int A_ = 0;
    if (A == 0 && D >= 1 + M) {
     A = A_;
     M = M_;
     N = N_;
     goto loc_f62;
    }
   }
   if (nondet_bool()) {
    int L_ = 1 + L;
    if (M >= D) {
     L = L_;
     goto loc_f59;
    }
   }
  goto end;
 }
 loc_f65:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int N_ = 1;
    int M_ = 1 + M;
    int A_ = 1;
    if (Y_0 >= 1 + Y_1) {
     A = A_;
     M = M_;
     N = N_;
     goto loc_f62;
    }
   }
   if (nondet_bool()) {
    int N_ = 1;
    int M_ = 1 + M;
    int A_ = 1;
    if (1 >= 0) {
     A = A_;
     M = M_;
     N = N_;
     goto loc_f62;
    }
   }
   if (nondet_bool()) {
    int N_ = 0;
    int M_ = 1 + M;
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     M = M_;
     N = N_;
     goto loc_f62;
    }
   }
  goto end;
 }
 loc_f77:
 {
 __cost++;
   if (nondet_bool()) {
    int O_ = 0;
    if (D >= 1 + I) {
     O = O_;
     goto loc_f80;
    }
   }
   if (nondet_bool()) {
    int H_ = 0;
    if (I >= D) {
     H = H_;
     goto loc_f98;
    }
   }
  goto end;
 }
 loc_f80:
 {
 __cost++;
   if (nondet_bool()) {
    int P_ = 1 + O;
    if (D >= 2 + O) {
     P = P_;
     goto loc_f83;
    }
   }
   if (nondet_bool()) {
    int I_ = 1 + I;
    if (1 + O >= D) {
     I = I_;
     goto loc_f77;
    }
   }
  goto end;
 }
 loc_f83:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + B && D >= 1 + P) {
     goto loc_f86;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 && D >= 1 + P) {
     goto loc_f86;
    }
   }
   if (nondet_bool()) {
    int Q_ = 0;
    int P_ = 1 + P;
    int B_ = 0;
    if (B == 0 && D >= 1 + P) {
     B = B_;
     P = P_;
     Q = Q_;
     goto loc_f83;
    }
   }
   if (nondet_bool()) {
    int O_ = 1 + O;
    if (P >= D) {
     O = O_;
     goto loc_f80;
    }
   }
  goto end;
 }
 loc_f86:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Q_ = 1;
    int P_ = 1 + P;
    int B_ = 1;
    if (Y_0 >= 1 + Y_1) {
     B = B_;
     P = P_;
     Q = Q_;
     goto loc_f83;
    }
   }
   if (nondet_bool()) {
    int Q_ = 1;
    int P_ = 1 + P;
    int B_ = 1;
    if (1 >= 0) {
     B = B_;
     P = P_;
     Q = Q_;
     goto loc_f83;
    }
   }
   if (nondet_bool()) {
    int Q_ = 0;
    int P_ = 1 + P;
    int B_ = 0;
    if (1 >= 0) {
     B = B_;
     P = P_;
     Q = Q_;
     goto loc_f83;
    }
   }
  goto end;
 }
 loc_f98:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = 0;
    if (C >= 1 + H) {
     I = I_;
     goto loc_f101;
    }
   }
   if (nondet_bool()) {
    if (H >= C && 0 >= 1 + E) {
     goto loc_f135;
    }
   }
   if (nondet_bool()) {
    if (H >= C && E >= 1) {
     goto loc_f135;
    }
   }
   if (nondet_bool()) {
    int W_ = 1;
    int E_ = 0;
    if (E == 0 && H >= C) {
     E = E_;
     W = W_;
     goto loc_f146;
    }
   }
  goto end;
 }
 loc_f146:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
