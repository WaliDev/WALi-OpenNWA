int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    if (B >= C && A >= 0) {
     goto loc_f2;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = -1 + A;
    if (1 + B >= C && A >= 1) {
     A = A_;
     C = C_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int C_ = -1 + C;
    int B_ = -1 + B + C;
    if (B >= 0 && A >= 0) {
     B = B_;
     C = C_;
     goto loc_f2;
    }
   }
  goto end;
 }
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
