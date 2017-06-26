int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O) {
 goto loc_f22;

 loc_f17:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = nondet();
    int B_ = nondet();
    int D_ = 1 + D;
    if (D >= 0 && C >= 0 && A >= 1 + B) {
     B = B_;
     D = D_;
     E = E_;
     goto loc_f17;
    }
   }
   if (nondet_bool()) {
    int E_ = nondet();
    int B_ = nondet();
    int D_ = 1 + D;
    if (D >= 0 && C >= 0 && B >= 1 + A) {
     B = B_;
     D = D_;
     E = E_;
     goto loc_f17;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int A_ = B;
    if (B == A && D >= 0 && C >= 0) {
     A = A_;
     G = G_;
     goto loc_f20;
    }
   }
  goto end;
 }
 loc_f18:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = nondet();
    int B_ = nondet();
    int D_ = 1;
    if (A >= 1 + B && F >= 0) {
     B = B_;
     D = D_;
     E = E_;
     goto loc_f17;
    }
   }
   if (nondet_bool()) {
    int E_ = nondet();
    int B_ = nondet();
    int D_ = 1;
    if (B >= 1 + A && F >= 0) {
     B = B_;
     D = D_;
     E = E_;
     goto loc_f17;
    }
   }
  goto end;
 }
 loc_f22:
 {
 __cost++;
   if (nondet_bool()) {
    int M_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int G_ = nondet();
    int O_ = 0;
    int N_ = 3;
    int I_ = 2;
    int B_ = H;
    if (F >= 0 && A >= 1 + H) {
     B = B_;
     G = G_;
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     O = O_;
     goto loc_f18;
    }
   }
   if (nondet_bool()) {
    int M_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int G_ = nondet();
    int O_ = 0;
    int N_ = 3;
    int I_ = 2;
    int B_ = H;
    if (F >= 0 && H >= 1 + A) {
     B = B_;
     G = G_;
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     O = O_;
     goto loc_f18;
    }
   }
  goto end;
 }
 loc_f20:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
