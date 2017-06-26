int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_evalEx2start;

 loc_evalEx2bb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = -1 + D;
    int C_ = 1 + C;
    if (1 >= 0) {
     C = C_;
     D = D_;
     goto loc_evalEx2bb2in;
    }
   }
  goto end;
 }
 loc_evalEx2bb2in:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (0 >= 1 + Y_0) {
     goto loc_evalEx2bb1in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (Y_0 >= 1) {
     goto loc_evalEx2bb1in;
    }
   }
   if (nondet_bool()) {
    int B_ = D;
    int A_ = C;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_evalEx2bb3in;
    }
   }
  goto end;
 }
 loc_evalEx2bb3in:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1 && B >= 1) {
     goto loc_evalEx2bbin;
    }
   }
   if (nondet_bool()) {
    if (0 >= B) {
     goto loc_evalEx2returnin;
    }
   }
   if (nondet_bool()) {
    if (0 >= A) {
     goto loc_evalEx2returnin;
    }
   }
  goto end;
 }
 loc_evalEx2bbin:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = -1 + B;
    int C_ = -1 + A;
    if (1 >= 0) {
     C = C_;
     D = D_;
     goto loc_evalEx2bb2in;
    }
   }
  goto end;
 }
 loc_evalEx2entryin:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = A;
    int A_ = B;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_evalEx2bb3in;
    }
   }
  goto end;
 }
 loc_evalEx2returnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalEx2stop;
    }
   }
  goto end;
 }
 loc_evalEx2start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalEx2entryin;
    }
   }
  goto end;
 }
 loc_evalEx2stop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
