int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_evalDis2start;

 loc_evalDis2bb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (1 >= 0) {
     C = C_;
     goto loc_evalDis2bb3in;
    }
   }
  goto end;
 }
 loc_evalDis2bb2in:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (1 >= 0) {
     B = B_;
     goto loc_evalDis2bb3in;
    }
   }
  goto end;
 }
 loc_evalDis2bb3in:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1 + C) {
     goto loc_evalDis2bbin;
    }
   }
   if (nondet_bool()) {
    if (C >= A) {
     goto loc_evalDis2returnin;
    }
   }
  goto end;
 }
 loc_evalDis2bbin:
 {
 __cost++;
   if (nondet_bool()) {
    if (B >= 1 + C) {
     goto loc_evalDis2bb1in;
    }
   }
   if (nondet_bool()) {
    if (C >= B) {
     goto loc_evalDis2bb2in;
    }
   }
  goto end;
 }
 loc_evalDis2entryin:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = A;
    int B_ = C;
    int A_ = B;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_evalDis2bb3in;
    }
   }
  goto end;
 }
 loc_evalDis2returnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalDis2stop;
    }
   }
  goto end;
 }
 loc_evalDis2start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalDis2entryin;
    }
   }
  goto end;
 }
 loc_evalDis2stop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
