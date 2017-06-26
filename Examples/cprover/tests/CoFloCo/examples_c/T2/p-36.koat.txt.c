int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R) {
 goto loc_f300;

 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int N_ = nondet();
    int M_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int H_ = 256;
    if (H == 256 && Y_0 >= 1 && G >= 1 + F) {
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int M_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    if (0 >= H && G >= 1 + F) {
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int J_ = nondet();
    int I_ = nondet();
    int Q_ = 0;
    int P_ = 0;
    int O_ = 0;
    if (F >= G) {
     I = I_;
     J = J_;
     O = O_;
     P = P_;
     Q = Q_;
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    int R_ = nondet();
    int N_ = nondet();
    int M_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int Q_ = H;
    int P_ = H;
    int O_ = H;
    if (H >= 257 && G >= 1 + F && H >= 1) {
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
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int R_ = nondet();
    int N_ = nondet();
    int M_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int Q_ = H;
    int P_ = H;
    int O_ = H;
    if (255 >= H && G >= 1 + F && H >= 1) {
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
     goto loc_f2;
    }
   }
  goto end;
 }
 loc_f300:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int B_ = nondet();
    int A_ = nondet();
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f3:
 loc_f2:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
