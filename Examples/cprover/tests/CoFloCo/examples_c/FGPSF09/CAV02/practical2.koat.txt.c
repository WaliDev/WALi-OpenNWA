int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_eval0;

 loc_eval0:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1;
    int A_ = B;
    if (1 >= 0) {
     A = A_;
     C = C_;
     goto loc_eval1;
    }
   }
  goto end;
 }
 loc_eval1:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 101) {
     goto loc_end;
    }
   }
   if (nondet_bool()) {
    if (100 >= A) {
     goto loc_eval3;
    }
   }
  goto end;
 }
 loc_eval11:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    int A_ = 11 + A;
    if (1 >= 0) {
     A = A_;
     C = C_;
     goto loc_eval5;
    }
   }
  goto end;
 }
 loc_eval3:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    int A_ = 11 + A;
    if (100 >= A) {
     A = A_;
     C = C_;
     goto loc_eval3;
    }
   }
   if (nondet_bool()) {
    if (A >= 101) {
     goto loc_eval5;
    }
   }
  goto end;
 }
 loc_eval5:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = -10 + A;
    if (C >= 2) {
     A = A_;
     C = C_;
     goto loc_eval7;
    }
   }
  goto end;
 }
 loc_eval7:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = -10 + A;
    if (C == 1 && A >= 101) {
     D = D_;
     goto loc_eval5;
    }
   }
   if (nondet_bool()) {
    if (100 >= A) {
     goto loc_eval9;
    }
   }
   if (nondet_bool()) {
    if (2 >= C) {
     goto loc_eval9;
    }
   }
   if (nondet_bool()) {
    if (C >= 0) {
     goto loc_eval9;
    }
   }
  goto end;
 }
 loc_eval9:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = -10 + A;
    if (A >= 101) {
     A = A_;
     C = C_;
     goto loc_eval11;
    }
   }
   if (nondet_bool()) {
    if (100 >= A) {
     goto loc_eval11;
    }
   }
  goto end;
 }
 loc_end:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
