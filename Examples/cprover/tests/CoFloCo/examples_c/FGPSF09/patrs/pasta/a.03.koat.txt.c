int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_eval1;

 loc_eval1:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (A >= 2) {
     A = A_;
     goto loc_eval2;
    }
   }
   if (nondet_bool()) {
    int B_ = -1 + B;
    if (1 >= A) {
     B = B_;
     goto loc_eval2;
    }
   }
  goto end;
 }
 loc_eval2:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 2 * A;
    int C_ = A;
    if (B >= 2) {
     C = C_;
     D = D_;
     goto loc_eval3;
    }
   }
  goto end;
 }
 loc_eval3:
 {
 __cost++;
   if (nondet_bool()) {
    if (B >= 1 + D && B >= D) {
     goto loc_eval4;
    }
   }
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (B >= 1 + D && B >= D) {
     D = D_;
     goto loc_eval4;
    }
   }
   if (nondet_bool()) {
    int D_ = 2 * D;
    int C_ = D;
    if (D >= 1 && B >= 1 + D && B >= D) {
     C = C_;
     D = D_;
     goto loc_eval3;
    }
   }
   if (nondet_bool()) {
    int D_ = 2 + 2 * D;
    int C_ = 1 + D;
    if (D >= 1 && B >= 1 + D && B >= D) {
     C = C_;
     D = D_;
     goto loc_eval3;
    }
   }
   if (nondet_bool()) {
    if (B == D) {
     goto loc_eval4;
    }
   }
   if (nondet_bool()) {
    int D_ = 2 * D;
    int C_ = D;
    if (B == D && D >= 1) {
     C = C_;
     D = D_;
     goto loc_eval3;
    }
   }
  goto end;
 }
 loc_eval4:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (B >= 2 && A >= 1 && A >= 2) {
     A = A_;
     goto loc_eval2;
    }
   }
   if (nondet_bool()) {
    int B_ = -1 + B;
    if (A == 1 && B >= 2) {
     B = B_;
     goto loc_eval2;
    }
   }
  goto end;
 }
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
