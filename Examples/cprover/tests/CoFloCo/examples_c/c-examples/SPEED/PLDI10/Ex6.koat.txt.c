int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_evalEx6start;

 loc_evalEx6bb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (1 >= 0) {
     B = B_;
     goto loc_evalEx6bb3in;
    }
   }
  goto end;
 }
 loc_evalEx6bb2in:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (1 >= 0) {
     A = A_;
     goto loc_evalEx6bb3in;
    }
   }
  goto end;
 }
 loc_evalEx6bb3in:
 {
 __cost++;
   if (nondet_bool()) {
    if (C >= 1 + B) {
     goto loc_evalEx6bbin;
    }
   }
   if (nondet_bool()) {
    if (B >= C) {
     goto loc_evalEx6returnin;
    }
   }
  goto end;
 }
 loc_evalEx6bbin:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1 + B) {
     goto loc_evalEx6bb1in;
    }
   }
   if (nondet_bool()) {
    if (B >= A) {
     goto loc_evalEx6bb2in;
    }
   }
  goto end;
 }
 loc_evalEx6entryin:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = A;
    int A_ = B;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_evalEx6bb3in;
    }
   }
  goto end;
 }
 loc_evalEx6returnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalEx6stop;
    }
   }
  goto end;
 }
 loc_evalEx6start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalEx6entryin;
    }
   }
  goto end;
 }
 loc_evalEx6stop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
