int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F) {
 goto loc_f3;

 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = -2 + A;
    int D_ = B;
    int C_ = A;
    int B_ = -1 + B;
    int A_ = -1 + A;
    if (B >= 1 && A >= 1) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int B_ = nondet();
    if (0 >= B_ && 0 >= B) {
     B = B_;
     F = F_;
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    if (0 >= A && B >= 1) {
     F = F_;
     goto loc_f4;
    }
   }
  goto end;
 }
 loc_f3:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f2;
    }
   }
  goto end;
 }
 loc_f4:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
