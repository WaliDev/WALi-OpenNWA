int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N) {
 goto loc_f3;

 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = nondet();
    int D_ = nondet();
    int G_ = B;
    int E_ = D;
    int C_ = D;
    int B_ = 1 + B;
    if (B >= 0 && A >= 1 + B) {
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     G = G_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int H_ = nondet();
    int E_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int B_ = nondet();
    int A_ = nondet();
    int L_ = C;
    if (H_ >= 2 && B_ >= H_ && B >= 0 && B >= A) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     goto loc_f4;
    }
   }
  goto end;
 }
 loc_f3:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int M_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int H_ = nondet();
    int E_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int B_ = nondet();
    int A_ = nondet();
    int L_ = 0;
    if (0 >= Y_0 && 0 >= H_) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int N_ = nondet();
    int M_ = nondet();
    int I_ = nondet();
    int H_ = nondet();
    int E_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int A_ = nondet();
    int B_ = 2;
    if (H_ >= 2) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     H = H_;
     I = I_;
     M = M_;
     N = N_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int M_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int E_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int B_ = nondet();
    int A_ = nondet();
    int L_ = D;
    int H_ = 1;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f4;
    }
   }
  goto end;
 }
 loc_f4:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
