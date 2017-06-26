int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_f1;

 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f2;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = nondet();
    if (B >= 2 && A >= 3) {
     C = C_;
     goto loc_f300;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int B_ = 2;
    int A_ = 1;
    if (A == 1 && B == 1 && 1 >= Y_0) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int B_ = 2;
    int A_ = 1 + A;
    if (B == 1 && 1 >= Y_0 && A >= 2) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int B_ = 2;
    int A_ = 1 + A;
    if (B == 1 && 1 >= Y_0 && 0 >= A) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    int A_ = 1;
    if (A == 1 && 1 >= B && B >= 2) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    int A_ = 1;
    if (A == 1 && 1 >= B && 0 >= B) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    int A_ = 1 + A;
    if (B >= 2 && A >= 2 && 1 >= B) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    int A_ = 1 + A;
    if (0 >= B && A >= 2 && 1 >= B) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    int A_ = 1 + A;
    if (B >= 2 && 0 >= A && 1 >= B) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    int A_ = 1 + A;
    if (0 >= B && 0 >= A && 1 >= B) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int B_ = 1 + B;
    int A_ = 1;
    if (A == 1 && 2 >= Y_0 && B >= 2) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int B_ = 1 + B;
    int A_ = 1;
    if (A == 1 && 2 >= Y_0 && 0 >= B && B >= 2) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    int A_ = 1 + A;
    if (A == 2 && B >= 2) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    int A_ = 1 + A;
    if (A == 2 && 0 >= B && B >= 2) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    int A_ = 1 + A;
    if (0 >= A && 2 >= A && B >= 2) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    int A_ = 1 + A;
    if (0 >= B && 0 >= A && 2 >= A && B >= 2) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
  goto end;
 }
 loc_f300:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
