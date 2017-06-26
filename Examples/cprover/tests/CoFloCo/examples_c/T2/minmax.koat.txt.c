int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int K_ = nondet();
    int J_ = 2;
    int I_ = 10;
    int E_ = 10;
    int D_ = 1;
    int B_ = 2;
    int A_ = 1;
    if (1 >= 0) {
     A = A_;
     B = B_;
     D = D_;
     E = E_;
     I = I_;
     J = J_;
     K = K_;
     goto loc_f38;
    }
   }
  goto end;
 }
 loc_f11:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = B + A;
    if (A >= B) {
     C = C_;
     goto loc_f54;
    }
   }
   if (nondet_bool()) {
    int C_ = B + -A;
    if (B >= 1 + A) {
     C = C_;
     goto loc_f54;
    }
   }
  goto end;
 }
 loc_f38:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = nondet();
    int G_ = D;
    int F_ = D;
    if (D >= 1 + E) {
     A = A_;
     F = F_;
     G = G_;
     goto loc_f11;
    }
   }
   if (nondet_bool()) {
    int H_ = nondet();
    int A_ = nondet();
    int G_ = D;
    int F_ = D;
    if (E >= D) {
     A = A_;
     F = F_;
     G = G_;
     H = H_;
     goto loc_f11;
    }
   }
  goto end;
 }
 loc_f54:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
