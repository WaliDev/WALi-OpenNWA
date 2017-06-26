int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int J_ = nondet();
    int A_ = 1;
    if (0 >= J_) {
     A = A_;
     J = J_;
     goto loc_f20;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int O_ = nondet();
    int N_ = nondet();
    int M_ = nondet();
    int L_ = nondet();
    int K_ = 1;
    int J_ = 0;
    int A_ = 1;
    if (Y_0 >= 1 && O_ >= 1) {
     A = A_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     O = O_;
     goto loc_f20;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int O_ = nondet();
    int N_ = nondet();
    int M_ = nondet();
    int L_ = nondet();
    int K_ = 1;
    int J_ = 0;
    int A_ = 1;
    if (Y_0 >= 1 && 0 >= O_) {
     A = A_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     O = O_;
     goto loc_f40;
    }
   }
  goto end;
 }
 loc_f20:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= A) {
     goto loc_f28;
    }
   }
   if (nondet_bool()) {
    int I_ = nondet();
    int H_ = nondet();
    int B_ = nondet();
    int G_ = 0;
    int A_ = 0;
    if (A >= 1) {
     A = A_;
     B = B_;
     G = G_;
     H = H_;
     I = I_;
     goto loc_f28;
    }
   }
  goto end;
 }
 loc_f28:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1) {
     goto loc_f40;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int E_ = nondet();
    int D_ = nondet();
    int C_ = 0;
    if (999 + B >= F_ && 0 >= A) {
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     goto loc_f40;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int E_ = nondet();
    int D_ = nondet();
    int C_ = 0;
    int A_ = 1;
    if (F_ >= 1000 + B && 0 >= A) {
     A = A_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     goto loc_f40;
    }
   }
  goto end;
 }
 loc_f40:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f40;
    }
   }
  goto end;
 }
 loc_f42:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f45;
    }
   }
  goto end;
 }
 loc_f45:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
