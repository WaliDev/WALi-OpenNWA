int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = nondet();
    if (A_ >= 1 && A_ >= 5 && A_ >= 2 && A_ >= 3) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = 1;
    if (0 >= 1 && 0 >= 4) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = nondet();
    if (A_ >= 1 && A_ >= 5 && 0 >= A_ && A_ >= 3) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = 1;
    if (0 >= 1 && 0 >= 4) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = 3;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = 1;
    if (0 >= 1) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = 3;
    if (0 >= 3) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = 1;
    if (0 >= 1) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = B;
    if (A == 2 * B && B >= 3 && B >= 2 && B >= 5) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = B;
    if (A == 2 * B && 1 >= B && B >= 2 && B >= 5) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = B;
    if (A == 2 * B && B >= 3 && 0 >= B && B >= 5) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = B;
    if (A == 2 * B && 1 >= B && 0 >= B && B >= 5) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = B;
    if (A == 6 && B == 3) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = B;
    if (A == 2 * B && 1 >= B && B >= 2 && 3 >= B) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = B;
    if (A == 6 && B == 3 && 0 >= 3) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = B;
    if (A == 2 * B && 1 >= B && 0 >= B && 3 >= B) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = 4 + 6 * B;
    if (A == 1 + 2 * B && 1 + 6 * B >= 0 && 2 + 6 * B >= 0 && 6 * B >= 1) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = 4 + 6 * B;
    if (A == 1 + 2 * B && 0 >= 3 + 6 * B && 2 + 6 * B >= 0 && 6 * B >= 1) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = 4 + 6 * B;
    if (A == 1 + 2 * B && 1 + 6 * B >= 0 && 0 >= 4 + 6 * B && 6 * B >= 1) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = 4 + 6 * B;
    if (A == 1 + 2 * B && 0 >= 3 + 6 * B && 0 >= 4 + 6 * B && 6 * B >= 1) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = 4 + 6 * B;
    if (A == 1 + 2 * B && 1 + 6 * B >= 0 && 2 + 6 * B >= 0 && 0 >= 1 + 6 * B) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = 4 + 6 * B;
    if (A == 1 + 2 * B && 0 >= 3 + 6 * B && 2 + 6 * B >= 0 && 0 >= 1 + 6 * B) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = 4 + 6 * B;
    if (A == 1 + 2 * B && 1 + 6 * B >= 0 && 0 >= 4 + 6 * B && 0 >= 1 + 6 * B) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = 4 + 6 * B;
    if (A == 1 + 2 * B && 0 >= 3 + 6 * B && 0 >= 4 + 6 * B && 0 >= 1 + 6 * B) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
  goto end;
 }
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
