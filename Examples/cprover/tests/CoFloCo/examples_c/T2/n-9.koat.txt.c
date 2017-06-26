int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G) {
 goto loc_f1;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = nondet();
    int A_ = nondet();
    if (A >= B) {
     A = A_;
     C = C_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int A_ = nondet();
    int E_ = 0;
    if (B >= 1 + A) {
     A = A_;
     D = D_;
     E = E_;
     goto loc_f0;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int E_ = nondet();
    int D_ = nondet();
    int A_ = nondet();
    if (B >= 1 + A && 0 >= 1 + E_) {
     A = A_;
     D = D_;
     E = E_;
     F = F_;
     goto loc_f0;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int E_ = nondet();
    int D_ = nondet();
    int A_ = nondet();
    if (B >= 1 + A && E_ >= 1) {
     A = A_;
     D = D_;
     E = E_;
     F = F_;
     goto loc_f0;
    }
   }
  goto end;
 }
 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = nondet();
    if (1 >= 0) {
     G = G_;
     goto loc_f0;
    }
   }
  goto end;
 }
 loc_f2:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
