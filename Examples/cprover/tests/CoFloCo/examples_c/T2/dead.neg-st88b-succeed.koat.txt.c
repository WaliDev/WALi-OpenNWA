int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = nondet();
    int B_ = nondet();
    int A_ = nondet();
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f11;
    }
   }
  goto end;
 }
 loc_f11:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 6) {
     goto loc_f14;
    }
   }
   if (nondet_bool()) {
    if (0 >= B && 5 >= A) {
     goto loc_f14;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 && 5 >= A) {
     goto loc_f20;
    }
   }
  goto end;
 }
 loc_f14:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = 1 + A;
    if (A >= 6) {
     A = A_;
     B = B_;
     goto loc_f11;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = 1 + A;
    if (5 >= A) {
     A = A_;
     B = B_;
     goto loc_f11;
    }
   }
  goto end;
 }
 loc_f20:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (A >= 3) {
     A = A_;
     goto loc_f20;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    if (2 >= A) {
     B = B_;
     goto loc_f11;
    }
   }
  goto end;
 }
 loc_f26:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f27;
    }
   }
  goto end;
 }
 loc_f27:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f27;
    }
   }
  goto end;
 }
 loc_f29:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f32;
    }
   }
  goto end;
 }
 loc_f32:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
