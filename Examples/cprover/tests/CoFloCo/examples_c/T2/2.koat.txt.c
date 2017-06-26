int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N) {
 goto loc_f300;

 loc_f11:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = nondet();
    int B_ = nondet();
    int C_ = B;
    if (B >= 1 && A >= 0) {
     B = B_;
     C = C_;
     D = D_;
     goto loc_f11;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int B_ = nondet();
    int C_ = B;
    if (0 >= 1 + B && A >= 0) {
     B = B_;
     C = C_;
     D = D_;
     goto loc_f11;
    }
   }
   if (nondet_bool()) {
    int K_ = nondet();
    int B_ = 0;
    if (B == 0 && A >= 0) {
     B = B_;
     K = K_;
     goto loc_f13;
    }
   }
  goto end;
 }
 loc_f16:
 {
 __cost++;
   if (nondet_bool()) {
    int J_ = nondet();
    int I_ = nondet();
    int H_ = nondet();
    int G_ = 1 + G;
    if (F >= 2 + G && E >= 0) {
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     goto loc_f16;
    }
   }
   if (nondet_bool()) {
    int K_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int B_ = nondet();
    int M_ = J;
    int L_ = J;
    if (E >= 0 && C_ >= 1 && 1 + G >= F) {
     B = B_;
     C = C_;
     D = D_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f11;
    }
   }
   if (nondet_bool()) {
    int K_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int B_ = nondet();
    int M_ = J;
    int L_ = J;
    if (E >= 0 && 0 >= 1 + C_ && 1 + G >= F) {
     B = B_;
     C = C_;
     D = D_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f11;
    }
   }
  goto end;
 }
 loc_f300:
 {
 __cost++;
   if (nondet_bool()) {
    int N_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int H_ = nondet();
    int G_ = 1;
    if (F >= 2) {
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     N = N_;
     goto loc_f16;
    }
   }
   if (nondet_bool()) {
    int N_ = nondet();
    int K_ = nondet();
    int M_ = 0;
    int L_ = 0;
    int J_ = 0;
    int G_ = 0;
    int B_ = 0;
    if (1 >= F) {
     B = B_;
     G = G_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     goto loc_f13;
    }
   }
  goto end;
 }
 loc_f13:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
