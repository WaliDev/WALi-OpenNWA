int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E) {
 goto loc_evalNestedMultiplestart;

 loc_evalNestedMultiplebb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 1 + E;
    if (1 >= 0) {
     E = E_;
     goto loc_evalNestedMultiplebb2in;
    }
   }
  goto end;
 }
 loc_evalNestedMultiplebb2in:
 {
 __cost++;
   if (nondet_bool()) {
    if (E >= C) {
     goto loc_evalNestedMultiplebb4in;
    }
   }
   if (nondet_bool()) {
    if (C >= 1 + E) {
     goto loc_evalNestedMultiplebb3in;
    }
   }
  goto end;
 }
 loc_evalNestedMultiplebb3in:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (0 >= 1 + Y_0) {
     goto loc_evalNestedMultiplebb1in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (Y_0 >= 1) {
     goto loc_evalNestedMultiplebb1in;
    }
   }
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalNestedMultiplebb4in;
    }
   }
  goto end;
 }
 loc_evalNestedMultiplebb4in:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = E;
    int B_ = 1 + B;
    if (1 >= 0) {
     B = B_;
     D = D_;
     goto loc_evalNestedMultiplebb5in;
    }
   }
  goto end;
 }
 loc_evalNestedMultiplebb5in:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = D;
    if (A >= 1 + B) {
     E = E_;
     goto loc_evalNestedMultiplebb2in;
    }
   }
   if (nondet_bool()) {
    if (B >= A) {
     goto loc_evalNestedMultiplereturnin;
    }
   }
  goto end;
 }
 loc_evalNestedMultipleentryin:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = C;
    int C_ = D;
    int B_ = A;
    int A_ = B;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     goto loc_evalNestedMultiplebb5in;
    }
   }
  goto end;
 }
 loc_evalNestedMultiplereturnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalNestedMultiplestop;
    }
   }
  goto end;
 }
 loc_evalNestedMultiplestart:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalNestedMultipleentryin;
    }
   }
  goto end;
 }
 loc_evalNestedMultiplestop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet());
}
