int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G) {
 goto loc_f5;

 loc_f4:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = A + B;
    if (A >= 0) {
     A = A_;
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int G_ = 0;
    int F_ = 0;
    int E_ = 0;
    int D_ = 0;
    int C_ = 0;
    if (0 >= 1 + A) {
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     G = G_;
     goto loc_f6;
    }
   }
  goto end;
 }
 loc_f5:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + B) {
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int G_ = 0;
    int F_ = 0;
    int E_ = 0;
    int D_ = 0;
    int C_ = 0;
    if (B >= 0) {
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     G = G_;
     goto loc_f6;
    }
   }
  goto end;
 }
 loc_f6:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
