int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_evalEx1start;

 loc_evalEx1bb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int D_ = -1 + D;
    if (0 >= 1 + Y_0) {
     D = D_;
     goto loc_evalEx1bb4in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int D_ = -1 + D;
    if (Y_0 >= 1 && 0 >= 1 + Y_0) {
     D = D_;
     goto loc_evalEx1bb4in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int D_ = -1 + D;
    if (Y_0 >= 1) {
     D = D_;
     goto loc_evalEx1bb4in;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1) {
     goto loc_evalEx1bb4in;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1) {
     goto loc_evalEx1bb4in;
    }
   }
   if (nondet_bool()) {
    int D_ = -1 + D;
    int C_ = 1 + C;
    if (0 >= 1) {
     C = C_;
     D = D_;
     goto loc_evalEx1bb4in;
    }
   }
   if (nondet_bool()) {
    int D_ = -1 + D;
    int C_ = 1 + C;
    if (0 >= 1) {
     C = C_;
     D = D_;
     goto loc_evalEx1bb4in;
    }
   }
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (1 >= 0) {
     C = C_;
     goto loc_evalEx1bb4in;
    }
   }
  goto end;
 }
 loc_evalEx1bb4in:
 {
 __cost++;
   if (nondet_bool()) {
    if (D >= 1 + C) {
     goto loc_evalEx1bb1in;
    }
   }
   if (nondet_bool()) {
    if (C >= D) {
     goto loc_evalEx1bb5in;
    }
   }
  goto end;
 }
 loc_evalEx1bb5in:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = D;
    int A_ = 1 + A;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_evalEx1bb6in;
    }
   }
  goto end;
 }
 loc_evalEx1bb6in:
 {
 __cost++;
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_evalEx1bbin;
    }
   }
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_evalEx1returnin;
    }
   }
  goto end;
 }
 loc_evalEx1bbin:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = B;
    int C_ = 1 + A;
    if (1 >= 0) {
     C = C_;
     D = D_;
     goto loc_evalEx1bb4in;
    }
   }
  goto end;
 }
 loc_evalEx1entryin:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = A;
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_evalEx1bb6in;
    }
   }
  goto end;
 }
 loc_evalEx1returnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalEx1stop;
    }
   }
  goto end;
 }
 loc_evalEx1start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalEx1entryin;
    }
   }
  goto end;
 }
 loc_evalEx1stop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
