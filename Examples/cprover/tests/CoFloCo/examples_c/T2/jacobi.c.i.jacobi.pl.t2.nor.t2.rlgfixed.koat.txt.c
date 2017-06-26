int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T, int U, int V, int W, int X, int Y, int Z, int A1) {
 goto loc_f2;

 loc_f101:
 {
 __cost++;
   if (nondet_bool()) {
    int P_ = nondet();
    int K_ = nondet();
    int A1_ = 1 + A1;
    if (C >= 1 + A1) {
     K = K_;
     P = P_;
     A1 = A1_;
     goto loc_f101;
    }
   }
   if (nondet_bool()) {
    if (A1 >= C) {
     goto loc_f110;
    }
   }
  goto end;
 }
 loc_f110:
 {
 __cost++;
   if (nondet_bool()) {
    int P_ = nondet();
    int K_ = nondet();
    int A1_ = 1 + A1;
    if (A >= A1) {
     K = K_;
     P = P_;
     A1 = A1_;
     goto loc_f110;
    }
   }
   if (nondet_bool()) {
    if (A1 >= 1 + A) {
     goto loc_f119;
    }
   }
  goto end;
 }
 loc_f119:
 {
 __cost++;
   if (nondet_bool()) {
    int P_ = nondet();
    int K_ = nondet();
    int A1_ = 1 + A1;
    if (A >= A1) {
     K = K_;
     P = P_;
     A1 = A1_;
     goto loc_f119;
    }
   }
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (A1 >= 1 + A) {
     C = C_;
     goto loc_f53;
    }
   }
  goto end;
 }
 loc_f132:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (A >= B) {
     B = B_;
     goto loc_f132;
    }
   }
   if (nondet_bool()) {
    int E_ = 1 + E;
    if (B >= 1 + A) {
     E = E_;
     goto loc_f27;
    }
   }
  goto end;
 }
 loc_f17:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = nondet();
    int B_ = 1 + B;
    if (A >= B) {
     B = B_;
     D = D_;
     goto loc_f17;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_f27;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f5;
    }
   }
  goto end;
 }
 loc_f27:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = 0;
    if (50 >= E) {
     F = F_;
     goto loc_f31;
    }
   }
   if (nondet_bool()) {
    if (E >= 51) {
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f31:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1 + B) {
     goto loc_f34;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + F && B >= A) {
     goto loc_f46;
    }
   }
   if (nondet_bool()) {
    if (F >= 1 && B >= A) {
     goto loc_f46;
    }
   }
   if (nondet_bool()) {
    int F_ = 0;
    if (F == 0 && B >= A) {
     F = F_;
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f34:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = F + G_;
    int C_ = 1 + C;
    if (A >= C) {
     C = C_;
     F = F_;
     G = G_;
     goto loc_f34;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (C >= 1 + A) {
     B = B_;
     goto loc_f31;
    }
   }
  goto end;
 }
 loc_f46:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = nondet();
    if (3 >= E) {
     H = H_;
     goto loc_f50;
    }
   }
   if (nondet_bool()) {
    int H_ = 0;
    if (E >= 4) {
     H = H_;
     goto loc_f50;
    }
   }
  goto end;
 }
 loc_f5:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_f8;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_f17;
    }
   }
  goto end;
 }
 loc_f50:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1 + B) {
     goto loc_f53;
    }
   }
   if (nondet_bool()) {
    if (B >= A) {
     goto loc_f132;
    }
   }
  goto end;
 }
 loc_f53:
 {
 __cost++;
   if (nondet_bool()) {
    int N_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int O_ = N_ + K_;
    int M_ = L_ + K_;
    int C_ = 1 + C;
    if (E >= 5 && A >= C) {
     C = C_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     O = O_;
     goto loc_f53;
    }
   }
   if (nondet_bool()) {
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    if (4 >= E && A >= C) {
     I = I_;
     J = J_;
     K = K_;
     goto loc_f69;
    }
   }
   if (nondet_bool()) {
    int O_ = nondet();
    int N_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int M_ = L_ + K_;
    if (O_ >= 1 + N_ + K_ && A >= C && E >= 5) {
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     O = O_;
     goto loc_f69;
    }
   }
   if (nondet_bool()) {
    int O_ = nondet();
    int N_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int M_ = L_ + K_;
    if (N_ + K_ >= 1 + O_ && A >= C && E >= 5) {
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     O = O_;
     goto loc_f69;
    }
   }
   if (nondet_bool()) {
    int M_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    if (M_ >= 1 + L_ + K_ && A >= C && E >= 5) {
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f69;
    }
   }
   if (nondet_bool()) {
    int M_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    if (L_ + K_ >= 1 + M_ && A >= C && E >= 5) {
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f69;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (C >= 1 + A) {
     B = B_;
     goto loc_f50;
    }
   }
  goto end;
 }
 loc_f69:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (H >= I) {
     C = C_;
     goto loc_f53;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int V_ = nondet();
    int U_ = nondet();
    int T_ = nondet();
    int S_ = nondet();
    int R_ = nondet();
    int Q_ = nondet();
    int P_ = Y_0 + -Y_1;
    if (R_ >= 1 + Q_ + K && I >= 1 + H) {
     P = P_;
     Q = Q_;
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     V = V_;
     goto loc_f80;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int V_ = nondet();
    int U_ = nondet();
    int T_ = nondet();
    int S_ = nondet();
    int R_ = nondet();
    int Q_ = nondet();
    int P_ = Y_0 + -Y_1;
    if (Q_ + K >= 1 + R_ && I >= 1 + H) {
     P = P_;
     Q = Q_;
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     V = V_;
     goto loc_f80;
    }
   }
   if (nondet_bool()) {
    int Z_ = nondet();
    int Y_ = nondet();
    int X_ = nondet();
    int W_ = nondet();
    int S_ = nondet();
    int Q_ = nondet();
    int P_ = nondet();
    int R_ = Q_ + K;
    if (I >= 1 + H) {
     P = P_;
     Q = Q_;
     R = R_;
     S = S_;
     W = W_;
     X = X_;
     Y = Y_;
     Z = Z_;
     goto loc_f92;
    }
   }
  goto end;
 }
 loc_f8:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (A >= C) {
     C = C_;
     goto loc_f8;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (C >= 1 + A) {
     B = B_;
     goto loc_f5;
    }
   }
  goto end;
 }
 loc_f80:
 {
 __cost++;
   if (nondet_bool()) {
    int Z_ = nondet();
    int Y_ = nondet();
    int X_ = nondet();
    int W_ = nondet();
    int P_ = nondet();
    if (T >= 0) {
     P = P_;
     W = W_;
     X = X_;
     Y = Y_;
     Z = Z_;
     goto loc_f92;
    }
   }
   if (nondet_bool()) {
    int Z_ = nondet();
    int Y_ = nondet();
    int X_ = nondet();
    int W_ = nondet();
    int P_ = nondet();
    int S_ = -S;
    if (0 >= 1 + T) {
     P = P_;
     S = S_;
     W = W_;
     X = X_;
     Y = Y_;
     Z = Z_;
     goto loc_f92;
    }
   }
  goto end;
 }
 loc_f92:
 {
 __cost++;
   if (nondet_bool()) {
    int P_ = nondet();
    int K_ = nondet();
    int A1_ = 1 + A1;
    if (B >= 1 + A1) {
     K = K_;
     P = P_;
     A1 = A1_;
     goto loc_f92;
    }
   }
   if (nondet_bool()) {
    if (A1 >= B) {
     goto loc_f101;
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
