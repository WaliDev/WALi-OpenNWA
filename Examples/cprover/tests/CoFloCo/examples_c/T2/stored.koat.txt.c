int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = nondet();
    int C_ = 0;
    if (1 >= 0) {
     A = A_;
     C = C_;
     goto loc_f6;
    }
   }
  goto end;
 }
 loc_f17:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= C) {
     goto loc_f24;
    }
   }
   if (nondet_bool()) {
    if (C >= 2) {
     goto loc_f24;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    if (C == 1) {
     C = C_;
     goto loc_f24;
    }
   }
  goto end;
 }
 loc_f6:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = nondet();
    if (0 >= 1 + A) {
     B = B_;
     goto loc_f9;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    if (A >= 1) {
     B = B_;
     goto loc_f9;
    }
   }
   if (nondet_bool()) {
    int A_ = 0;
    if (A == 0) {
     A = A_;
     goto loc_f17;
    }
   }
  goto end;
 }
 loc_f9:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1;
    int B_ = 0;
    if (B == 0) {
     B = B_;
     C = C_;
     goto loc_f17;
    }
   }
   if (nondet_bool()) {
    int A_ = nondet();
    if (0 >= 1 + B) {
     A = A_;
     goto loc_f6;
    }
   }
   if (nondet_bool()) {
    int A_ = nondet();
    if (B >= 1) {
     A = A_;
     goto loc_f6;
    }
   }
  goto end;
 }
 loc_f24:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
