int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_evalEx4start;

 loc_evalEx4bb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = -1 + D;
    int C_ = 1;
    if (1 >= 0) {
     C = C_;
     D = D_;
     goto loc_evalEx4bb2in;
    }
   }
  goto end;
 }
 loc_evalEx4bb2in:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = D;
    int A_ = C;
    if (0 >= D) {
     A = A_;
     B = B_;
     goto loc_evalEx4bb4in;
    }
   }
   if (nondet_bool()) {
    if (D >= 1) {
     goto loc_evalEx4bb3in;
    }
   }
  goto end;
 }
 loc_evalEx4bb3in:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (0 >= 1 + Y_0) {
     goto loc_evalEx4bb1in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (Y_0 >= 1) {
     goto loc_evalEx4bb1in;
    }
   }
   if (nondet_bool()) {
    int B_ = D;
    int A_ = C;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_evalEx4bb4in;
    }
   }
  goto end;
 }
 loc_evalEx4bb4in:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = B;
    int C_ = 0;
    if (A == 1) {
     C = C_;
     D = D_;
     goto loc_evalEx4bb2in;
    }
   }
   if (nondet_bool()) {
    if (0 >= A) {
     goto loc_evalEx4returnin;
    }
   }
   if (nondet_bool()) {
    if (A >= 2) {
     goto loc_evalEx4returnin;
    }
   }
  goto end;
 }
 loc_evalEx4entryin:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = A;
    int A_ = 1;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_evalEx4bb4in;
    }
   }
  goto end;
 }
 loc_evalEx4returnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalEx4stop;
    }
   }
  goto end;
 }
 loc_evalEx4start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalEx4entryin;
    }
   }
  goto end;
 }
 loc_evalEx4stop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
