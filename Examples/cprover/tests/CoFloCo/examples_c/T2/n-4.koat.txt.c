int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E) {
 goto loc_f5;

 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = nondet();
    int A_ = 1 + A;
    if (B >= 1 + A && D_ >= 1) {
     A = A_;
     D = D_;
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int A_ = 1 + A;
    if (B >= 1 + A && 0 >= 1 + D_) {
     A = A_;
     D = D_;
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int D_ = 0;
    if (B >= 1 + A) {
     D = D_;
     goto loc_f2;
    }
   }
  goto end;
 }
 loc_f300:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = nondet();
    if (C >= A) {
     E = E_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (A >= B && A >= 1 + C) {
     C = C_;
     goto loc_f300;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int A_ = 1 + A;
    if (B >= 1 + A && A >= 1 + C && D_ >= 1) {
     A = A_;
     D = D_;
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int A_ = 1 + A;
    if (B >= 1 + A && A >= 1 + C && 0 >= 1 + D_) {
     A = A_;
     D = D_;
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int D_ = 0;
    if (B >= 1 + A && A >= 1 + C) {
     D = D_;
     goto loc_f2;
    }
   }
  goto end;
 }
 loc_f4:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (A >= B) {
     C = C_;
     goto loc_f300;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int A_ = 1 + A;
    if (B >= 1 + A && D_ >= 1) {
     A = A_;
     D = D_;
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int A_ = 1 + A;
    if (B >= 1 + A && 0 >= 1 + D_) {
     A = A_;
     D = D_;
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int D_ = 0;
    if (B >= 1 + A) {
     D = D_;
     goto loc_f2;
    }
   }
  goto end;
 }
 loc_f5:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f300;
    }
   }
  goto end;
 }
 loc_f1:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet());
}
