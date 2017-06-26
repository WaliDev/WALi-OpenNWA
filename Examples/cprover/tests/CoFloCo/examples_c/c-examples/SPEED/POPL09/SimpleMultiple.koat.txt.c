int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_evalSimpleMultiplestart;

 loc_evalSimpleMultiplebb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (1 >= 0) {
     A = A_;
     goto loc_evalSimpleMultiplebb3in;
    }
   }
  goto end;
 }
 loc_evalSimpleMultiplebb2in:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (1 >= 0) {
     B = B_;
     goto loc_evalSimpleMultiplebb3in;
    }
   }
  goto end;
 }
 loc_evalSimpleMultiplebb3in:
 {
 __cost++;
   if (nondet_bool()) {
    if (C >= 1 + B) {
     goto loc_evalSimpleMultiplebbin;
    }
   }
   if (nondet_bool()) {
    if (B >= C) {
     goto loc_evalSimpleMultiplereturnin;
    }
   }
  goto end;
 }
 loc_evalSimpleMultiplebbin:
 {
 __cost++;
   if (nondet_bool()) {
    if (D >= 1 + A) {
     goto loc_evalSimpleMultiplebb1in;
    }
   }
   if (nondet_bool()) {
    if (A >= D) {
     goto loc_evalSimpleMultiplebb2in;
    }
   }
  goto end;
 }
 loc_evalSimpleMultipleentryin:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 0;
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_evalSimpleMultiplebb3in;
    }
   }
  goto end;
 }
 loc_evalSimpleMultiplereturnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalSimpleMultiplestop;
    }
   }
  goto end;
 }
 loc_evalSimpleMultiplestart:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalSimpleMultipleentryin;
    }
   }
  goto end;
 }
 loc_evalSimpleMultiplestop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
