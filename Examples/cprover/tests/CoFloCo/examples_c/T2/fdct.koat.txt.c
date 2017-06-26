int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 0;
    int A_ = 8;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_f7;
    }
   }
  goto end;
 }
 loc_f62:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Y_2 = nondet();
    int Y_3 = nondet();
    int Y_4 = nondet();
    int Y_5 = nondet();
    int Y_6 = nondet();
    int Y_7 = nondet();
    int Y_8 = nondet();
    int Y_9 = nondet();
    int T_ = nondet();
    int Q_ = nondet();
    int P_ = nondet();
    int J_ = nondet();
    int H_ = nondet();
    int F_ = nondet();
    int D_ = nondet();
    int S_ = Y_9 + T_;
    int R_ = Y_8 + T_;
    int O_ = -3196;
    int N_ = Y_2 + Y_3 + -Y_4 + -Y_5;
    int M_ = Y_2 + Y_3 + Y_4 + Y_5;
    int L_ = Y_0 + Y_1 + -Y_6 + -Y_7;
    int K_ = Y_0 + Y_1 + Y_6 + Y_7;
    int I_ = Y_6 + Y_7;
    int G_ = Y_4 + Y_5;
    int E_ = Y_2 + Y_3;
    int C_ = Y_0 + Y_1;
    int B_ = 1 + B;
    if (7 >= B) {
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
     N = N_;
     O = O_;
     P = P_;
     Q = Q_;
     R = R_;
     S = S_;
     T = T_;
     goto loc_f62;
    }
   }
   if (nondet_bool()) {
    if (B >= 8) {
     goto loc_f118;
    }
   }
  goto end;
 }
 loc_f7:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Y_2 = nondet();
    int Y_3 = nondet();
    int Y_4 = nondet();
    int Y_5 = nondet();
    int Y_6 = nondet();
    int Y_7 = nondet();
    int Y_8 = nondet();
    int Y_9 = nondet();
    int T_ = nondet();
    int Q_ = nondet();
    int P_ = nondet();
    int J_ = nondet();
    int H_ = nondet();
    int F_ = nondet();
    int D_ = nondet();
    int S_ = Y_9 + T_;
    int R_ = Y_8 + T_;
    int O_ = -3196;
    int N_ = Y_2 + Y_3 + -Y_4 + -Y_5;
    int M_ = Y_2 + Y_3 + Y_4 + Y_5;
    int L_ = Y_0 + Y_1 + -Y_6 + -Y_7;
    int K_ = Y_0 + Y_1 + Y_6 + Y_7;
    int I_ = Y_6 + Y_7;
    int G_ = Y_4 + Y_5;
    int E_ = Y_2 + Y_3;
    int C_ = Y_0 + Y_1;
    int B_ = 1 + B;
    if (7 >= B) {
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
     N = N_;
     O = O_;
     P = P_;
     Q = Q_;
     R = R_;
     S = S_;
     T = T_;
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    int B_ = 0;
    if (B >= 8) {
     B = B_;
     goto loc_f62;
    }
   }
  goto end;
 }
 loc_f118:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
