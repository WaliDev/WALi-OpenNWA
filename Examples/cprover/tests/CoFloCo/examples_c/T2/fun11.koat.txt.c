int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F) {
 goto loc_f4;

 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = nondet();
    int B_ = nondet();
    int A_ = 1 + A;
    if (A >= 1 && C_ >= 1) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int C_ = nondet();
    int B_ = nondet();
    int A_ = 1 + A;
    if (A >= 1 && 0 >= 1 + C_) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int C_ = 0;
    int B_ = 0;
    if (1 >= 0) {
     B = B_;
     C = C_;
     D = D_;
     goto loc_f0;
    }
   }
  goto end;
 }
 loc_f3:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = nondet();
    int B_ = nondet();
    int A_ = 1 + A;
    if (A >= 1 && C_ >= 1) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int C_ = nondet();
    int B_ = nondet();
    int A_ = 1 + A;
    if (A >= 1 && 0 >= 1 + C_) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int C_ = 0;
    int B_ = 0;
    if (1 >= 0) {
     B = B_;
     C = C_;
     D = D_;
     goto loc_f0;
    }
   }
  goto end;
 }
 loc_f4:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = nondet();
    int E_ = nondet();
    int C_ = nondet();
    int B_ = nondet();
    int A_ = 1 + F_;
    if (F_ >= 1 && C_ >= 1) {
     A = A_;
     B = B_;
     C = C_;
     E = E_;
     F = F_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int E_ = nondet();
    int C_ = nondet();
    int B_ = nondet();
    int A_ = 1 + F_;
    if (F_ >= 1 && 0 >= 1 + C_) {
     A = A_;
     B = B_;
     C = C_;
     E = E_;
     F = F_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int E_ = nondet();
    int D_ = nondet();
    int A_ = nondet();
    int C_ = 0;
    int B_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     goto loc_f0;
    }
   }
  goto end;
 }
 loc_f0:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
