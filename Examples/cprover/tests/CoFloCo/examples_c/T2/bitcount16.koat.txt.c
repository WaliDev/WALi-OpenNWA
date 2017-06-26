int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = nondet();
    int C_ = 0;
    int B_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f5;
    }
   }
  goto end;
 }
 loc_f5:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 1;
    int C_ = 1 + C;
    int B_ = 1 + B;
    if (15 >= C) {
     B = B_;
     C = C_;
     D = D_;
     goto loc_f5;
    }
   }
   if (nondet_bool()) {
    int D_ = 0;
    int C_ = 1 + C;
    if (15 >= C) {
     C = C_;
     D = D_;
     goto loc_f5;
    }
   }
   if (nondet_bool()) {
    int J_ = nondet();
    int I_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int F_ = B;
    int E_ = B;
    if (C >= 16) {
     E = E_;
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     goto loc_f27;
    }
   }
  goto end;
 }
 loc_f27:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
