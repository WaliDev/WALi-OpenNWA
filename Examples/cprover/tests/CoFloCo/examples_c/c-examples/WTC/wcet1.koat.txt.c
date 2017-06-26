int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_evalwcet1start;

 loc_evalwcet1bb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 0;
    if (1 + B >= A) {
     D = D_;
     goto loc_evalwcet1bb6in;
    }
   }
   if (nondet_bool()) {
    int D_ = 1 + B;
    if (A >= 2 + B) {
     D = D_;
     goto loc_evalwcet1bb6in;
    }
   }
  goto end;
 }
 loc_evalwcet1bb4in:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= B) {
     goto loc_evalwcet1bb5in;
    }
   }
   if (nondet_bool()) {
    int D_ = -1 + B;
    if (B >= 2) {
     D = D_;
     goto loc_evalwcet1bb6in;
    }
   }
  goto end;
 }
 loc_evalwcet1bb5in:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 0;
    if (1 >= 0) {
     D = D_;
     goto loc_evalwcet1bb6in;
    }
   }
  goto end;
 }
 loc_evalwcet1bb6in:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int B_ = D;
    if (C >= 2) {
     B = B_;
     C = C_;
     goto loc_evalwcet1bbin;
    }
   }
   if (nondet_bool()) {
    if (1 >= C) {
     goto loc_evalwcet1returnin;
    }
   }
  goto end;
 }
 loc_evalwcet1bbin:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (0 >= 1 + Y_0) {
     goto loc_evalwcet1bb1in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (Y_0 >= 1) {
     goto loc_evalwcet1bb1in;
    }
   }
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalwcet1bb4in;
    }
   }
  goto end;
 }
 loc_evalwcet1entryin:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = A;
    int B_ = 0;
    if (A >= 1) {
     B = B_;
     C = C_;
     goto loc_evalwcet1bbin;
    }
   }
   if (nondet_bool()) {
    if (0 >= A) {
     goto loc_evalwcet1returnin;
    }
   }
  goto end;
 }
 loc_evalwcet1returnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalwcet1stop;
    }
   }
  goto end;
 }
 loc_evalwcet1start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalwcet1entryin;
    }
   }
  goto end;
 }
 loc_evalwcet1stop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
