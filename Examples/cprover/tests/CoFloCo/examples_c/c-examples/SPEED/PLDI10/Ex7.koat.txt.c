int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_evalEx7start;

 loc_evalEx7bb3in:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1 + C) {
     goto loc_evalEx7bbin;
    }
   }
   if (nondet_bool()) {
    if (C >= 1 + A) {
     goto loc_evalEx7bbin;
    }
   }
   if (nondet_bool()) {
    if (C == A) {
     goto loc_evalEx7returnin;
    }
   }
  goto end;
 }
 loc_evalEx7bbin:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 0;
    if (C >= 1 + B) {
     C = C_;
     goto loc_evalEx7bb3in;
    }
   }
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (B >= C) {
     C = C_;
     goto loc_evalEx7bb3in;
    }
   }
  goto end;
 }
 loc_evalEx7entryin:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + A;
    if (B >= 1 + A && A >= 1) {
     C = C_;
     goto loc_evalEx7bb3in;
    }
   }
  goto end;
 }
 loc_evalEx7returnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalEx7stop;
    }
   }
  goto end;
 }
 loc_evalEx7start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalEx7entryin;
    }
   }
  goto end;
 }
 loc_evalEx7stop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
