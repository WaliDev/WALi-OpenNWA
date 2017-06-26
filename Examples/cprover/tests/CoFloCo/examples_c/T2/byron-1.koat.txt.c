int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_f3;

 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    int A_ = -B + A;
    if (A >= 1) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int C_ = nondet();
    if (0 >= A) {
     C = C_;
     goto loc_f4;
    }
   }
  goto end;
 }
 loc_f3:
 {
 __cost++;
   if (nondet_bool()) {
    if (B >= 1) {
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int C_ = nondet();
    if (0 >= B) {
     C = C_;
     goto loc_f4;
    }
   }
  goto end;
 }
 loc_f4:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
