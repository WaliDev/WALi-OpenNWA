int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = nondet();
    int B_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_f4;
    }
   }
  goto end;
 }
 loc_f4:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + A) {
     goto loc_f5;
    }
   }
   if (nondet_bool()) {
    if (A >= 1) {
     goto loc_f5;
    }
   }
   if (nondet_bool()) {
    int A_ = 0;
    if (A == 0) {
     A = A_;
     goto loc_f12;
    }
   }
  goto end;
 }
 loc_f5:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = nondet();
    int B_ = 1 + B;
    if (3 >= B) {
     A = A_;
     B = B_;
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int A_ = nondet();
    int B_ = 1 + B;
    if (B >= 5) {
     A = A_;
     B = B_;
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int A_ = nondet();
    int B_ = 1;
    if (B == 4) {
     A = A_;
     B = B_;
     goto loc_f4;
    }
   }
  goto end;
 }
 loc_f12:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
