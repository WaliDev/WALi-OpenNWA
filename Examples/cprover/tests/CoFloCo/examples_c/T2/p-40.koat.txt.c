int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_f2;

 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = nondet();
    if (A >= 500) {
     B = B_;
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (499 >= A) {
     A = A_;
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = nondet();
    int B_ = nondet();
    int A_ = nondet();
    if (C_ >= 500) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    int C_ = nondet();
    int A_ = 1 + C_;
    if (499 >= C_) {
     A = A_;
     C = C_;
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f3:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
