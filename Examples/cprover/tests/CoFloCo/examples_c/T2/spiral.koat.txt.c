int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    if (B >= 1 && A >= 1) {
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + B && A >= 1) {
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 && 0 >= 1 + A) {
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + B && 0 >= 1 + A) {
     goto loc_f2;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = -1 + B;
    if (A + B >= 2 && A >= 1 && B >= 2 && A >= B) {
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = -1 + B;
    if (A + B >= 2 && 0 >= 1 + A && B >= 2 && A >= B) {
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = -1 + B;
    if (A + B >= 2 && A >= 1 && 0 >= B && A >= B) {
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = -1 + B;
    if (A + B >= 2 && 0 >= 1 + A && 0 >= B && A >= B) {
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (B >= 1 && A >= 2 && A >= 1 + B && 1 >= A + B) {
     A = A_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (0 >= 1 + B && A >= 2 && A >= 1 + B && 1 >= A + B) {
     A = A_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (B >= 1 && 0 >= A && A >= 1 + B && 1 >= A + B) {
     A = A_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (0 >= 1 + B && 0 >= A && A >= 1 + B && 1 >= A + B) {
     A = A_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (0 >= 1 + B + A && A >= 1 && B >= 0 && B >= A) {
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (0 >= 1 + B + A && 0 >= 1 + A && B >= 0 && B >= A) {
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (0 >= 1 + B + A && A >= 1 && 0 >= 2 + B && B >= A) {
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (0 >= 1 + B + A && 0 >= 1 + A && 0 >= 2 + B && B >= A) {
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (B >= 1 && A >= 0 && B >= 1 + A && B + A >= 0) {
     A = A_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (0 >= 1 + B && A >= 0 && B >= 1 + A && B + A >= 0) {
     A = A_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (B >= 1 && 0 >= 2 + A && B >= 1 + A && B + A >= 0) {
     A = A_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (0 >= 1 + B && 0 >= 2 + A && B >= 1 + A && B + A >= 0) {
     A = A_;
     goto loc_f2;
    }
   }
  goto end;
 }
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
