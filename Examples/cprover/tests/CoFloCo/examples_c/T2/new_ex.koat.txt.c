int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = nondet();
    int B_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_f6;
    }
   }
  goto end;
 }
 loc_f10:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + A;
    int B_ = -1 + B;
    if (B >= 1) {
     B = B_;
     C = C_;
     goto loc_f14;
    }
   }
   if (nondet_bool()) {
    if (0 >= B) {
     goto loc_f6;
    }
   }
  goto end;
 }
 loc_f14:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 0;
    int C_ = -1 + C;
    if (C >= 1) {
     C = C_;
     D = D_;
     goto loc_f14;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int C_ = -1 + C;
    int B_ = 1 + B;
    int A_ = -1 + A;
    if (0 >= 1 + D_ && C >= 1) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     goto loc_f14;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int C_ = -1 + C;
    int B_ = 1 + B;
    int A_ = -1 + A;
    if (D_ >= 1 && C >= 1) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     goto loc_f14;
    }
   }
   if (nondet_bool()) {
    if (0 >= C) {
     goto loc_f10;
    }
   }
  goto end;
 }
 loc_f6:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    int A_ = -1 + A;
    if (A >= 1) {
     A = A_;
     B = B_;
     goto loc_f10;
    }
   }
   if (nondet_bool()) {
    if (0 >= A) {
     goto loc_f25;
    }
   }
  goto end;
 }
 loc_f25:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
