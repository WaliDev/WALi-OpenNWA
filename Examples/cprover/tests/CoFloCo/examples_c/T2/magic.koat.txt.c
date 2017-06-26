int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T, int U, int V) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int L_ = nondet();
    int F_ = nondet();
    int E_ = nondet();
    int M_ = 0;
    int K_ = 0;
    int J_ = 0;
    int I_ = 0;
    int H_ = 0;
    int G_ = 0;
    int D_ = 4;
    int C_ = 1;
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
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f25;
    }
   }
  goto end;
 }
 loc_f102:
 {
 __cost++;
   if (nondet_bool()) {
    int M_ = 0;
    int K_ = 0;
    if (D >= 1 + O) {
     K = K_;
     M = M_;
     goto loc_f106;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Y_2 = nondet();
    int Y_3 = nondet();
    int Y_4 = nondet();
    int Y_5 = nondet();
    int Y_6 = nondet();
    int Y_7 = nondet();
    int V_ = 1;
    int G_ = 1;
    if (Y_0 >= 1 + Y_1 && O >= D && Y_2 >= 1 + Y_3 && Y_4 >= 1 + Y_5 && Y_6 >= 1 + Y_7) {
     G = G_;
     V = V_;
     goto loc_f129;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Y_2 = nondet();
    int Y_3 = nondet();
    int Y_4 = nondet();
    int Y_5 = nondet();
    int V_ = 0;
    int G_ = 0;
    if (Y_0 >= 1 + Y_1 && O >= D && Y_2 >= 1 + Y_3 && Y_4 >= 1 + Y_5) {
     G = G_;
     V = V_;
     goto loc_f129;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Y_2 = nondet();
    int Y_3 = nondet();
    int V_ = 0;
    int G_ = 0;
    if (Y_0 >= 1 + Y_1 && O >= D && Y_2 >= 1 + Y_3) {
     G = G_;
     V = V_;
     goto loc_f129;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int V_ = 0;
    int G_ = 0;
    if (Y_0 >= 1 + Y_1 && O >= D) {
     G = G_;
     V = V_;
     goto loc_f129;
    }
   }
   if (nondet_bool()) {
    int V_ = 0;
    int G_ = 0;
    if (O >= D) {
     G = G_;
     V = V_;
     goto loc_f129;
    }
   }
  goto end;
 }
 loc_f106:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int M_ = 1 + M;
    int K_ = K + Y_0;
    if (D >= 1 + M) {
     K = K_;
     M = M_;
     goto loc_f106;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + C && M >= D) {
     goto loc_f112;
    }
   }
   if (nondet_bool()) {
    if (C >= 1 && M >= D) {
     goto loc_f112;
    }
   }
   if (nondet_bool()) {
    int T_ = 0;
    int O_ = 1 + O;
    int C_ = 0;
    if (C == 0 && M >= D) {
     C = C_;
     O = O_;
     T = T_;
     goto loc_f102;
    }
   }
  goto end;
 }
 loc_f112:
 {
 __cost++;
   if (nondet_bool()) {
    int T_ = 1;
    int O_ = 1 + O;
    int K_ = F;
    int C_ = 1;
    if (F == K) {
     C = C_;
     K = K_;
     O = O_;
     T = T_;
     goto loc_f102;
    }
   }
   if (nondet_bool()) {
    int T_ = 0;
    int O_ = 1 + O;
    int C_ = 0;
    if (F >= 1 + K) {
     C = C_;
     O = O_;
     T = T_;
     goto loc_f102;
    }
   }
   if (nondet_bool()) {
    int T_ = 0;
    int O_ = 1 + O;
    int C_ = 0;
    if (K >= 1 + F) {
     C = C_;
     O = O_;
     T = T_;
     goto loc_f102;
    }
   }
  goto end;
 }
 loc_f129:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + A) {
     goto loc_f130;
    }
   }
   if (nondet_bool()) {
    if (A >= 1) {
     goto loc_f130;
    }
   }
   if (nondet_bool()) {
    int U_ = 1;
    int A_ = 0;
    if (A == 0) {
     A = A_;
     U = U_;
     goto loc_f141;
    }
   }
  goto end;
 }
 loc_f130:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + B) {
     goto loc_f131;
    }
   }
   if (nondet_bool()) {
    if (B >= 1) {
     goto loc_f131;
    }
   }
   if (nondet_bool()) {
    int U_ = 1;
    int B_ = 0;
    if (B == 0) {
     B = B_;
     U = U_;
     goto loc_f141;
    }
   }
  goto end;
 }
 loc_f131:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + C) {
     goto loc_f132;
    }
   }
   if (nondet_bool()) {
    if (C >= 1) {
     goto loc_f132;
    }
   }
   if (nondet_bool()) {
    int U_ = 1;
    int C_ = 0;
    if (C == 0) {
     C = C_;
     U = U_;
     goto loc_f141;
    }
   }
  goto end;
 }
 loc_f132:
 {
 __cost++;
   if (nondet_bool()) {
    int U_ = 0;
    if (0 >= 1 + G) {
     U = U_;
     goto loc_f141;
    }
   }
   if (nondet_bool()) {
    int U_ = 0;
    if (G >= 1) {
     U = U_;
     goto loc_f141;
    }
   }
   if (nondet_bool()) {
    int U_ = 1;
    int G_ = 0;
    if (G == 0) {
     G = G_;
     U = U_;
     goto loc_f141;
    }
   }
  goto end;
 }
 loc_f25:
 {
 __cost++;
   if (nondet_bool()) {
    int M_ = 1 + M;
    if (E >= 1 + M) {
     M = M_;
     goto loc_f25;
    }
   }
   if (nondet_bool()) {
    int M_ = 0;
    if (M >= E) {
     M = M_;
     goto loc_f31;
    }
   }
  goto end;
 }
 loc_f31:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + A && E >= 1 + M) {
     goto loc_f34;
    }
   }
   if (nondet_bool()) {
    if (A >= 1 && E >= 1 + M) {
     goto loc_f34;
    }
   }
   if (nondet_bool()) {
    int N_ = 0;
    int M_ = 1 + M;
    int A_ = 0;
    if (A == 0 && E >= 1 + M) {
     A = A_;
     M = M_;
     N = N_;
     goto loc_f31;
    }
   }
   if (nondet_bool()) {
    int M_ = 0;
    if (M >= E) {
     M = M_;
     goto loc_f44;
    }
   }
  goto end;
 }
 loc_f34:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int N_ = 1;
    int M_ = 1 + M;
    int A_ = 1;
    if (Y_0 >= 1) {
     A = A_;
     M = M_;
     N = N_;
     goto loc_f31;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int N_ = 0;
    int M_ = 1 + M;
    int A_ = 0;
    if (Y_0 >= 1) {
     A = A_;
     M = M_;
     N = N_;
     goto loc_f31;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int N_ = 0;
    int M_ = 1 + M;
    int A_ = 0;
    if (0 >= Y_0) {
     A = A_;
     M = M_;
     N = N_;
     goto loc_f31;
    }
   }
  goto end;
 }
 loc_f44:
 {
 __cost++;
   if (nondet_bool()) {
    int O_ = 1 + M;
    if (E >= 1 + M) {
     O = O_;
     goto loc_f47;
    }
   }
   if (nondet_bool()) {
    int M_ = 0;
    if (M >= E) {
     M = M_;
     goto loc_f60;
    }
   }
  goto end;
 }
 loc_f47:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + B && E >= 1 + O) {
     goto loc_f50;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 && E >= 1 + O) {
     goto loc_f50;
    }
   }
   if (nondet_bool()) {
    int P_ = 0;
    int O_ = 1 + O;
    int B_ = 0;
    if (B == 0 && E >= 1 + O) {
     B = B_;
     O = O_;
     P = P_;
     goto loc_f47;
    }
   }
   if (nondet_bool()) {
    int M_ = 1 + M;
    if (O >= E) {
     M = M_;
     goto loc_f44;
    }
   }
  goto end;
 }
 loc_f50:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int P_ = 1;
    int O_ = 1 + O;
    int B_ = 1;
    if (Y_0 >= 1 + Y_1) {
     B = B_;
     O = O_;
     P = P_;
     goto loc_f47;
    }
   }
   if (nondet_bool()) {
    int P_ = 1;
    int O_ = 1 + O;
    int B_ = 1;
    if (1 >= 0) {
     B = B_;
     O = O_;
     P = P_;
     goto loc_f47;
    }
   }
   if (nondet_bool()) {
    int P_ = 0;
    int O_ = 1 + O;
    int B_ = 0;
    if (1 >= 0) {
     B = B_;
     O = O_;
     P = P_;
     goto loc_f47;
    }
   }
  goto end;
 }
 loc_f60:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int M_ = 1 + M;
    int H_ = H + Y_0;
    if (D >= 1 + M) {
     H = H_;
     M = M_;
     goto loc_f60;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + C && M >= D) {
     goto loc_f66;
    }
   }
   if (nondet_bool()) {
    if (C >= 1 && M >= D) {
     goto loc_f66;
    }
   }
   if (nondet_bool()) {
    int Q_ = 0;
    int M_ = 0;
    int C_ = 0;
    if (C == 0 && M >= D) {
     C = C_;
     M = M_;
     Q = Q_;
     goto loc_f72;
    }
   }
  goto end;
 }
 loc_f66:
 {
 __cost++;
   if (nondet_bool()) {
    int Q_ = 1;
    int M_ = 0;
    int H_ = F;
    int C_ = 1;
    if (F == H) {
     C = C_;
     H = H_;
     M = M_;
     Q = Q_;
     goto loc_f72;
    }
   }
   if (nondet_bool()) {
    int Q_ = 0;
    int M_ = 0;
    int C_ = 0;
    if (F >= 1 + H) {
     C = C_;
     M = M_;
     Q = Q_;
     goto loc_f72;
    }
   }
   if (nondet_bool()) {
    int Q_ = 0;
    int M_ = 0;
    int C_ = 0;
    if (H >= 1 + F) {
     C = C_;
     M = M_;
     Q = Q_;
     goto loc_f72;
    }
   }
  goto end;
 }
 loc_f72:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int M_ = 1 + M;
    int I_ = I + Y_0;
    if (D >= 1 + M) {
     I = I_;
     M = M_;
     goto loc_f72;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + C && M >= D) {
     goto loc_f78;
    }
   }
   if (nondet_bool()) {
    if (C >= 1 && M >= D) {
     goto loc_f78;
    }
   }
   if (nondet_bool()) {
    int R_ = 0;
    int M_ = 0;
    int C_ = 0;
    if (C == 0 && M >= D) {
     C = C_;
     M = M_;
     R = R_;
     goto loc_f84;
    }
   }
  goto end;
 }
 loc_f78:
 {
 __cost++;
   if (nondet_bool()) {
    int R_ = 1;
    int M_ = 0;
    int I_ = F;
    int C_ = 1;
    if (F == I) {
     C = C_;
     I = I_;
     M = M_;
     R = R_;
     goto loc_f84;
    }
   }
   if (nondet_bool()) {
    int R_ = 0;
    int M_ = 0;
    int C_ = 0;
    if (F >= 1 + I) {
     C = C_;
     M = M_;
     R = R_;
     goto loc_f84;
    }
   }
   if (nondet_bool()) {
    int R_ = 0;
    int M_ = 0;
    int C_ = 0;
    if (I >= 1 + F) {
     C = C_;
     M = M_;
     R = R_;
     goto loc_f84;
    }
   }
  goto end;
 }
 loc_f84:
 {
 __cost++;
   if (nondet_bool()) {
    int O_ = 0;
    int J_ = 0;
    if (D >= 1 + M) {
     J = J_;
     O = O_;
     goto loc_f88;
    }
   }
   if (nondet_bool()) {
    int O_ = 0;
    if (M >= D) {
     O = O_;
     goto loc_f102;
    }
   }
  goto end;
 }
 loc_f88:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int O_ = 1 + O;
    int J_ = J + Y_0;
    if (D >= 1 + O) {
     J = J_;
     O = O_;
     goto loc_f88;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + C && O >= D) {
     goto loc_f94;
    }
   }
   if (nondet_bool()) {
    if (C >= 1 && O >= D) {
     goto loc_f94;
    }
   }
   if (nondet_bool()) {
    int S_ = 0;
    int M_ = 1 + M;
    int C_ = 0;
    if (C == 0 && O >= D) {
     C = C_;
     M = M_;
     S = S_;
     goto loc_f84;
    }
   }
  goto end;
 }
 loc_f94:
 {
 __cost++;
   if (nondet_bool()) {
    int S_ = 1;
    int M_ = 1 + M;
    int J_ = F;
    int C_ = 1;
    if (F == J) {
     C = C_;
     J = J_;
     M = M_;
     S = S_;
     goto loc_f84;
    }
   }
   if (nondet_bool()) {
    int S_ = 0;
    int M_ = 1 + M;
    int C_ = 0;
    if (F >= 1 + J) {
     C = C_;
     M = M_;
     S = S_;
     goto loc_f84;
    }
   }
   if (nondet_bool()) {
    int S_ = 0;
    int M_ = 1 + M;
    int C_ = 0;
    if (J >= 1 + F) {
     C = C_;
     M = M_;
     S = S_;
     goto loc_f84;
    }
   }
  goto end;
 }
 loc_f141:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
