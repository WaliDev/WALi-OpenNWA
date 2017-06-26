int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f8;
    }
   }
   if (nondet_bool()) {
    int H_ = B;
    int G_ = A;
    int E_ = F;
    int C_ = D;
    int B_ = D;
    int A_ = F;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     E = E_;
     G = G_;
     H = H_;
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    int J_ = nondet();
    int I_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int E_ = F;
    int C_ = D;
    int B_ = D;
    int A_ = F;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     E = E_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    int L_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int F_ = nondet();
    int D_ = nondet();
    int B_ = nondet();
    int A_ = nondet();
    int H_ = B;
    int G_ = A;
    int E_ = F;
    int C_ = D;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     goto loc_f8;
    }
   }
   if (nondet_bool()) {
    int H_ = B;
    int G_ = A;
    int E_ = F;
    int C_ = D;
    int A_ = 1 + A;
    if (1 >= 0) {
     A = A_;
     C = C_;
     E = E_;
     G = G_;
     H = H_;
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    int H_ = B;
    int G_ = A;
    int E_ = F;
    int C_ = D;
    int B_ = 1 + B;
    if (1 >= 0) {
     B = B_;
     C = C_;
     E = E_;
     G = G_;
     H = H_;
     goto loc_f3;
    }
   }
  goto end;
 }
 loc_f3:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = B;
    int G_ = A;
    int E_ = F;
    int C_ = D;
    if (A >= 1 + B) {
     C = C_;
     E = E_;
     G = G_;
     H = H_;
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int H_ = B;
    int G_ = A;
    int E_ = F;
    int C_ = D;
    if (B >= 1 + A) {
     C = C_;
     E = E_;
     G = G_;
     H = H_;
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int L_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int F_ = nondet();
    int D_ = nondet();
    int B_ = nondet();
    int A_ = nondet();
    int H_ = A;
    int G_ = A;
    int E_ = F;
    int C_ = D;
    if (A == B) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     goto loc_f8;
    }
   }
  goto end;
 }
 loc_f4:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = B;
    int G_ = A;
    int E_ = F;
    int C_ = D;
    int A_ = 1 + A;
    if (B >= 1 + A) {
     A = A_;
     C = C_;
     E = E_;
     G = G_;
     H = H_;
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    int H_ = B;
    int G_ = A;
    int E_ = F;
    int C_ = D;
    int B_ = 1 + B;
    if (A >= B) {
     B = B_;
     C = C_;
     E = E_;
     G = G_;
     H = H_;
     goto loc_f3;
    }
   }
  goto end;
 }
 loc_f8:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
