int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_evalwcet2start;

 loc_evalwcet2bb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (1 >= 0) {
     B = B_;
     goto loc_evalwcet2bb2in;
    }
   }
  goto end;
 }
 loc_evalwcet2bb2in:
 {
 __cost++;
   if (nondet_bool()) {
    if (9 >= B && A >= 3) {
     goto loc_evalwcet2bb1in;
    }
   }
   if (nondet_bool()) {
    if (2 >= A) {
     goto loc_evalwcet2bb4in;
    }
   }
   if (nondet_bool()) {
    if (B >= 10) {
     goto loc_evalwcet2bb4in;
    }
   }
  goto end;
 }
 loc_evalwcet2bb4in:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (1 >= 0) {
     A = A_;
     goto loc_evalwcet2bb5in;
    }
   }
  goto end;
 }
 loc_evalwcet2bb5in:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 0;
    if (4 >= A) {
     B = B_;
     goto loc_evalwcet2bb2in;
    }
   }
   if (nondet_bool()) {
    if (A >= 5) {
     goto loc_evalwcet2returnin;
    }
   }
  goto end;
 }
 loc_evalwcet2entryin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalwcet2bb5in;
    }
   }
  goto end;
 }
 loc_evalwcet2returnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalwcet2stop;
    }
   }
  goto end;
 }
 loc_evalwcet2start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalwcet2entryin;
    }
   }
  goto end;
 }
 loc_evalwcet2stop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
