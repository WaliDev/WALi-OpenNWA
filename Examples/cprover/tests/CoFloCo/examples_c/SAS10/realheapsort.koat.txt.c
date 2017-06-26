int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T) {
 goto loc_start0;

 loc_lbl101:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = nondet();
    if (T == A && N == O && L == M && 2 + 2 * J >= R && P >= R && R >= 1 + 2 * J && R >= 1 && A >= 3 && 2 + 2 * D >= R && R >= 1 + 2 * D) {
     F = F_;
     goto loc_lbl111;
    }
   }
  goto end;
 }
 loc_lbl111:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = nondet();
    if (T == A && N == O && L == M && 2 + 2 * D >= R && 2 + 2 * J >= R && P >= R && R >= 1 + 2 * J && R >= 1 + 2 * D && R >= 1 && A >= 3 && 2 + 2 * F >= R && R >= 1 + 2 * F) {
     H = H_;
     goto loc_lbl121;
    }
   }
  goto end;
 }
 loc_lbl121:
 {
 __cost++;
   if (nondet_bool()) {
    int R_ = H;
    if (T == A && N == O && L == M && 2 + 2 * D >= R && 2 + 2 * F >= R && 2 + 2 * J >= R && P >= R && R >= 1 + 2 * J && R >= 1 + 2 * F && R >= 1 + 2 * D && R >= 1 && A >= 3 && 2 + 2 * H >= R && R >= 1 + 2 * H) {
     R = R_;
     goto loc_lbl123;
    }
   }
  goto end;
 }
 loc_lbl123:
 {
 __cost++;
   if (nondet_bool()) {
    int J_ = nondet();
    if (N == O && T == A && L == M && R == H && A >= 3 && 1 + 2 * J >= 2 * H && 1 + 2 * F >= 2 * H && 1 + 2 * F >= 2 * J && 1 + 2 * F >= 2 * D && 1 + 2 * D >= 2 * H && 1 + 2 * D >= 2 * J && 1 + 2 * J >= 2 * F && 1 + 2 * J >= 2 * D && 1 + 2 * H >= 2 * F && 1 + 2 * H >= 2 * D && 1 + 2 * H >= 2 * J && 1 + 2 * D >= 2 * F && P >= 1 + 2 * H && P >= 1 + 2 * D && P >= 1 + 2 * F && P >= 1 + 2 * J && 1 + 2 * D >= 0 && 1 + 2 * J >= 0 && 1 + 2 * F >= 0 && P >= 1 && 1 + 2 * H >= 0) {
     J = J_;
     goto loc_lbl71;
    }
   }
  goto end;
 }
 loc_lbl133:
 {
 __cost++;
   if (nondet_bool()) {
    if (T == A && P == 1 + L && 1 + B >= A && B >= 1 + 2 * J && A >= 2 + L + N && L >= 0 && L + N >= 1 && R >= N && 2 + R + L + N >= A && 2 + L >= A) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int R_ = 0;
    int P_ = 1 + P;
    int N_ = 0;
    int L_ = P;
    if (3 + L == A && 2 + P == A && T == A && 1 + B >= A && B >= 1 + 2 * J && 1 >= N && A >= 3 && A + N >= 4 && R >= N && R + N >= 1) {
     L = L_;
     N = N_;
     P = P_;
     R = R_;
     goto loc_lbl133;
    }
   }
   if (nondet_bool()) {
    int R_ = T;
    int N_ = 1;
    if (T == A && P == 1 + L && 1 + B >= A && B >= 1 + 2 * J && A >= 2 + L + N && L >= 0 && L + N >= 1 && R >= N && 2 + R + L + N >= A && A >= 4 + L) {
     N = N_;
     R = R_;
     goto loc_lbl281;
    }
   }
   if (nondet_bool()) {
    int R_ = 1;
    int N_ = 1;
    if (T == A && P == 1 + L && 1 + B >= A && B >= 1 + 2 * J && A >= 2 + L + N && L >= 0 && L + N >= 1 && R >= N && 2 + R + L + N >= A && A >= 4 + L) {
     N = N_;
     R = R_;
     goto loc_lbl271;
    }
   }
   if (nondet_bool()) {
    int R_ = T;
    int N_ = 2;
    if (T == A && P == 1 + L && 1 + B >= A && B >= 1 + 2 * J && A >= 2 + L + N && L >= 0 && L + N >= 1 && R >= N && 2 + R + L + N >= A && A >= 5 + L) {
     N = N_;
     R = R_;
     goto loc_lbl281;
    }
   }
   if (nondet_bool()) {
    int R_ = 2;
    int N_ = 2;
    if (T == A && P == 1 + L && 1 + B >= A && B >= 1 + 2 * J && A >= 2 + L + N && L >= 0 && L + N >= 1 && R >= N && 2 + R + L + N >= A && A >= 5 + L) {
     N = N_;
     R = R_;
     goto loc_lbl271;
    }
   }
  goto end;
 }
 loc_lbl271:
 {
 __cost++;
   if (nondet_bool()) {
    int P_ = 1 + P;
    int L_ = P;
    if (T == A && R == N && 1 + B >= A && B >= 1 + 2 * J && A >= 2 + N + P && A >= 2 * N + P && N >= 1 && P >= 0 && 2 + 2 * N + P >= A) {
     L = L_;
     P = P_;
     goto loc_lbl133;
    }
   }
   if (nondet_bool()) {
    int R_ = T;
    int N_ = 1 + 2 * R;
    if (T == A && R == N && 1 + B >= A && B >= 1 + 2 * J && A >= 2 + N + P && A >= 2 * N + P && N >= 1 && P >= 0 && A >= 3 + 2 * N + P) {
     N = N_;
     R = R_;
     goto loc_lbl281;
    }
   }
   if (nondet_bool()) {
    int R_ = 1 + 2 * R;
    int N_ = 1 + 2 * R;
    if (T == A && R == N && 1 + B >= A && B >= 1 + 2 * J && A >= 2 + N + P && A >= 2 * N + P && N >= 1 && P >= 0 && A >= 3 + 2 * N + P) {
     N = N_;
     R = R_;
     goto loc_lbl271;
    }
   }
   if (nondet_bool()) {
    int R_ = T;
    int N_ = 2 + 2 * R;
    if (T == A && R == N && 1 + B >= A && B >= 1 + 2 * J && A >= 2 + N + P && A >= 2 * N + P && N >= 1 && P >= 0 && A >= 4 + 2 * N + P) {
     N = N_;
     R = R_;
     goto loc_lbl281;
    }
   }
   if (nondet_bool()) {
    int R_ = 2 + 2 * R;
    int N_ = 2 + 2 * R;
    if (T == A && R == N && 1 + B >= A && B >= 1 + 2 * J && A >= 2 + N + P && A >= 2 * N + P && N >= 1 && P >= 0 && A >= 4 + 2 * N + P) {
     N = N_;
     R = R_;
     goto loc_lbl271;
    }
   }
  goto end;
 }
 loc_lbl281:
 {
 __cost++;
   if (nondet_bool()) {
    int P_ = 1 + P;
    int L_ = P;
    if (R == A && T == A && 1 + B >= A && B >= 1 + 2 * J && A >= 2 + P + N && P >= 0 && N >= 1 && 2 + A + P >= 0) {
     L = L_;
     P = P_;
     goto loc_lbl133;
    }
   }
   if (nondet_bool()) {
    int R_ = T;
    int N_ = 1 + 2 * R;
    if (R == A && T == A && 1 + B >= A && B >= 1 + 2 * J && A >= 2 + P + N && P >= 0 && N >= 1 && 0 >= 3 + A + P) {
     N = N_;
     R = R_;
     goto loc_lbl281;
    }
   }
   if (nondet_bool()) {
    int R_ = 1 + 2 * R;
    int N_ = 1 + 2 * R;
    if (R == A && T == A && 1 + B >= A && B >= 1 + 2 * J && A >= 2 + P + N && P >= 0 && N >= 1 && 0 >= 3 + A + P) {
     N = N_;
     R = R_;
     goto loc_lbl271;
    }
   }
   if (nondet_bool()) {
    int R_ = T;
    int N_ = 2 + 2 * R;
    if (R == A && T == A && 1 + B >= A && B >= 1 + 2 * J && A >= 2 + P + N && P >= 0 && N >= 1 && 0 >= 4 + A + P) {
     N = N_;
     R = R_;
     goto loc_lbl281;
    }
   }
   if (nondet_bool()) {
    int R_ = 2 + 2 * R;
    int N_ = 2 + 2 * R;
    if (R == A && T == A && 1 + B >= A && B >= 1 + 2 * J && A >= 2 + P + N && P >= 0 && N >= 1 && 0 >= 4 + A + P) {
     N = N_;
     R = R_;
     goto loc_lbl271;
    }
   }
  goto end;
 }
 loc_lbl43:
 {
 __cost++;
   if (nondet_bool()) {
    int J_ = nondet();
    int R_ = P;
    if (T == A && N == O && L == M && P == 1 + B && B >= R && 2 + 2 * J >= R && B >= 1 && R >= 1 + 2 * J && A >= 3 && A >= 2 + B) {
     J = J_;
     R = R_;
     goto loc_lbl71;
    }
   }
   if (nondet_bool()) {
    int P_ = 0;
    if (T == A && N == O && L == M && P == 1 + B && B >= R && 2 + 2 * J >= R && B >= 1 && R >= 1 + 2 * J && A >= 3 && 1 >= A && 1 + B >= A) {
     P = P_;
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int R_ = 0;
    int P_ = 1;
    int N_ = 0;
    int L_ = 0;
    if (A == 2 && N == O && L == M && P == 1 + B && T == 2 && B >= R && 2 + 2 * J >= R && R >= 1 + 2 * J && 0 >= 1 && B >= 1) {
     L = L_;
     N = N_;
     P = P_;
     R = R_;
     goto loc_lbl133;
    }
   }
   if (nondet_bool()) {
    int R_ = T;
    int P_ = 0;
    int N_ = 1;
    if (T == A && N == O && L == M && P == 1 + B && B >= R && 2 + 2 * J >= R && B >= 1 && R >= 1 + 2 * J && 1 + B >= A && A >= 3) {
     N = N_;
     P = P_;
     R = R_;
     goto loc_lbl281;
    }
   }
   if (nondet_bool()) {
    int R_ = 1;
    int P_ = 0;
    int N_ = 1;
    if (T == A && N == O && L == M && P == 1 + B && B >= R && 2 + 2 * J >= R && B >= 1 && R >= 1 + 2 * J && 1 + B >= A && A >= 3) {
     N = N_;
     P = P_;
     R = R_;
     goto loc_lbl271;
    }
   }
   if (nondet_bool()) {
    int R_ = T;
    int P_ = 0;
    int N_ = 2;
    if (T == A && N == O && L == M && P == 1 + B && B >= R && 2 + 2 * J >= R && B >= 1 && R >= 1 + 2 * J && A >= 3 && 1 + B >= A && A >= 4) {
     N = N_;
     P = P_;
     R = R_;
     goto loc_lbl281;
    }
   }
   if (nondet_bool()) {
    int R_ = 2;
    int P_ = 0;
    int N_ = 2;
    if (T == A && N == O && L == M && P == 1 + B && B >= R && 2 + 2 * J >= R && B >= 1 && R >= 1 + 2 * J && A >= 3 && 1 + B >= A && A >= 4) {
     N = N_;
     P = P_;
     R = R_;
     goto loc_lbl271;
    }
   }
  goto end;
 }
 loc_lbl71:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = nondet();
    if (L == M && N == O && T == A && P >= R && A >= 3 && P >= 1 && 2 + 2 * J >= R && R >= 1 + 2 * J && R >= 1) {
     D = D_;
     goto loc_lbl101;
    }
   }
   if (nondet_bool()) {
    int P_ = 1 + P;
    int B_ = P;
    if (L == M && N == O && T == A && P >= R && A >= 3 && P >= 1 && 2 + 2 * J >= R && R >= 1 + 2 * J) {
     B = B_;
     P = P_;
     goto loc_lbl43;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (T == A && R == S && P == Q && N == O && L == M && J == K && H == I && F == G && D == E && B == C && 2 >= A) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int J_ = nondet();
    int R_ = 1;
    int P_ = 1;
    if (T == A && R == S && P == Q && N == O && L == M && J == K && H == I && F == G && D == E && B == C && A >= 3) {
     J = J_;
     P = P_;
     R = R_;
     goto loc_lbl71;
    }
   }
  goto end;
 }
 loc_start0:
 {
 __cost++;
   if (nondet_bool()) {
    int T_ = A;
    int R_ = S;
    int P_ = Q;
    int N_ = O;
    int L_ = M;
    int J_ = K;
    int H_ = I;
    int F_ = G;
    int D_ = E;
    int B_ = C;
    if (1 >= 0) {
     B = B_;
     D = D_;
     F = F_;
     H = H_;
     J = J_;
     L = L_;
     N = N_;
     P = P_;
     R = R_;
     T = T_;
     goto loc_start;
    }
   }
  goto end;
 }
 loc_stop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
