int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = nondet();
    int D_ = nondet();
    int E_ = 10;
    int C_ = 10;
    int B_ = 0;
    int A_ = 10;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     goto loc_f18;
    }
   }
  goto end;
 }
 loc_f18:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (A >= 1 + B) {
     B = B_;
     goto loc_f18;
    }
   }
   if (nondet_bool()) {
    int B_ = 0;
    if (B >= A) {
     B = B_;
     goto loc_f24;
    }
   }
  goto end;
 }
 loc_f24:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (A >= 1 + B) {
     B = B_;
     goto loc_f24;
    }
   }
   if (nondet_bool()) {
    int B_ = 0;
    if (B >= A) {
     B = B_;
     goto loc_f31;
    }
   }
  goto end;
 }
 loc_f31:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (A >= 1 + B) {
     B = B_;
     goto loc_f31;
    }
   }
   if (nondet_bool()) {
    if (B >= A) {
     goto loc_f39;
    }
   }
  goto end;
 }
 loc_f39:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
