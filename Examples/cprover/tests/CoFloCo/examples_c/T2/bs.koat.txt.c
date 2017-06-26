int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = -1;
    int C_ = 14;
    int B_ = 0;
    int A_ = 8;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     goto loc_f6;
    }
   }
  goto end;
 }
 loc_f12:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + E;
    if (1 >= 0) {
     C = C_;
     goto loc_f6;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + E;
    if (1 >= 0) {
     B = B_;
     goto loc_f6;
    }
   }
  goto end;
 }
 loc_f6:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int E_ = nondet();
    if (A >= 1 + Y_0 && C >= B) {
     E = E_;
     goto loc_f12;
    }
   }
   if (nondet_bool()) {
    int E_ = nondet();
    if (C >= B) {
     E = E_;
     goto loc_f12;
    }
   }
   if (nondet_bool()) {
    int E_ = nondet();
    int D_ = nondet();
    int C_ = -1 + B;
    if (C >= B) {
     C = C_;
     D = D_;
     E = E_;
     goto loc_f6;
    }
   }
   if (nondet_bool()) {
    int G_ = D;
    int F_ = D;
    if (B >= 1 + C) {
     F = F_;
     G = G_;
     goto loc_f20;
    }
   }
  goto end;
 }
 loc_f20:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
