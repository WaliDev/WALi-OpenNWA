int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R) {
 goto loc_f2;

 loc_f13:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = nondet();
    int D_ = nondet();
    int F_ = 1;
    int C_ = 1 + B;
    if (A >= B) {
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     goto loc_f20;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1;
    if (A == 1) {
     A = A_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int B_ = 1;
    if (0 >= A) {
     B = B_;
     goto loc_f13;
    }
   }
   if (nondet_bool()) {
    int B_ = 1;
    if (A >= 2) {
     B = B_;
     goto loc_f13;
    }
   }
  goto end;
 }
 loc_f20:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = nondet();
    int D_ = nondet();
    int F_ = 1 + F;
    if (B >= F) {
     D = D_;
     E = E_;
     F = F_;
     goto loc_f20;
    }
   }
   if (nondet_bool()) {
    int F_ = 1;
    if (F >= 1 + B && 0 >= 1 + E) {
     F = F_;
     goto loc_f31;
    }
   }
   if (nondet_bool()) {
    int F_ = 1;
    if (F >= 1 + B && E >= 1) {
     F = F_;
     goto loc_f31;
    }
   }
   if (nondet_bool()) {
    int F_ = 1;
    int E_ = 0;
    if (E == 0 && F >= 1 + B) {
     E = E_;
     F = F_;
     goto loc_f31;
    }
   }
  goto end;
 }
 loc_f31:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = 1 + F;
    if (B >= F) {
     F = F_;
     goto loc_f31;
    }
   }
   if (nondet_bool()) {
    int C_ = A;
    if (A == C && F >= 1 + B) {
     C = C_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int I_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int F_ = 1;
    if (F >= 1 + B && A >= 1 + C) {
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     goto loc_f45;
    }
   }
   if (nondet_bool()) {
    int I_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int F_ = 1;
    if (F >= 1 + B && C >= 1 + A) {
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     goto loc_f45;
    }
   }
  goto end;
 }
 loc_f45:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int F_ = 1 + F;
    if (B >= F) {
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     goto loc_f45;
    }
   }
   if (nondet_bool()) {
    int R_ = nondet();
    int Q_ = nondet();
    int J_ = nondet();
    int K_ = B;
    int F_ = 1;
    if (F >= 1 + B) {
     F = F_;
     J = J_;
     K = K_;
     Q = Q_;
     R = R_;
     goto loc_f60;
    }
   }
  goto end;
 }
 loc_f60:
 {
 __cost++;
   if (nondet_bool()) {
    int O_ = nondet();
    int N_ = nondet();
    int M_ = nondet();
    int L_ = nondet();
    int P_ = K;
    int K_ = -1 + K;
    int F_ = 1 + F;
    if (J >= F) {
     F = F_;
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     O = O_;
     P = P_;
     goto loc_f60;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (F >= 1 + J) {
     B = B_;
     goto loc_f13;
    }
   }
  goto end;
 }
 loc_f1:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
