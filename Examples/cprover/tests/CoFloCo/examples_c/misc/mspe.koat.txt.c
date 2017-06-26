int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F) {
 goto loc_start;

 loc_m1:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = nondet();
    int C_ = nondet();
    if (F_ >= 1 + F && 1 + F >= F_ && C_ >= 1 + C && 1 + C >= C_ && 1 + B >= F_ && A >= 1 + E && D >= 0 && B >= 1) {
     C = C_;
     F = F_;
     goto loc_m1;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int A_ = nondet();
    if (A_ >= 1 + A && 1 + A >= A_ && F_ >= 1 + F && 1 + F >= F_ && C >= 1 + B && 1 + E >= A_ && B >= F && D >= 0 && B >= 1) {
     A = A_;
     F = F_;
     goto loc_m1;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int C_ = nondet();
    if (C_ >= 1 + C && 1 + C >= C_ && F_ >= 1 + F && 1 + F >= F_ && E >= A && 1 + B >= C_ && B >= F && D >= 0 && B >= 1) {
     C = C_;
     F = F_;
     goto loc_m1;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int A_ = nondet();
    if (F_ >= 1 + F && 1 + F >= F_ && A_ >= 1 + A && 1 + A >= A_ && 1 + E >= A_ && B >= C && B >= F && D >= 0 && B >= 1) {
     A = A_;
     F = F_;
     goto loc_m1;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (F == A && 1 + E == C && D >= 0 && 2 * C >= B + A && B >= 1 + A && 2 + B + A >= 2 * C && A >= 0) {
     goto loc_m1;
    }
   }
  goto end;
 }
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
