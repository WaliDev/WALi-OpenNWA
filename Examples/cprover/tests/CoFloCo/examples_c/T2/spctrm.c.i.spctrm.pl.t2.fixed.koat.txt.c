int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T, int U, int V, int W, int X, int Y, int Z, int A1) {
 goto loc_f2;

 loc_f103:
 {
 __cost++;
   if (nondet_bool()) {
    int L_ = nondet();
    int X_ = 0;
    if (L == 0) {
     L = L_;
     X = X_;
     goto loc_f107;
    }
   }
   if (nondet_bool()) {
    int L_ = nondet();
    int X_ = L*L;
    if (0 >= 1 + L) {
     L = L_;
     X = X_;
     goto loc_f107;
    }
   }
   if (nondet_bool()) {
    int L_ = nondet();
    int X_ = L*L;
    if (L >= 1) {
     L = L_;
     X = X_;
     goto loc_f107;
    }
   }
  goto end;
 }
 loc_f107:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_ = 0;
    int L_ = 0;
    int J_ = 1 + J;
    if (L == 0) {
     J = J_;
     L = L_;
     Y = Y_;
     goto loc_f91;
    }
   }
   if (nondet_bool()) {
    int Y_ = L*L;
    int J_ = 1 + J;
    if (0 >= 1 + L) {
     J = J_;
     Y = Y_;
     goto loc_f91;
    }
   }
   if (nondet_bool()) {
    int Y_ = L*L;
    int J_ = 1 + J;
    if (L >= 1) {
     J = J_;
     Y = Y_;
     goto loc_f91;
    }
   }
  goto end;
 }
 loc_f118:
 {
 __cost++;
   if (nondet_bool()) {
    int J_ = 1 + J;
    if (A >= J) {
     J = J_;
     goto loc_f118;
    }
   }
   if (nondet_bool()) {
    if (J >= 1 + A) {
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f13:
 {
 __cost++;
   if (nondet_bool()) {
    int M_ = 0;
    int L_ = 0;
    int K_ = 1;
    int J_ = 1 + J;
    if (D >= J) {
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f13;
    }
   }
   if (nondet_bool()) {
    int K_ = nondet();
    int M_ = 1 + -2 * K_ + K_*K_;
    int L_ = 1 + -K_;
    int J_ = 1 + J;
    int B_ = 1 + B + -2 * K_ + K_*K_;
    if (D >= J && K_ >= 2) {
     B = B_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f13;
    }
   }
   if (nondet_bool()) {
    int K_ = nondet();
    int M_ = 1 + -2 * K_ + K_*K_;
    int L_ = 1 + -K_;
    int J_ = 1 + J;
    int B_ = 1 + B + -2 * K_ + K_*K_;
    if (D >= J && 0 >= K_) {
     B = B_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f13;
    }
   }
   if (nondet_bool()) {
    if (J >= 1 + D) {
     goto loc_f24;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int I_ = nondet();
    int H_ = A;
    int G_ = 4 + 4 * A;
    int F_ = 3 + 4 * A;
    int E_ = 4 * A;
    int D_ = 2 * A;
    int C_ = 0;
    int B_ = 0;
    if (Y_0 >= I_ && A * Y_0 + Y_0 >= 2 && 1 >= A * Y_0 && I_ >= Y_1 && A * Y_1 + Y_1 >= 2 && 1 >= A * Y_1) {
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     goto loc_f13;
    }
   }
  goto end;
 }
 loc_f24:
 {
 __cost++;
   if (nondet_bool()) {
    int J_ = 1 + J;
    if (A >= J) {
     J = J_;
     goto loc_f24;
    }
   }
   if (nondet_bool()) {
    if (J >= 1 + A && 0 >= 1 + P) {
     goto loc_f31;
    }
   }
   if (nondet_bool()) {
    if (J >= 1 + A && P >= 1) {
     goto loc_f31;
    }
   }
   if (nondet_bool()) {
    int P_ = 0;
    if (P == 0 && J >= 1 + A) {
     P = P_;
     goto loc_f37;
    }
   }
  goto end;
 }
 loc_f31:
 {
 __cost++;
   if (nondet_bool()) {
    int J_ = 1 + J;
    if (A >= J) {
     J = J_;
     goto loc_f31;
    }
   }
   if (nondet_bool()) {
    if (J >= 1 + A) {
     goto loc_f37;
    }
   }
  goto end;
 }
 loc_f37:
 {
 __cost++;
   if (nondet_bool()) {
    if (N >= O) {
     goto loc_f40;
    }
   }
   if (nondet_bool()) {
    int C_ = C * E;
    if (O >= 1 + N) {
     C = C_;
     goto loc_f118;
    }
   }
  goto end;
 }
 loc_f40:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= Q && 0 >= 1 + P) {
     goto loc_f44;
    }
   }
   if (nondet_bool()) {
    if (0 >= Q && P >= 1) {
     goto loc_f44;
    }
   }
   if (nondet_bool()) {
    int P_ = 0;
    if (P == 0 && 0 >= Q) {
     P = P_;
     goto loc_f64;
    }
   }
   if (nondet_bool()) {
    if (Q >= 1) {
     goto loc_f71;
    }
   }
  goto end;
 }
 loc_f44:
 {
 __cost++;
   if (nondet_bool()) {
    int J_ = 1 + J;
    if (A >= J) {
     J = J_;
     goto loc_f44;
    }
   }
   if (nondet_bool()) {
    if (J >= 1 + A) {
     goto loc_f50;
    }
   }
  goto end;
 }
 loc_f50:
 {
 __cost++;
   if (nondet_bool()) {
    int J_ = 1 + J;
    if (A >= J) {
     J = J_;
     goto loc_f50;
    }
   }
   if (nondet_bool()) {
    int A1_ = Q + D;
    if (J >= 1 + A) {
     A1 = A1_;
     goto loc_f57;
    }
   }
  goto end;
 }
 loc_f57:
 {
 __cost++;
   if (nondet_bool()) {
    int J_ = 1 + J;
    if (A >= J) {
     J = J_;
     goto loc_f57;
    }
   }
   if (nondet_bool()) {
    int Q_ = 1 + Q;
    if (J >= 1 + A) {
     Q = Q_;
     goto loc_f40;
    }
   }
  goto end;
 }
 loc_f64:
 {
 __cost++;
   if (nondet_bool()) {
    int J_ = 2 + J;
    if (E >= J) {
     J = J_;
     goto loc_f64;
    }
   }
   if (nondet_bool()) {
    int Q_ = 1 + Q;
    if (J >= 1 + E) {
     Q = Q_;
     goto loc_f40;
    }
   }
  goto end;
 }
 loc_f71:
 {
 __cost++;
   if (nondet_bool()) {
    int S_ = nondet();
    int T_ = 1 + -S_;
    int R_ = 2 * J;
    int J_ = 1 + J;
    if (D >= J) {
     J = J_;
     R = R_;
     S = S_;
     T = T_;
     goto loc_f71;
    }
   }
   if (nondet_bool()) {
    int L_ = nondet();
    int Z_ = 0;
    if (J >= 1 + D) {
     L = L_;
     Z = Z_;
     goto loc_f86;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int L_ = nondet();
    int Z_ = Y_0*Y_0;
    if (J >= 1 + D && 0 >= 1 + Y_0) {
     L = L_;
     Z = Z_;
     goto loc_f86;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int L_ = nondet();
    int Z_ = Y_0*Y_0;
    if (J >= 1 + D && Y_0 >= 1) {
     L = L_;
     Z = Z_;
     goto loc_f86;
    }
   }
  goto end;
 }
 loc_f86:
 {
 __cost++;
   if (nondet_bool()) {
    int U_ = 0;
    int L_ = 0;
    if (L == 0) {
     L = L_;
     U = U_;
     goto loc_f91;
    }
   }
   if (nondet_bool()) {
    int U_ = L*L;
    if (0 >= 1 + L) {
     U = U_;
     goto loc_f91;
    }
   }
   if (nondet_bool()) {
    int U_ = L*L;
    if (L >= 1) {
     U = U_;
     goto loc_f91;
    }
   }
  goto end;
 }
 loc_f91:
 {
 __cost++;
   if (nondet_bool()) {
    int L_ = nondet();
    int V_ = 0;
    int R_ = 2 * J;
    if (A >= J) {
     L = L_;
     R = R_;
     V = V_;
     goto loc_f99;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int L_ = nondet();
    int V_ = Y_0*Y_0;
    int R_ = 2 * J;
    if (A >= J && 0 >= 1 + Y_0) {
     L = L_;
     R = R_;
     V = V_;
     goto loc_f99;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int L_ = nondet();
    int V_ = Y_0*Y_0;
    int R_ = 2 * J;
    if (A >= J && Y_0 >= 1) {
     L = L_;
     R = R_;
     V = V_;
     goto loc_f99;
    }
   }
   if (nondet_bool()) {
    int O_ = 1 + O;
    int C_ = C + B;
    if (J >= 1 + A) {
     C = C_;
     O = O_;
     goto loc_f37;
    }
   }
  goto end;
 }
 loc_f99:
 {
 __cost++;
   if (nondet_bool()) {
    int L_ = nondet();
    int W_ = 0;
    if (L == 0) {
     L = L_;
     W = W_;
     goto loc_f103;
    }
   }
   if (nondet_bool()) {
    int L_ = nondet();
    int W_ = L*L;
    if (0 >= 1 + L) {
     L = L_;
     W = W_;
     goto loc_f103;
    }
   }
   if (nondet_bool()) {
    int L_ = nondet();
    int W_ = L*L;
    if (L >= 1) {
     L = L_;
     W = W_;
     goto loc_f103;
    }
   }
  goto end;
 }
 loc_f1:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
