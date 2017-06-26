int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F) {
 goto loc_f1;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + A;
    int B_ = B + A;
    int A_ = -A;
    if (0 >= 1 + A) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f0;
    }
   }
   if (nondet_bool()) {
    int D_ = D + B;
    int B_ = -B;
    int A_ = A + B;
    if (0 >= 1 + B) {
     A = A_;
     B = B_;
     D = D_;
     goto loc_f0;
    }
   }
   if (nondet_bool()) {
    int E_ = E + D;
    int D_ = -D;
    int B_ = B + D;
    if (0 >= 1 + D) {
     B = B_;
     D = D_;
     E = E_;
     goto loc_f0;
    }
   }
   if (nondet_bool()) {
    int E_ = -E;
    int D_ = D + E;
    int C_ = C + E;
    if (0 >= 1 + E) {
     C = C_;
     D = D_;
     E = E_;
     goto loc_f0;
    }
   }
   if (nondet_bool()) {
    int E_ = E + C;
    int C_ = -C;
    int A_ = A + C;
    if (0 >= 1 + C) {
     A = A_;
     C = C_;
     E = E_;
     goto loc_f0;
    }
   }
  goto end;
 }
 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int B_ = nondet();
    int A_ = nondet();
    int F_ = A_ + B_ + D_ + E_ + C_;
    if (A_ + B_ + D_ + E_ + C_ >= 1) {
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
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
