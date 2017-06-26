int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = nondet();
    int B_ = -1 + B;
    if (B >= 1) {
     B = B_;
     C = C_;
     goto loc_f1;
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
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = nondet();
    int A_ = 1;
    if (1 >= 0) {
     A = A_;
     C = C_;
     goto loc_f4;
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
