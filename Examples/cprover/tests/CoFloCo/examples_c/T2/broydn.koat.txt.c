int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T, int U, int V, int W, int X, int Y, int Z, int A1, int B1, int C1) {
 goto loc_start;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = nondet();
    int B_ = 1 + B;
    if (C >= D_ && A >= B) {
     B = B_;
     D = D_;
     goto loc_f0;
    }
   }
   if (nondet_bool()) {
    int E_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int B_ = 1 + B;
    if (D_ >= 1 + C && A >= B) {
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f0;
    }
   }
   if (nondet_bool()) {
    if (0 >= C && B >= 1 + A) {
     goto loc_f43;
    }
   }
   if (nondet_bool()) {
    if (C >= 1 && B >= 1 + A) {
     goto loc_f44;
    }
   }
  goto end;
 }
 loc_f104:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_f106;
    }
   }
   if (nondet_bool()) {
    int Z_ = 1;
    if (B >= 1 + A) {
     Z = Z_;
     goto loc_f115;
    }
   }
  goto end;
 }
 loc_f106:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = nondet();
    int K_ = 1 + K;
    if (A >= K) {
     H = H_;
     K = K_;
     goto loc_f106;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (K >= 1 + A) {
     B = B_;
     goto loc_f104;
    }
   }
  goto end;
 }
 loc_f115:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_f117;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + Z && B >= 1 + A) {
     goto loc_f161;
    }
   }
   if (nondet_bool()) {
    if (Z >= 1 && B >= 1 + A) {
     goto loc_f161;
    }
   }
   if (nondet_bool()) {
    int Z_ = 0;
    if (Z == 0 && B >= 1 + A) {
     Z = Z_;
     goto loc_f132;
    }
   }
  goto end;
 }
 loc_f117:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = nondet();
    int K_ = 1 + K;
    if (A >= K) {
     H = H_;
     K = K_;
     goto loc_f117;
    }
   }
   if (nondet_bool()) {
    int C1_ = nondet();
    int B1_ = nondet();
    int A1_ = nondet();
    int Z_ = 0;
    int B_ = 1 + B;
    if (A1_ >= 0 && K >= 1 + A) {
     B = B_;
     Z = Z_;
     A1 = A1_;
     B1 = B1_;
     C1 = C1_;
     goto loc_f115;
    }
   }
   if (nondet_bool()) {
    int C1_ = nondet();
    int B1_ = nondet();
    int A1_ = nondet();
    int B_ = 1 + B;
    if (0 >= 1 + A1_ && K >= 1 + A) {
     B = B_;
     A1 = A1_;
     B1 = B1_;
     C1 = C1_;
     goto loc_f115;
    }
   }
  goto end;
 }
 loc_f132:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_f134;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_f142;
    }
   }
  goto end;
 }
 loc_f134:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = nondet();
    int K_ = 1 + K;
    if (A >= K) {
     H = H_;
     K = K_;
     goto loc_f134;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (K >= 1 + A) {
     B = B_;
     goto loc_f132;
    }
   }
  goto end;
 }
 loc_f142:
 {
 __cost++;
   if (nondet_bool()) {
    int N_ = 0;
    int F_ = 0;
    int B_ = 1 + B;
    if (A >= B) {
     B = B_;
     F = F_;
     N = N_;
     goto loc_f142;
    }
   }
   if (nondet_bool()) {
    int N_ = nondet();
    int F_ = nondet();
    int O_ = O + N_;
    int B_ = 1 + B;
    if (0 >= 1 + F_ && A >= B) {
     B = B_;
     F = F_;
     N = N_;
     O = O_;
     goto loc_f142;
    }
   }
   if (nondet_bool()) {
    int N_ = nondet();
    int F_ = nondet();
    int O_ = O + N_;
    int B_ = 1 + B;
    if (F_ >= 1 && A >= B) {
     B = B_;
     F = F_;
     N = N_;
     O = O_;
     goto loc_f142;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_f151;
    }
   }
  goto end;
 }
 loc_f151:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (A >= B) {
     B = B_;
     goto loc_f151;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_f156;
    }
   }
  goto end;
 }
 loc_f156:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (A >= B) {
     B = B_;
     goto loc_f156;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_f161;
    }
   }
  goto end;
 }
 loc_f161:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_f163;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_f171;
    }
   }
  goto end;
 }
 loc_f163:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = nondet();
    int K_ = 1 + K;
    if (A >= K) {
     H = H_;
     K = K_;
     goto loc_f163;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (K >= 1 + A) {
     B = B_;
     goto loc_f161;
    }
   }
  goto end;
 }
 loc_f171:
 {
 __cost++;
   if (nondet_bool()) {
    if (B >= 1) {
     goto loc_f173;
    }
   }
   if (nondet_bool()) {
    if (0 >= B) {
     goto loc_f181;
    }
   }
  goto end;
 }
 loc_f173:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = nondet();
    int K_ = 1 + K;
    if (B >= K) {
     H = H_;
     K = K_;
     goto loc_f173;
    }
   }
   if (nondet_bool()) {
    int B_ = -1 + B;
    if (K >= 1 + B) {
     B = B_;
     goto loc_f171;
    }
   }
  goto end;
 }
 loc_f181:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (A >= B) {
     B = B_;
     goto loc_f181;
    }
   }
   if (nondet_bool()) {
    int Y_ = nondet();
    if (B >= 1 + A) {
     Y = Y_;
     goto loc_f188;
    }
   }
  goto end;
 }
 loc_f188:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_f190;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    if (B >= 1 + A) {
     C = C_;
     goto loc_f201;
    }
   }
  goto end;
 }
 loc_f190:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = nondet();
    int K_ = 1 + K;
    if (A >= K) {
     H = H_;
     K = K_;
     goto loc_f190;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (K >= 1 + A) {
     B = B_;
     goto loc_f188;
    }
   }
  goto end;
 }
 loc_f201:
 {
 __cost++;
   if (nondet_bool()) {
    int P_ = nondet();
    int B_ = 1 + B;
    if (C >= P_ && A >= B) {
     B = B_;
     P = P_;
     goto loc_f201;
    }
   }
   if (nondet_bool()) {
    int Q_ = nondet();
    int P_ = nondet();
    int C_ = nondet();
    int B_ = 1 + B;
    if (P_ >= 1 + C && A >= B) {
     B = B_;
     C = C_;
     P = P_;
     Q = Q_;
     goto loc_f201;
    }
   }
   if (nondet_bool()) {
    int X_ = nondet();
    if (0 >= 1 + X_ && B >= 1 + A) {
     X = X_;
     goto loc_f211;
    }
   }
   if (nondet_bool()) {
    int X_ = nondet();
    if (X_ >= 1 && B >= 1 + A) {
     X = X_;
     goto loc_f211;
    }
   }
   if (nondet_bool()) {
    int X_ = 0;
    int I_ = 0;
    int C_ = 0;
    if (B >= 1 + A) {
     C = C_;
     I = I_;
     X = X_;
     goto loc_f218;
    }
   }
  goto end;
 }
 loc_f211:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (A >= B) {
     B = B_;
     goto loc_f211;
    }
   }
   if (nondet_bool()) {
    int J_ = 1 + J;
    if (B >= 1 + A) {
     J = J_;
     goto loc_f54;
    }
   }
  goto end;
 }
 loc_f218:
 {
 __cost++;
   if (nondet_bool()) {
    int W_ = nondet();
    int V_ = nondet();
    int T_ = nondet();
    int S_ = nondet();
    int R_ = nondet();
    int U_ = 1;
    if (V_ >= 2 && A >= B) {
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     V = V_;
     W = W_;
     goto loc_f228;
    }
   }
   if (nondet_bool()) {
    int W_ = nondet();
    int T_ = nondet();
    int S_ = nondet();
    int R_ = nondet();
    int V_ = 1;
    int U_ = 1;
    if (1 >= T_ && A >= B) {
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     V = V_;
     W = W_;
     goto loc_f228;
    }
   }
   if (nondet_bool()) {
    int J_ = 1 + J;
    if (C >= 0 && B >= 1 + A) {
     J = J_;
     goto loc_f54;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + C && B >= 1 + A) {
     goto loc_f43;
    }
   }
  goto end;
 }
 loc_f228:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (C >= W) {
     B = B_;
     goto loc_f218;
    }
   }
   if (nondet_bool()) {
    int C_ = W;
    int B_ = 1 + B;
    if (W >= 1 + C) {
     B = B_;
     C = C_;
     goto loc_f218;
    }
   }
  goto end;
 }
 loc_f44:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = 0;
    int F_ = 0;
    int B_ = 1 + B;
    if (A >= B) {
     B = B_;
     F = F_;
     G = G_;
     goto loc_f44;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = nondet();
    int H_ = H + G_;
    int B_ = 1 + B;
    if (0 >= 1 + F_ && A >= B) {
     B = B_;
     F = F_;
     G = G_;
     H = H_;
     goto loc_f44;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = nondet();
    int H_ = H + G_;
    int B_ = 1 + B;
    if (F_ >= 1 && A >= B) {
     B = B_;
     F = F_;
     G = G_;
     H = H_;
     goto loc_f44;
    }
   }
   if (nondet_bool()) {
    int I_ = 1;
    if (B >= 1 + A) {
     I = I_;
     goto loc_f54;
    }
   }
  goto end;
 }
 loc_f54:
 {
 __cost++;
   if (nondet_bool()) {
    if (200 >= J && 0 >= 1 + I) {
     goto loc_f57;
    }
   }
   if (nondet_bool()) {
    if (200 >= J && I >= 1) {
     goto loc_f57;
    }
   }
   if (nondet_bool()) {
    int I_ = 0;
    if (I == 0 && 200 >= J) {
     I = I_;
     goto loc_f99;
    }
   }
   if (nondet_bool()) {
    if (J >= 201) {
     goto loc_f43;
    }
   }
  goto end;
 }
 loc_f57:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_f59;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_f67;
    }
   }
  goto end;
 }
 loc_f59:
 {
 __cost++;
   if (nondet_bool()) {
    int K_ = 1 + K;
    if (A >= K) {
     K = K_;
     goto loc_f59;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (K >= 1 + A) {
     B = B_;
     goto loc_f57;
    }
   }
  goto end;
 }
 loc_f67:
 {
 __cost++;
   if (nondet_bool()) {
    int M_ = nondet();
    if (0 >= 1 + M_ && A >= 1 + L) {
     M = M_;
     goto loc_f71;
    }
   }
   if (nondet_bool()) {
    int M_ = nondet();
    if (M_ >= 1 && A >= 1 + L) {
     M = M_;
     goto loc_f71;
    }
   }
   if (nondet_bool()) {
    int M_ = 0;
    int L_ = 1 + L;
    if (A >= 1 + L) {
     L = L_;
     M = M_;
     goto loc_f67;
    }
   }
   if (nondet_bool()) {
    if (L >= A) {
     goto loc_f89;
    }
   }
  goto end;
 }
 loc_f71:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = 0;
    if (A >= K) {
     H = H_;
     goto loc_f74;
    }
   }
   if (nondet_bool()) {
    int L_ = 1 + L;
    if (K >= 1 + A) {
     L = L_;
     goto loc_f67;
    }
   }
  goto end;
 }
 loc_f74:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = nondet();
    int B_ = 1 + B;
    if (A >= B) {
     B = B_;
     H = H_;
     goto loc_f74;
    }
   }
   if (nondet_bool()) {
    int H_ = nondet();
    if (B >= 1 + A) {
     H = H_;
     goto loc_f80;
    }
   }
  goto end;
 }
 loc_f80:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (A >= B) {
     B = B_;
     goto loc_f80;
    }
   }
   if (nondet_bool()) {
    int K_ = 1 + K;
    if (B >= 1 + A) {
     K = K_;
     goto loc_f71;
    }
   }
  goto end;
 }
 loc_f89:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_f92;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_f161;
    }
   }
  goto end;
 }
 loc_f92:
 {
 __cost++;
   if (nondet_bool()) {
    int K_ = 1 + K;
    if (B >= 1 + K) {
     K = K_;
     goto loc_f92;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (K >= B) {
     B = B_;
     goto loc_f89;
    }
   }
  goto end;
 }
 loc_f99:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (A >= B) {
     B = B_;
     goto loc_f99;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_f104;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f0;
    }
   }
  goto end;
 }
 loc_f43:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
