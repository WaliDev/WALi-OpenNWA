int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_evalrealheapsortstep2start;

 loc_evalrealheapsortstep2bb10in:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (1 >= 0) {
     B = B_;
     goto loc_evalrealheapsortstep2bb11in;
    }
   }
  goto end;
 }
 loc_evalrealheapsortstep2bb11in:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 2 + B) {
     goto loc_evalrealheapsortstep2bb1in;
    }
   }
   if (nondet_bool()) {
    if (1 + B >= A) {
     goto loc_evalrealheapsortstep2returnin;
    }
   }
  goto end;
 }
 loc_evalrealheapsortstep2bb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 0;
    if (1 >= 0) {
     C = C_;
     goto loc_evalrealheapsortstep2bb9in;
    }
   }
  goto end;
 }
 loc_evalrealheapsortstep2bb2in:
 {
 __cost++;
   if (nondet_bool()) {
    if (A == 3 + 2 * C + B) {
     goto loc_evalrealheapsortstep2bb4in;
    }
   }
   if (nondet_bool()) {
    if (A >= 4 + B + 2 * C) {
     goto loc_evalrealheapsortstep2bb3in;
    }
   }
   if (nondet_bool()) {
    if (2 + 2 * C + B >= A) {
     goto loc_evalrealheapsortstep2bb3in;
    }
   }
  goto end;
 }
 loc_evalrealheapsortstep2bb3in:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalrealheapsortstep2bb4in;
    }
   }
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalrealheapsortstep2bb5in;
    }
   }
  goto end;
 }
 loc_evalrealheapsortstep2bb4in:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 1 + 2 * C;
    if (1 >= 0) {
     D = D_;
     goto loc_evalrealheapsortstep2bb6in;
    }
   }
  goto end;
 }
 loc_evalrealheapsortstep2bb5in:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 2 + 2 * C;
    if (1 >= 0) {
     D = D_;
     goto loc_evalrealheapsortstep2bb6in;
    }
   }
  goto end;
 }
 loc_evalrealheapsortstep2bb6in:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalrealheapsortstep2bb7in;
    }
   }
   if (nondet_bool()) {
    int C_ = A;
    if (1 >= 0) {
     C = C_;
     goto loc_evalrealheapsortstep2bb9in;
    }
   }
  goto end;
 }
 loc_evalrealheapsortstep2bb7in:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = D;
    if (1 >= 0) {
     C = C_;
     goto loc_evalrealheapsortstep2bb9in;
    }
   }
  goto end;
 }
 loc_evalrealheapsortstep2bb9in:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 3 + B + 2 * C) {
     goto loc_evalrealheapsortstep2bb2in;
    }
   }
   if (nondet_bool()) {
    if (2 + 2 * C + B >= A) {
     goto loc_evalrealheapsortstep2bb10in;
    }
   }
  goto end;
 }
 loc_evalrealheapsortstep2bbin:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 0;
    if (1 >= 0) {
     B = B_;
     goto loc_evalrealheapsortstep2bb11in;
    }
   }
  goto end;
 }
 loc_evalrealheapsortstep2entryin:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 3) {
     goto loc_evalrealheapsortstep2bbin;
    }
   }
   if (nondet_bool()) {
    if (2 >= A) {
     goto loc_evalrealheapsortstep2returnin;
    }
   }
  goto end;
 }
 loc_evalrealheapsortstep2returnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalrealheapsortstep2stop;
    }
   }
  goto end;
 }
 loc_evalrealheapsortstep2start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalrealheapsortstep2entryin;
    }
   }
  goto end;
 }
 loc_evalrealheapsortstep2stop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
