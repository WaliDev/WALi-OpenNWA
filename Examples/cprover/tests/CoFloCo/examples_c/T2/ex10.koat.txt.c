int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f5;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int A_ = nondet();
    if (15 >= Y_0) {
     A = A_;
     goto loc_f12;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int E_ = 1;
    int D_ = 1;
    if (15 >= Y_0) {
     D = D_;
     E = E_;
     goto loc_f5;
    }
   }
  goto end;
 }
 loc_f12:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = nondet();
    int B_ = nondet();
    if (0 >= 1 + A) {
     B = B_;
     C = C_;
     goto loc_f20;
    }
   }
   if (nondet_bool()) {
    int C_ = nondet();
    int B_ = nondet();
    if (A >= 1) {
     B = B_;
     C = C_;
     goto loc_f20;
    }
   }
   if (nondet_bool()) {
    int I_ = nondet();
    int H_ = nondet();
    int K_ = 0;
    int J_ = 0;
    int E_ = 0;
    int D_ = 0;
    int A_ = 0;
    if (A == 0) {
     A = A_;
     D = D_;
     E = E_;
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     goto loc_f5;
    }
   }
  goto end;
 }
 loc_f20:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = 0;
    int F_ = 0;
    int A_ = -1 + A;
    if (1 >= 0) {
     A = A_;
     F = F_;
     G = G_;
     goto loc_f12;
    }
   }
  goto end;
 }
 loc_f5:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
