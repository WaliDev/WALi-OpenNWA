int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = nondet();
    int F_ = 1;
    int E_ = 2;
    int D_ = 0;
    int B_ = 1;
    int A_ = 1;
    if (C_ >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     goto loc_f10;
    }
   }
  goto end;
 }
 loc_f10:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = 0;
    int B_ = -1 + B;
    if (0 >= C && B >= 1 && E >= F && F >= 1) {
     B = B_;
     G = G_;
     goto loc_f21;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int C_ = nondet();
    int B_ = 1 + A;
    int A_ = 1 + A;
    if (0 >= C && 0 >= B && E >= F && F >= 1 && C_ >= 0 && 1 >= G_ && G_ >= 0) {
     A = A_;
     B = B_;
     C = C_;
     G = G_;
     goto loc_f21;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int C_ = -1 + C;
    if (E >= F && C >= 1 && F >= 1 && 1 >= G_ && G_ >= 0) {
     C = C_;
     G = G_;
     goto loc_f21;
    }
   }
   if (nondet_bool()) {
    if (E >= F && 0 >= F) {
     goto loc_f31;
    }
   }
   if (nondet_bool()) {
    if (F >= 1 + E) {
     goto loc_f31;
    }
   }
  goto end;
 }
 loc_f21:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = -1 + F;
    if (0 >= G) {
     F = F_;
     goto loc_f10;
    }
   }
   if (nondet_bool()) {
    int F_ = 1 + F;
    if (G >= 1) {
     F = F_;
     goto loc_f10;
    }
   }
  goto end;
 }
 loc_f31:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
