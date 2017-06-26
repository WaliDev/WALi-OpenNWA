int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_evalEx3start;

 loc_evalEx3bb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    if (1 >= 0) {
     C = C_;
     goto loc_evalEx3bb2in;
    }
   }
  goto end;
 }
 loc_evalEx3bb2in:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = C;
    if (0 >= C) {
     A = A_;
     goto loc_evalEx3bb4in;
    }
   }
   if (nondet_bool()) {
    if (C >= 1) {
     goto loc_evalEx3bb3in;
    }
   }
  goto end;
 }
 loc_evalEx3bb3in:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalEx3bb1in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int A_ = C;
    if (B >= 1 + Y_0) {
     A = A_;
     goto loc_evalEx3bb4in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int A_ = C;
    if (Y_0 >= 1 + B) {
     A = A_;
     goto loc_evalEx3bb4in;
    }
   }
  goto end;
 }
 loc_evalEx3bb4in:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1) {
     goto loc_evalEx3bbin;
    }
   }
   if (nondet_bool()) {
    if (0 >= A) {
     goto loc_evalEx3returnin;
    }
   }
  goto end;
 }
 loc_evalEx3bbin:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = nondet();
    int C_ = A;
    if (1 >= 0) {
     B = B_;
     C = C_;
     goto loc_evalEx3bb2in;
    }
   }
  goto end;
 }
 loc_evalEx3entryin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalEx3bb4in;
    }
   }
  goto end;
 }
 loc_evalEx3returnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalEx3stop;
    }
   }
  goto end;
 }
 loc_evalEx3start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalEx3entryin;
    }
   }
  goto end;
 }
 loc_evalEx3stop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
