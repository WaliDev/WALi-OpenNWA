int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_sqrt;

 loc_f:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 2 + C + B;
    int B_ = 2 + B;
    int A_ = 1 + A;
    if (B >= 0 && D >= C) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f;
    }
   }
   if (nondet_bool()) {
    if (C >= 1 + D) {
     goto loc_end;
    }
   }
  goto end;
 }
 loc_sqrt:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1;
    int B_ = 1;
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f;
    }
   }
  goto end;
 }
 loc_end:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
