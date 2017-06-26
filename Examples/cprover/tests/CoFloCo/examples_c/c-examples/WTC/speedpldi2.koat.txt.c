int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_evalspeedpldi2start;

 loc_evalspeedpldi2bb2in:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1 + B) {
     goto loc_evalspeedpldi2bb3in;
    }
   }
   if (nondet_bool()) {
    int B_ = 0;
    if (B >= A) {
     B = B_;
     goto loc_evalspeedpldi2bb5in;
    }
   }
  goto end;
 }
 loc_evalspeedpldi2bb3in:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int B_ = 1 + B;
    if (1 >= 0) {
     B = B_;
     C = C_;
     goto loc_evalspeedpldi2bb5in;
    }
   }
  goto end;
 }
 loc_evalspeedpldi2bb5in:
 {
 __cost++;
   if (nondet_bool()) {
    if (C >= 1) {
     goto loc_evalspeedpldi2bb2in;
    }
   }
   if (nondet_bool()) {
    if (0 >= C) {
     goto loc_evalspeedpldi2returnin;
    }
   }
  goto end;
 }
 loc_evalspeedpldi2entryin:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = A;
    int B_ = 0;
    int A_ = B;
    if (B >= 1 && A >= 0) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_evalspeedpldi2bb5in;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + A) {
     goto loc_evalspeedpldi2returnin;
    }
   }
   if (nondet_bool()) {
    if (0 >= B) {
     goto loc_evalspeedpldi2returnin;
    }
   }
  goto end;
 }
 loc_evalspeedpldi2returnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalspeedpldi2stop;
    }
   }
  goto end;
 }
 loc_evalspeedpldi2start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalspeedpldi2entryin;
    }
   }
  goto end;
 }
 loc_evalspeedpldi2stop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
