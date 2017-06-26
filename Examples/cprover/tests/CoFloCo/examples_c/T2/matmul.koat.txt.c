int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1;
    if (1 >= 0) {
     A = A_;
     goto loc_f10;
    }
   }
  goto end;
 }
 loc_f10:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1;
    if (5 >= A) {
     B = B_;
     goto loc_f13;
    }
   }
   if (nondet_bool()) {
    int A_ = 1;
    if (A >= 6) {
     A = A_;
     goto loc_f21;
    }
   }
  goto end;
 }
 loc_f13:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (5 >= B) {
     B = B_;
     goto loc_f13;
    }
   }
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (B >= 6) {
     A = A_;
     goto loc_f10;
    }
   }
  goto end;
 }
 loc_f21:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1;
    if (5 >= A) {
     B = B_;
     goto loc_f24;
    }
   }
   if (nondet_bool()) {
    if (A >= 6) {
     goto loc_f39;
    }
   }
  goto end;
 }
 loc_f24:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1;
    if (5 >= B) {
     C = C_;
     goto loc_f27;
    }
   }
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (B >= 6) {
     A = A_;
     goto loc_f21;
    }
   }
  goto end;
 }
 loc_f27:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (5 >= C) {
     C = C_;
     goto loc_f27;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (C >= 6) {
     B = B_;
     goto loc_f24;
    }
   }
  goto end;
 }
 loc_f39:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
