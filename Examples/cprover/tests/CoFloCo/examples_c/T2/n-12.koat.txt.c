int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_f3;

 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = -1 + A;
    if (A >= 0 && B_ >= 1) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = -1 + A;
    if (A >= 0 && 0 >= 1 + B_) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = 0;
    if (A >= 0) {
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int C_ = nondet();
    if (0 >= 1 + A) {
     C = C_;
     goto loc_f300;
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
 loc_f300:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
