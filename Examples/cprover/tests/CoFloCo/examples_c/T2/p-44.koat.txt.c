int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_f2;

 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f300;
    }
   }
  goto end;
 }
 loc_f300:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = -2 + A;
    int A_ = -1 + A;
    if (B >= 1 && B + A >= 1 && A >= 1) {
     A = A_;
     B = B_;
     goto loc_f300;
    }
   }
   if (nondet_bool()) {
    int C_ = nondet();
    if (B >= 1 && 0 >= B + A && A >= 1) {
     C = C_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int C_ = nondet();
    if (0 >= A && B >= 1) {
     C = C_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int C_ = nondet();
    if (0 >= B) {
     C = C_;
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f1:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
