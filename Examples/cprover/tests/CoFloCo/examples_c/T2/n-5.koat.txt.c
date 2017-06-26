int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M) {
 goto loc_f15;

 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = nondet();
    int C_ = nondet();
    int K_ = 0;
    int J_ = 0;
    int H_ = 0;
    if (4 >= M && E >= 1 + B) {
     C = C_;
     F = F_;
     G = G_;
     H = H_;
     J = J_;
     K = K_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int C_ = nondet();
    int K_ = 1;
    int J_ = 1;
    int H_ = 1;
    int B_ = 1 + B;
    if (M >= 5 && E >= 1 + B) {
     B = B_;
     C = C_;
     F = F_;
     H = H_;
     J = J_;
     K = K_;
     goto loc_f300;
    }
   }
  goto end;
 }
 loc_f10:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = nondet();
    int C_ = nondet();
    int M_ = -2;
    int K_ = 0;
    int J_ = 0;
    int H_ = 0;
    if (1 >= 0) {
     C = C_;
     F = F_;
     G = G_;
     H = H_;
     J = J_;
     K = K_;
     M = M_;
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f12:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = nondet();
    int C_ = nondet();
    int L_ = -1 + I;
    int K_ = 0;
    int J_ = 0;
    int H_ = 0;
    if (4 >= I) {
     C = C_;
     F = F_;
     G = G_;
     H = H_;
     J = J_;
     K = K_;
     L = L_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int C_ = nondet();
    int K_ = 1;
    int J_ = 1;
    int H_ = 1;
    int B_ = 1 + B;
    if (I >= 5) {
     B = B_;
     C = C_;
     F = F_;
     H = H_;
     J = J_;
     K = K_;
     goto loc_f300;
    }
   }
  goto end;
 }
 loc_f13:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = nondet();
    int C_ = nondet();
    int K_ = 0;
    int J_ = 0;
    int I_ = -2;
    int H_ = 0;
    if (1 >= 0) {
     C = C_;
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f15:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = nondet();
    if (1 >= 0) {
     D = D_;
     goto loc_f8;
    }
   }
  goto end;
 }
 loc_f300:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (B >= E) {
     A = A_;
     goto loc_f8;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = nondet();
    int C_ = nondet();
    int K_ = 0;
    int J_ = 0;
    int H_ = 0;
    if (E >= 1 + B) {
     C = C_;
     F = F_;
     G = G_;
     H = H_;
     J = J_;
     K = K_;
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f8:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = nondet();
    if (A >= B) {
     C = C_;
     goto loc_f32;
    }
   }
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (B >= E && B >= 1 + A) {
     A = A_;
     goto loc_f8;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = nondet();
    int C_ = nondet();
    int K_ = 0;
    int J_ = 0;
    int H_ = 0;
    if (E >= 1 + B && B >= 1 + A) {
     C = C_;
     F = F_;
     G = G_;
     H = H_;
     J = J_;
     K = K_;
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f32:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
