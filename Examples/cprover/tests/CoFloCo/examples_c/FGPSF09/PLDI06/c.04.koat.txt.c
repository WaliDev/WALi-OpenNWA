int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_eval0;

 loc_eval0:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1) {
     goto loc_eval1;
    }
   }
  goto end;
 }
 loc_eval1:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = B + A;
    if (A >= 1 && C >= 1 + A && A >= 1 + B) {
     B = B_;
     goto loc_eval1;
    }
   }
   if (nondet_bool()) {
    int C_ = B + -A;
    if (A >= 1 && C >= 1 + A && A >= 1 + B) {
     C = C_;
     goto loc_eval1;
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
