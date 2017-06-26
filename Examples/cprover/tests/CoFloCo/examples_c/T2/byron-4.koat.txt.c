int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_f6;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = -A;
    if (1 >= 0) {
     A = A_;
     goto loc_f3;
    }
   }
  goto end;
 }
 loc_f3:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = 0;
    if (A == 0) {
     A = A_;
     B = B_;
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    int C_ = 1;
    int A_ = -1 + -A;
    if (0 >= C && 0 >= 1 + A) {
     A = A_;
     C = C_;
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int C_ = 1;
    int A_ = -1 + -A;
    if (C >= 2 && 0 >= 1 + A) {
     A = A_;
     C = C_;
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int C_ = 1;
    int A_ = -1 + -A;
    if (0 >= C && A >= 1) {
     A = A_;
     C = C_;
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int C_ = 1;
    int A_ = -1 + -A;
    if (C >= 2 && A >= 1) {
     A = A_;
     C = C_;
     goto loc_f4;
    }
   }
  goto end;
 }
 loc_f4:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = 0;
    if (A == 0) {
     A = A_;
     B = B_;
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    int A_ = 1 + -A;
    if (C == 1 && 0 >= 1 + A) {
     A = A_;
     C = C_;
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    int A_ = 1 + -A;
    if (C == 1 && A >= 1) {
     A = A_;
     C = C_;
     goto loc_f3;
    }
   }
  goto end;
 }
 loc_f5:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 0;
    int A_ = 1 + -A;
    if (C == 1 && 0 >= 1 + A) {
     A = A_;
     C = C_;
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    int A_ = 1 + -A;
    if (C == 1 && A >= 1) {
     A = A_;
     C = C_;
     goto loc_f3;
    }
   }
  goto end;
 }
 loc_f6:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1;
    if (A >= 1) {
     C = C_;
     goto loc_f4;
    }
   }
  goto end;
 }
 loc_f7:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
