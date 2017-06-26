int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_f300;

 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = -1 + A;
    if (A >= 2 && B_ >= 1) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = -1 + A;
    if (A >= 2 && 0 >= 1 + B_) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int C_ = nondet();
    int B_ = nondet();
    int A_ = -1 + A;
    if (1 >= A && B_ >= 1) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int C_ = nondet();
    int B_ = nondet();
    int A_ = -1 + A;
    if (1 >= A && 0 >= 1 + B_) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int B_ = 0;
    if (A >= 1) {
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int C_ = nondet();
    int B_ = 0;
    if (0 >= A) {
     B = B_;
     C = C_;
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f300:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f2;
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
