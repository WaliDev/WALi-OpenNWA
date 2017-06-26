int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_evalSimpleSingle2start;

 loc_evalSimpleSingle2bb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    int A_ = 1 + A;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_evalSimpleSingle2bb4in;
    }
   }
  goto end;
 }
 loc_evalSimpleSingle2bb2in:
 {
 __cost++;
   if (nondet_bool()) {
    if (D >= 1 + A) {
     goto loc_evalSimpleSingle2bb3in;
    }
   }
   if (nondet_bool()) {
    if (A >= D) {
     goto loc_evalSimpleSingle2returnin;
    }
   }
  goto end;
 }
 loc_evalSimpleSingle2bb3in:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    int A_ = 1 + A;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_evalSimpleSingle2bb4in;
    }
   }
  goto end;
 }
 loc_evalSimpleSingle2bb4in:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (0 >= 1 + Y_0) {
     goto loc_evalSimpleSingle2bbin;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (Y_0 >= 1) {
     goto loc_evalSimpleSingle2bbin;
    }
   }
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalSimpleSingle2returnin;
    }
   }
  goto end;
 }
 loc_evalSimpleSingle2bbin:
 {
 __cost++;
   if (nondet_bool()) {
    if (C >= 1 + B) {
     goto loc_evalSimpleSingle2bb1in;
    }
   }
   if (nondet_bool()) {
    if (B >= C) {
     goto loc_evalSimpleSingle2bb2in;
    }
   }
  goto end;
 }
 loc_evalSimpleSingle2entryin:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 0;
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_evalSimpleSingle2bb4in;
    }
   }
  goto end;
 }
 loc_evalSimpleSingle2returnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalSimpleSingle2stop;
    }
   }
  goto end;
 }
 loc_evalSimpleSingle2start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalSimpleSingle2entryin;
    }
   }
  goto end;
 }
 loc_evalSimpleSingle2stop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
