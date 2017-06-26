int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T, int U, int V) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1 + B) {
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int L_ = 0;
    int H_ = 0;
    int A_ = B;
    if (B == A) {
     A = A_;
     H = H_;
     L = L_;
     goto loc_f17;
    }
   }
  goto end;
 }
 loc_f101:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + E) {
     goto loc_f102;
    }
   }
   if (nondet_bool()) {
    if (E >= 1) {
     goto loc_f102;
    }
   }
   if (nondet_bool()) {
    int U_ = nondet();
    int T_ = 0;
    int J_ = 0;
    int E_ = 0;
    if (E == 0) {
     E = E_;
     J = J_;
     T = T_;
     U = U_;
     goto loc_f108;
    }
   }
  goto end;
 }
 loc_f102:
 {
 __cost++;
   if (nondet_bool()) {
    int U_ = nondet();
    int T_ = 1;
    int J_ = 1;
    if (0 >= 1 + B) {
     J = J_;
     T = T_;
     U = U_;
     goto loc_f108;
    }
   }
   if (nondet_bool()) {
    int U_ = nondet();
    int T_ = 1;
    int J_ = 1;
    if (B >= 1) {
     J = J_;
     T = T_;
     U = U_;
     goto loc_f108;
    }
   }
   if (nondet_bool()) {
    int U_ = nondet();
    int T_ = 0;
    int J_ = 0;
    int B_ = 0;
    if (B == 0) {
     B = B_;
     J = J_;
     T = T_;
     U = U_;
     goto loc_f108;
    }
   }
  goto end;
 }
 loc_f108:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + H) {
     goto loc_f109;
    }
   }
   if (nondet_bool()) {
    if (H >= 1) {
     goto loc_f109;
    }
   }
   if (nondet_bool()) {
    int V_ = 1;
    int H_ = 0;
    if (H == 0) {
     H = H_;
     V = V_;
     goto loc_f119;
    }
   }
  goto end;
 }
 loc_f109:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + I) {
     goto loc_f110;
    }
   }
   if (nondet_bool()) {
    if (I >= 1) {
     goto loc_f110;
    }
   }
   if (nondet_bool()) {
    int V_ = 1;
    int I_ = 0;
    if (I == 0) {
     I = I_;
     V = V_;
     goto loc_f119;
    }
   }
  goto end;
 }
 loc_f110:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + J) {
     goto loc_f111;
    }
   }
   if (nondet_bool()) {
    if (J >= 1) {
     goto loc_f111;
    }
   }
   if (nondet_bool()) {
    int V_ = 1;
    int J_ = 0;
    if (J == 0) {
     J = J_;
     V = V_;
     goto loc_f119;
    }
   }
  goto end;
 }
 loc_f111:
 {
 __cost++;
   if (nondet_bool()) {
    int V_ = 0;
    if (0 >= 1 + U) {
     V = V_;
     goto loc_f119;
    }
   }
   if (nondet_bool()) {
    int V_ = 0;
    if (U >= 1) {
     V = V_;
     goto loc_f119;
    }
   }
   if (nondet_bool()) {
    int V_ = 1;
    int U_ = 0;
    if (U == 0) {
     U = U_;
     V = V_;
     goto loc_f119;
    }
   }
  goto end;
 }
 loc_f17:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + H) {
     goto loc_f18;
    }
   }
   if (nondet_bool()) {
    if (H >= 1) {
     goto loc_f18;
    }
   }
   if (nondet_bool()) {
    int M_ = 0;
    int H_ = 0;
    if (H == 0) {
     H = H_;
     M = M_;
     goto loc_f33;
    }
   }
  goto end;
 }
 loc_f18:
 {
 __cost++;
   if (nondet_bool()) {
    if (C >= 1 + A) {
     goto loc_f19;
    }
   }
   if (nondet_bool()) {
    if (A >= 1 + C) {
     goto loc_f19;
    }
   }
   if (nondet_bool()) {
    int M_ = 0;
    int H_ = 0;
    int C_ = A;
    if (A == C) {
     C = C_;
     H = H_;
     M = M_;
     goto loc_f33;
    }
   }
  goto end;
 }
 loc_f19:
 {
 __cost++;
   if (nondet_bool()) {
    if (D >= 1 + A) {
     goto loc_f20;
    }
   }
   if (nondet_bool()) {
    if (A >= 1 + D) {
     goto loc_f20;
    }
   }
   if (nondet_bool()) {
    int M_ = 0;
    int H_ = 0;
    int D_ = A;
    if (A == D) {
     D = D_;
     H = H_;
     M = M_;
     goto loc_f33;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    if (C >= 1 + B) {
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + C) {
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    int L_ = 0;
    int H_ = 0;
    int C_ = B;
    if (B == C) {
     C = C_;
     H = H_;
     L = L_;
     goto loc_f17;
    }
   }
  goto end;
 }
 loc_f20:
 {
 __cost++;
   if (nondet_bool()) {
    if (E >= 1 + A) {
     goto loc_f21;
    }
   }
   if (nondet_bool()) {
    if (A >= 1 + E) {
     goto loc_f21;
    }
   }
   if (nondet_bool()) {
    int M_ = 0;
    int H_ = 0;
    int E_ = A;
    if (A == E) {
     E = E_;
     H = H_;
     M = M_;
     goto loc_f33;
    }
   }
  goto end;
 }
 loc_f21:
 {
 __cost++;
   if (nondet_bool()) {
    if (F >= 1 + A) {
     goto loc_f22;
    }
   }
   if (nondet_bool()) {
    if (A >= 1 + F) {
     goto loc_f22;
    }
   }
   if (nondet_bool()) {
    int M_ = 0;
    int H_ = 0;
    int F_ = A;
    if (A == F) {
     F = F_;
     H = H_;
     M = M_;
     goto loc_f33;
    }
   }
  goto end;
 }
 loc_f22:
 {
 __cost++;
   if (nondet_bool()) {
    if (G >= 1 + A) {
     goto loc_f23;
    }
   }
   if (nondet_bool()) {
    if (A >= 1 + G) {
     goto loc_f23;
    }
   }
   if (nondet_bool()) {
    int M_ = 0;
    int H_ = 0;
    int G_ = A;
    if (A == G) {
     G = G_;
     H = H_;
     M = M_;
     goto loc_f33;
    }
   }
  goto end;
 }
 loc_f23:
 {
 __cost++;
   if (nondet_bool()) {
    int M_ = 1;
    int H_ = 1;
    if (K >= 1 + A) {
     H = H_;
     M = M_;
     goto loc_f33;
    }
   }
   if (nondet_bool()) {
    int M_ = 1;
    int H_ = 1;
    if (A >= 1 + K) {
     H = H_;
     M = M_;
     goto loc_f33;
    }
   }
   if (nondet_bool()) {
    int M_ = 0;
    int K_ = A;
    int H_ = 0;
    if (A == K) {
     H = H_;
     K = K_;
     M = M_;
     goto loc_f33;
    }
   }
  goto end;
 }
 loc_f3:
 {
 __cost++;
   if (nondet_bool()) {
    if (D >= 1 + B) {
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + D) {
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int L_ = 0;
    int H_ = 0;
    int D_ = B;
    if (B == D) {
     D = D_;
     H = H_;
     L = L_;
     goto loc_f17;
    }
   }
  goto end;
 }
 loc_f33:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + H) {
     goto loc_f34;
    }
   }
   if (nondet_bool()) {
    if (H >= 1) {
     goto loc_f34;
    }
   }
   if (nondet_bool()) {
    int N_ = 0;
    int H_ = 0;
    if (H == 0) {
     H = H_;
     N = N_;
     goto loc_f47;
    }
   }
  goto end;
 }
 loc_f34:
 {
 __cost++;
   if (nondet_bool()) {
    if (D >= 1 + C) {
     goto loc_f35;
    }
   }
   if (nondet_bool()) {
    if (C >= 1 + D) {
     goto loc_f35;
    }
   }
   if (nondet_bool()) {
    int N_ = 0;
    int H_ = 0;
    int D_ = C;
    if (C == D) {
     D = D_;
     H = H_;
     N = N_;
     goto loc_f47;
    }
   }
  goto end;
 }
 loc_f35:
 {
 __cost++;
   if (nondet_bool()) {
    if (E >= 1 + C) {
     goto loc_f36;
    }
   }
   if (nondet_bool()) {
    if (C >= 1 + E) {
     goto loc_f36;
    }
   }
   if (nondet_bool()) {
    int N_ = 0;
    int H_ = 0;
    int E_ = C;
    if (C == E) {
     E = E_;
     H = H_;
     N = N_;
     goto loc_f47;
    }
   }
  goto end;
 }
 loc_f36:
 {
 __cost++;
   if (nondet_bool()) {
    if (F >= 1 + C) {
     goto loc_f37;
    }
   }
   if (nondet_bool()) {
    if (C >= 1 + F) {
     goto loc_f37;
    }
   }
   if (nondet_bool()) {
    int N_ = 0;
    int H_ = 0;
    int F_ = C;
    if (C == F) {
     F = F_;
     H = H_;
     N = N_;
     goto loc_f47;
    }
   }
  goto end;
 }
 loc_f37:
 {
 __cost++;
   if (nondet_bool()) {
    if (G >= 1 + C) {
     goto loc_f38;
    }
   }
   if (nondet_bool()) {
    if (C >= 1 + G) {
     goto loc_f38;
    }
   }
   if (nondet_bool()) {
    int N_ = 0;
    int H_ = 0;
    int G_ = C;
    if (C == G) {
     G = G_;
     H = H_;
     N = N_;
     goto loc_f47;
    }
   }
  goto end;
 }
 loc_f38:
 {
 __cost++;
   if (nondet_bool()) {
    int N_ = 1;
    int H_ = 1;
    if (K >= 1 + C) {
     H = H_;
     N = N_;
     goto loc_f47;
    }
   }
   if (nondet_bool()) {
    int N_ = 1;
    int H_ = 1;
    if (C >= 1 + K) {
     H = H_;
     N = N_;
     goto loc_f47;
    }
   }
   if (nondet_bool()) {
    int N_ = 0;
    int K_ = C;
    int H_ = 0;
    if (C == K) {
     H = H_;
     K = K_;
     N = N_;
     goto loc_f47;
    }
   }
  goto end;
 }
 loc_f4:
 {
 __cost++;
   if (nondet_bool()) {
    if (E >= 1 + B) {
     goto loc_f5;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + E) {
     goto loc_f5;
    }
   }
   if (nondet_bool()) {
    int L_ = 0;
    int H_ = 0;
    int E_ = B;
    if (B == E) {
     E = E_;
     H = H_;
     L = L_;
     goto loc_f17;
    }
   }
  goto end;
 }
 loc_f47:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + H) {
     goto loc_f48;
    }
   }
   if (nondet_bool()) {
    if (H >= 1) {
     goto loc_f48;
    }
   }
   if (nondet_bool()) {
    int O_ = 0;
    int H_ = 0;
    if (H == 0) {
     H = H_;
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
    if (E >= 1 + D) {
     goto loc_f49;
    }
   }
   if (nondet_bool()) {
    if (D >= 1 + E) {
     goto loc_f49;
    }
   }
   if (nondet_bool()) {
    int O_ = 0;
    int H_ = 0;
    int E_ = D;
    if (D == E) {
     E = E_;
     H = H_;
     O = O_;
     goto loc_f59;
    }
   }
  goto end;
 }
 loc_f49:
 {
 __cost++;
   if (nondet_bool()) {
    if (F >= 1 + D) {
     goto loc_f50;
    }
   }
   if (nondet_bool()) {
    if (D >= 1 + F) {
     goto loc_f50;
    }
   }
   if (nondet_bool()) {
    int O_ = 0;
    int H_ = 0;
    int F_ = D;
    if (D == F) {
     F = F_;
     H = H_;
     O = O_;
     goto loc_f59;
    }
   }
  goto end;
 }
 loc_f5:
 {
 __cost++;
   if (nondet_bool()) {
    if (F >= 1 + B) {
     goto loc_f6;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + F) {
     goto loc_f6;
    }
   }
   if (nondet_bool()) {
    int L_ = 0;
    int H_ = 0;
    int F_ = B;
    if (B == F) {
     F = F_;
     H = H_;
     L = L_;
     goto loc_f17;
    }
   }
  goto end;
 }
 loc_f50:
 {
 __cost++;
   if (nondet_bool()) {
    if (G >= 1 + D) {
     goto loc_f51;
    }
   }
   if (nondet_bool()) {
    if (D >= 1 + G) {
     goto loc_f51;
    }
   }
   if (nondet_bool()) {
    int O_ = 0;
    int H_ = 0;
    int G_ = D;
    if (D == G) {
     G = G_;
     H = H_;
     O = O_;
     goto loc_f59;
    }
   }
  goto end;
 }
 loc_f51:
 {
 __cost++;
   if (nondet_bool()) {
    int O_ = 1;
    int H_ = 1;
    if (K >= 1 + D) {
     H = H_;
     O = O_;
     goto loc_f59;
    }
   }
   if (nondet_bool()) {
    int O_ = 1;
    int H_ = 1;
    if (D >= 1 + K) {
     H = H_;
     O = O_;
     goto loc_f59;
    }
   }
   if (nondet_bool()) {
    int O_ = 0;
    int K_ = D;
    int H_ = 0;
    if (D == K) {
     H = H_;
     K = K_;
     O = O_;
     goto loc_f59;
    }
   }
  goto end;
 }
 loc_f59:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + H) {
     goto loc_f60;
    }
   }
   if (nondet_bool()) {
    if (H >= 1) {
     goto loc_f60;
    }
   }
   if (nondet_bool()) {
    int P_ = 0;
    int H_ = 0;
    if (H == 0) {
     H = H_;
     P = P_;
     goto loc_f69;
    }
   }
  goto end;
 }
 loc_f6:
 {
 __cost++;
   if (nondet_bool()) {
    if (G >= 1 + B) {
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + G) {
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    int L_ = 0;
    int H_ = 0;
    int G_ = B;
    if (B == G) {
     G = G_;
     H = H_;
     L = L_;
     goto loc_f17;
    }
   }
  goto end;
 }
 loc_f60:
 {
 __cost++;
   if (nondet_bool()) {
    if (F >= 1 + E) {
     goto loc_f61;
    }
   }
   if (nondet_bool()) {
    if (E >= 1 + F) {
     goto loc_f61;
    }
   }
   if (nondet_bool()) {
    int P_ = 0;
    int H_ = 0;
    int F_ = E;
    if (E == F) {
     F = F_;
     H = H_;
     P = P_;
     goto loc_f69;
    }
   }
  goto end;
 }
 loc_f61:
 {
 __cost++;
   if (nondet_bool()) {
    if (G >= 1 + E) {
     goto loc_f62;
    }
   }
   if (nondet_bool()) {
    if (E >= 1 + G) {
     goto loc_f62;
    }
   }
   if (nondet_bool()) {
    int P_ = 0;
    int H_ = 0;
    int G_ = E;
    if (E == G) {
     G = G_;
     H = H_;
     P = P_;
     goto loc_f69;
    }
   }
  goto end;
 }
 loc_f62:
 {
 __cost++;
   if (nondet_bool()) {
    int P_ = 1;
    int H_ = 1;
    if (K >= 1 + E) {
     H = H_;
     P = P_;
     goto loc_f69;
    }
   }
   if (nondet_bool()) {
    int P_ = 1;
    int H_ = 1;
    if (E >= 1 + K) {
     H = H_;
     P = P_;
     goto loc_f69;
    }
   }
   if (nondet_bool()) {
    int P_ = 0;
    int K_ = E;
    int H_ = 0;
    if (E == K) {
     H = H_;
     K = K_;
     P = P_;
     goto loc_f69;
    }
   }
  goto end;
 }
 loc_f69:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + H) {
     goto loc_f70;
    }
   }
   if (nondet_bool()) {
    if (H >= 1) {
     goto loc_f70;
    }
   }
   if (nondet_bool()) {
    int Q_ = 0;
    int H_ = 0;
    if (H == 0) {
     H = H_;
     Q = Q_;
     goto loc_f77;
    }
   }
  goto end;
 }
 loc_f7:
 {
 __cost++;
   if (nondet_bool()) {
    int L_ = 1;
    int H_ = 1;
    if (K >= 1 + B) {
     H = H_;
     L = L_;
     goto loc_f17;
    }
   }
   if (nondet_bool()) {
    int L_ = 1;
    int H_ = 1;
    if (B >= 1 + K) {
     H = H_;
     L = L_;
     goto loc_f17;
    }
   }
   if (nondet_bool()) {
    int L_ = 0;
    int K_ = B;
    int H_ = 0;
    if (B == K) {
     H = H_;
     K = K_;
     L = L_;
     goto loc_f17;
    }
   }
  goto end;
 }
 loc_f70:
 {
 __cost++;
   if (nondet_bool()) {
    if (G >= 1 + F) {
     goto loc_f71;
    }
   }
   if (nondet_bool()) {
    if (F >= 1 + G) {
     goto loc_f71;
    }
   }
   if (nondet_bool()) {
    int Q_ = 0;
    int H_ = 0;
    int G_ = F;
    if (F == G) {
     G = G_;
     H = H_;
     Q = Q_;
     goto loc_f77;
    }
   }
  goto end;
 }
 loc_f71:
 {
 __cost++;
   if (nondet_bool()) {
    int Q_ = 1;
    int H_ = 1;
    if (K >= 1 + F) {
     H = H_;
     Q = Q_;
     goto loc_f77;
    }
   }
   if (nondet_bool()) {
    int Q_ = 1;
    int H_ = 1;
    if (F >= 1 + K) {
     H = H_;
     Q = Q_;
     goto loc_f77;
    }
   }
   if (nondet_bool()) {
    int Q_ = 0;
    int K_ = F;
    int H_ = 0;
    if (F == K) {
     H = H_;
     K = K_;
     Q = Q_;
     goto loc_f77;
    }
   }
  goto end;
 }
 loc_f77:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + H) {
     goto loc_f78;
    }
   }
   if (nondet_bool()) {
    if (H >= 1) {
     goto loc_f78;
    }
   }
   if (nondet_bool()) {
    int R_ = 0;
    int H_ = 0;
    if (H == 0) {
     H = H_;
     R = R_;
     goto loc_f83;
    }
   }
  goto end;
 }
 loc_f78:
 {
 __cost++;
   if (nondet_bool()) {
    int R_ = 1;
    int H_ = 1;
    if (K >= 1 + G) {
     H = H_;
     R = R_;
     goto loc_f83;
    }
   }
   if (nondet_bool()) {
    int R_ = 1;
    int H_ = 1;
    if (G >= 1 + K) {
     H = H_;
     R = R_;
     goto loc_f83;
    }
   }
   if (nondet_bool()) {
    int R_ = 0;
    int K_ = G;
    int H_ = 0;
    if (G == K) {
     H = H_;
     K = K_;
     R = R_;
     goto loc_f83;
    }
   }
  goto end;
 }
 loc_f83:
 {
 __cost++;
   if (nondet_bool()) {
    int S_ = 1;
    int I_ = 1;
    if (9 >= A && 9 >= B && 9 >= C && 9 >= D && 9 >= E && 9 >= F && 9 >= G && 9 >= K) {
     I = I_;
     S = S_;
     goto loc_f101;
    }
   }
   if (nondet_bool()) {
    int S_ = 0;
    int I_ = 0;
    if (9 >= A && 9 >= B && 9 >= C && 9 >= D && 9 >= E && 9 >= F && 9 >= G && K >= 10) {
     I = I_;
     S = S_;
     goto loc_f101;
    }
   }
   if (nondet_bool()) {
    int S_ = 0;
    int I_ = 0;
    if (9 >= A && 9 >= B && 9 >= C && 9 >= D && 9 >= E && 9 >= F && G >= 10) {
     I = I_;
     S = S_;
     goto loc_f101;
    }
   }
   if (nondet_bool()) {
    int S_ = 0;
    int I_ = 0;
    if (9 >= A && 9 >= B && 9 >= C && 9 >= D && 9 >= E && F >= 10) {
     I = I_;
     S = S_;
     goto loc_f101;
    }
   }
   if (nondet_bool()) {
    int S_ = 0;
    int I_ = 0;
    if (9 >= A && 9 >= B && 9 >= C && 9 >= D && E >= 10) {
     I = I_;
     S = S_;
     goto loc_f101;
    }
   }
   if (nondet_bool()) {
    int S_ = 0;
    int I_ = 0;
    if (9 >= A && 9 >= B && 9 >= C && D >= 10) {
     I = I_;
     S = S_;
     goto loc_f101;
    }
   }
   if (nondet_bool()) {
    int S_ = 0;
    int I_ = 0;
    if (9 >= A && 9 >= B && C >= 10) {
     I = I_;
     S = S_;
     goto loc_f101;
    }
   }
   if (nondet_bool()) {
    int S_ = 0;
    int I_ = 0;
    if (A >= 10 && 9 >= B) {
     I = I_;
     S = S_;
     goto loc_f101;
    }
   }
   if (nondet_bool()) {
    int S_ = 0;
    int I_ = 0;
    if (B >= 10) {
     I = I_;
     S = S_;
     goto loc_f101;
    }
   }
  goto end;
 }
 loc_f119:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
