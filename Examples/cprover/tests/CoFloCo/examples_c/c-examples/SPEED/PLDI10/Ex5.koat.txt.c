int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E) {
 goto loc_evalEx5start;

 loc_evalEx5bb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int E_ = -1 + D;
    if (0 >= 1 + Y_0) {
     E = E_;
     goto loc_evalEx5bb2in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int E_ = -1 + D;
    if (Y_0 >= 1 && 0 >= 1 + Y_0) {
     E = E_;
     goto loc_evalEx5bb2in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int E_ = -1 + D;
    if (Y_0 >= 1) {
     E = E_;
     goto loc_evalEx5bb2in;
    }
   }
   if (nondet_bool()) {
    int D_ = -1 + D;
    if (0 >= 1) {
     D = D_;
     goto loc_evalEx5bb3in;
    }
   }
   if (nondet_bool()) {
    int D_ = -1 + D;
    if (0 >= 1) {
     D = D_;
     goto loc_evalEx5bb3in;
    }
   }
   if (nondet_bool()) {
    int E_ = D;
    if (0 >= 1) {
     E = E_;
     goto loc_evalEx5bb2in;
    }
   }
   if (nondet_bool()) {
    int E_ = D;
    if (0 >= 1) {
     E = E_;
     goto loc_evalEx5bb2in;
    }
   }
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalEx5bb3in;
    }
   }
  goto end;
 }
 loc_evalEx5bb2in:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = E;
    int C_ = 1;
    if (1 >= 0) {
     C = C_;
     D = D_;
     goto loc_evalEx5bb3in;
    }
   }
  goto end;
 }
 loc_evalEx5bb3in:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (0 >= 1 + Y_0) {
     goto loc_evalEx5bb1in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (Y_0 >= 1) {
     goto loc_evalEx5bb1in;
    }
   }
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalEx5bb4in;
    }
   }
  goto end;
 }
 loc_evalEx5bb4in:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = D;
    int A_ = 1 + A;
    if (C == 0) {
     A = A_;
     B = B_;
     goto loc_evalEx5bb6in;
    }
   }
   if (nondet_bool()) {
    int B_ = D;
    if (0 >= 1 + C) {
     B = B_;
     goto loc_evalEx5bb6in;
    }
   }
   if (nondet_bool()) {
    int B_ = D;
    if (C >= 1) {
     B = B_;
     goto loc_evalEx5bb6in;
    }
   }
  goto end;
 }
 loc_evalEx5bb6in:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = B;
    int C_ = 0;
    if (B >= 1 + A) {
     C = C_;
     D = D_;
     goto loc_evalEx5bb3in;
    }
   }
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_evalEx5returnin;
    }
   }
  goto end;
 }
 loc_evalEx5entryin:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = A;
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_evalEx5bb6in;
    }
   }
  goto end;
 }
 loc_evalEx5returnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalEx5stop;
    }
   }
  goto end;
 }
 loc_evalEx5start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalEx5entryin;
    }
   }
  goto end;
 }
 loc_evalEx5stop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet());
}
