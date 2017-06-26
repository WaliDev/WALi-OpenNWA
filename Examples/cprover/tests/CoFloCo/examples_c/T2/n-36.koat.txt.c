int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T, int U) {
 goto loc_f300;

 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int P_ = nondet();
    int O_ = nondet();
    int N_ = nondet();
    int M_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int H_ = 256;
    if (H == 256 && 1 + F == G && Y_0 >= 1) {
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     O = O_;
     P = P_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int O_ = nondet();
    int N_ = nondet();
    int M_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    if (1 + F == G && 0 >= H) {
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     O = O_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int P_ = nondet();
    int O_ = nondet();
    int N_ = nondet();
    int M_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int Q_ = 256;
    if (Q == 256 && G >= 2 + F && G >= 1 + F && Y_0 >= 1) {
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     O = O_;
     P = P_;
     Q = Q_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int O_ = nondet();
    int N_ = nondet();
    int M_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    if (G >= 2 + F && G >= 1 + F && 0 >= Q) {
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     O = O_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int J_ = nondet();
    int I_ = nondet();
    int T_ = 0;
    int S_ = 0;
    int R_ = 0;
    if (F >= G) {
     I = I_;
     J = J_;
     R = R_;
     S = S_;
     T = T_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int U_ = nondet();
    int P_ = nondet();
    int O_ = nondet();
    int N_ = nondet();
    int M_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int T_ = H;
    int S_ = H;
    int R_ = H;
    if (1 + F == G && H >= 257 && H >= 1) {
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     O = O_;
     P = P_;
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    int U_ = nondet();
    int P_ = nondet();
    int O_ = nondet();
    int N_ = nondet();
    int M_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int T_ = H;
    int S_ = H;
    int R_ = H;
    if (1 + F == G && 255 >= H && H >= 1) {
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     O = O_;
     P = P_;
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    int U_ = nondet();
    int P_ = nondet();
    int O_ = nondet();
    int N_ = nondet();
    int M_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int T_ = Q;
    int S_ = Q;
    int R_ = Q;
    if (G >= 2 + F && G >= 1 + F && Q >= 257 && Q >= 1) {
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     O = O_;
     P = P_;
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    int U_ = nondet();
    int P_ = nondet();
    int O_ = nondet();
    int N_ = nondet();
    int M_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int T_ = Q;
    int S_ = Q;
    int R_ = Q;
    if (G >= 2 + F && G >= 1 + F && 255 >= Q && Q >= 1) {
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     O = O_;
     P = P_;
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     goto loc_f3;
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
 loc_f2:
 loc_f3:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
