int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int O_ = 0;
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     O = O_;
     goto loc_f11;
    }
   }
  goto end;
 }
 loc_f11:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 0;
    if (9 >= A) {
     B = B_;
     goto loc_f14;
    }
   }
   if (nondet_bool()) {
    int Q_ = 1000;
    int H_ = 0;
    int G_ = 0;
    int F_ = 0;
    int E_ = 0;
    int C_ = 0;
    if (A >= 10) {
     C = C_;
     E = E_;
     F = F_;
     G = G_;
     H = H_;
     Q = Q_;
     goto loc_f33;
    }
   }
  goto end;
 }
 loc_f14:
 {
 __cost++;
   if (nondet_bool()) {
    int P_ = nondet();
    int O_ = nondet();
    int B_ = 1 + B;
    if (9 >= B) {
     B = B_;
     O = O_;
     P = P_;
     goto loc_f14;
    }
   }
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (B >= 10) {
     A = A_;
     goto loc_f11;
    }
   }
  goto end;
 }
 loc_f33:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 0;
    if (9 >= C) {
     D = D_;
     goto loc_f36;
    }
   }
   if (nondet_bool()) {
    int N_ = nondet();
    int M_ = 1500;
    int L_ = H;
    int K_ = G;
    int J_ = F;
    int I_ = E;
    if (C >= 10) {
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     goto loc_f58;
    }
   }
  goto end;
 }
 loc_f36:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_1 = nondet();
    int Y_0 = nondet();
    int F_ = 1 + F;
    int E_ = E + Y_1;
    int D_ = 1 + D;
    if (0 >= 1 + Y_0 && 9 >= D) {
     D = D_;
     E = E_;
     F = F_;
     goto loc_f36;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int H_ = 1 + H;
    int G_ = G + Y_0;
    int D_ = 1 + D;
    if (9 >= D) {
     D = D_;
     G = G_;
     H = H_;
     goto loc_f36;
    }
   }
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (D >= 10) {
     C = C_;
     goto loc_f33;
    }
   }
  goto end;
 }
 loc_f58:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
