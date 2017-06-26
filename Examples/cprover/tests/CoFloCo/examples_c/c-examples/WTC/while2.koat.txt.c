int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_evalwhile2start;

 loc_evalwhile2bb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    if (1 >= 0) {
     C = C_;
     goto loc_evalwhile2bb2in;
    }
   }
  goto end;
 }
 loc_evalwhile2bb2in:
 {
 __cost++;
   if (nondet_bool()) {
    if (C >= 1) {
     goto loc_evalwhile2bb1in;
    }
   }
   if (nondet_bool()) {
    if (0 >= C) {
     goto loc_evalwhile2bb3in;
    }
   }
  goto end;
 }
 loc_evalwhile2bb3in:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (1 >= 0) {
     A = A_;
     goto loc_evalwhile2bb4in;
    }
   }
  goto end;
 }
 loc_evalwhile2bb4in:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = B;
    if (A >= 1) {
     C = C_;
     goto loc_evalwhile2bb2in;
    }
   }
   if (nondet_bool()) {
    if (0 >= A) {
     goto loc_evalwhile2returnin;
    }
   }
  goto end;
 }
 loc_evalwhile2entryin:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = B;
    if (1 >= 0) {
     A = A_;
     goto loc_evalwhile2bb4in;
    }
   }
  goto end;
 }
 loc_evalwhile2returnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalwhile2stop;
    }
   }
  goto end;
 }
 loc_evalwhile2start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalwhile2entryin;
    }
   }
  goto end;
 }
 loc_evalwhile2stop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
