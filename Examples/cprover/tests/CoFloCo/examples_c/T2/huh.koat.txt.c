int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R) {
 goto loc_f6;

 loc_f5:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int G_ = nondet();
    int H_ = G;
    if (F >= 0 && E >= 1 && Y_0 >= 1 + Y_1) {
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     goto loc_f0;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int O_ = nondet();
    int N_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int P_ = E;
    int M_ = N;
    int L_ = M;
    int H_ = 0;
    int G_ = M;
    int F_ = -1 + F;
    int E_ = 1 + E;
    if (H == 0 && F >= 0 && E >= 0 && Y_0 >= Y_1) {
     E = E_;
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     L = L_;
     M = M_;
     N = N_;
     O = O_;
     P = P_;
     goto loc_f5;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Q_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int P_ = E;
    int N_ = 0;
    int L_ = M;
    int H_ = 0;
    if (H == 0 && N == 0 && F >= 0 && E >= 0 && Y_0 >= Y_1) {
     H = H_;
     I = I_;
     J = J_;
     L = L_;
     N = N_;
     P = P_;
     Q = Q_;
     goto loc_f12;
    }
   }
  goto end;
 }
 loc_f6:
 {
 __cost++;
   if (nondet_bool()) {
    int L_ = nondet();
    int D_ = nondet();
    int C_ = 0;
    int B_ = 1;
    int A_ = 17;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     L = L_;
     goto loc_f9;
    }
   }
  goto end;
 }
 loc_f9:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = nondet();
    int C_ = 1 + C;
    int B_ = 1 + B;
    if (C >= 0 && A >= 1 + B) {
     B = B_;
     C = C_;
     D = D_;
     goto loc_f9;
    }
   }
   if (nondet_bool()) {
    int Y_2 = nondet();
    int Y_0 = nondet();
    int Y_1 = nondet();
    int O_ = nondet();
    int N_ = nondet();
    int M_ = nondet();
    int L_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int G_ = nondet();
    int R_ = -2 + C;
    int Q_ = D;
    int H_ = 0;
    int F_ = -3 + Y_2 + C;
    int E_ = 1;
    if (Y_0 >= Y_1 && B >= A && C >= 2) {
     E = E_;
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     L = L_;
     M = M_;
     N = N_;
     O = O_;
     Q = Q_;
     R = R_;
     goto loc_f5;
    }
   }
  goto end;
 }
 loc_f0:
 loc_f12:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
