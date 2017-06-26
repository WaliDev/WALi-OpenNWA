int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     goto loc_f8;
    }
   }
  goto end;
 }
 loc_f14:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (1 >= 0) {
     A = A_;
     goto loc_f8;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int A_ = 1 + A;
    if (0 >= 1 + Y_0) {
     A = A_;
     goto loc_f8;
    }
   }
  goto end;
 }
 loc_f23:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int C_ = nondet();
    if (0 >= 1 + Y_0 && 0 >= A) {
     C = C_;
     goto loc_f28;
    }
   }
   if (nondet_bool()) {
    int C_ = nondet();
    if (0 >= A) {
     C = C_;
     goto loc_f28;
    }
   }
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (0 >= A) {
     A = A_;
     goto loc_f23;
    }
   }
   if (nondet_bool()) {
    if (A >= 1) {
     goto loc_f38;
    }
   }
  goto end;
 }
 loc_f28:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (1 >= 0) {
     A = A_;
     goto loc_f23;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int A_ = 1 + A;
    if (0 >= 1 + Y_0) {
     A = A_;
     goto loc_f23;
    }
   }
  goto end;
 }
 loc_f8:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int B_ = A;
    if (0 >= Y_0 && 0 >= A) {
     B = B_;
     goto loc_f14;
    }
   }
   if (nondet_bool()) {
    int B_ = A;
    if (0 >= A) {
     B = B_;
     goto loc_f14;
    }
   }
   if (nondet_bool()) {
    int B_ = A;
    int A_ = 1 + A;
    if (0 >= A) {
     A = A_;
     B = B_;
     goto loc_f8;
    }
   }
   if (nondet_bool()) {
    int A_ = 0;
    if (A >= 1) {
     A = A_;
     goto loc_f23;
    }
   }
  goto end;
 }
 loc_f38:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
