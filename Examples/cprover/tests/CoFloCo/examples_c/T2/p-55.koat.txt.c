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
    int B_ = 0;
    int A_ = -99 + A;
    if (1 + B == 0 && 0 >= 1 + A) {
     A = A_;
     B = B_;
     goto loc_f300;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    int A_ = 1 + A;
    if (B >= 0 && 0 >= 1 + A) {
     A = A_;
     B = B_;
     goto loc_f300;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    int A_ = 1 + A;
    if (0 >= 2 + B && 0 >= 1 + A) {
     A = A_;
     B = B_;
     goto loc_f300;
    }
   }
   if (nondet_bool()) {
    int C_ = nondet();
    if (A >= 0) {
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
