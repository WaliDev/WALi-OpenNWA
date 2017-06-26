int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = nondet();
    int C_ = 1;
    int A_ = 100;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f11;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int C_ = 0;
    int A_ = 100;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f11;
    }
   }
  goto end;
 }
 loc_f11:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = nondet();
    int H_ = 1;
    int F_ = 100;
    int E_ = 1;
    int D_ = 1;
    int C_ = 1;
    if (C == 1) {
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     G = G_;
     H = H_;
     goto loc_f23;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int H_ = 0;
    int F_ = 100;
    int E_ = 1;
    int D_ = 1;
    int C_ = 1;
    if (C == 1) {
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     G = G_;
     H = H_;
     goto loc_f23;
    }
   }
   if (nondet_bool()) {
    int J_ = 100;
    int E_ = C;
    int D_ = C;
    if (0 >= C) {
     D = D_;
     E = E_;
     J = J_;
     goto loc_f36;
    }
   }
   if (nondet_bool()) {
    int J_ = 100;
    int E_ = C;
    int D_ = C;
    if (C >= 2) {
     D = D_;
     E = E_;
     J = J_;
     goto loc_f36;
    }
   }
  goto end;
 }
 loc_f23:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = 100;
    int E_ = H;
    if (0 >= H) {
     E = E_;
     I = I_;
     goto loc_f26;
    }
   }
   if (nondet_bool()) {
    int I_ = 100;
    int E_ = H;
    if (H >= 2) {
     E = E_;
     I = I_;
     goto loc_f26;
    }
   }
   if (nondet_bool()) {
    int L_ = nondet();
    int K_ = nondet();
    int H_ = 1;
    int E_ = 1;
    if (H == 1 && 0 >= 2 + K_) {
     E = E_;
     H = H_;
     K = K_;
     L = L_;
     goto loc_f32;
    }
   }
   if (nondet_bool()) {
    int L_ = nondet();
    int K_ = nondet();
    int H_ = 1;
    int E_ = 1;
    if (H == 1 && K_ >= 0) {
     E = E_;
     H = H_;
     K = K_;
     L = L_;
     goto loc_f32;
    }
   }
   if (nondet_bool()) {
    int N_ = nondet();
    int M_ = 100;
    int K_ = -1;
    int H_ = 1;
    int E_ = 1;
    if (H == 1) {
     E = E_;
     H = H_;
     K = K_;
     M = M_;
     N = N_;
     goto loc_f32;
    }
   }
  goto end;
 }
 loc_f26:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (0 >= 1 + Y_0) {
     goto loc_f32;
    }
   }
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f32;
    }
   }
  goto end;
 }
 loc_f36:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (0 >= 1 + Y_0) {
     goto loc_f32;
    }
   }
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f32;
    }
   }
  goto end;
 }
 loc_f32:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
