int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 0;
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_f3;
    }
   }
  goto end;
 }
 loc_f3:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = nondet();
    int C_ = -1 + C;
    if (D_ >= 1 && C >= 1) {
     C = C_;
     D = D_;
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int C_ = -2 + C;
    if (0 >= D_ && C >= 1) {
     C = C_;
     D = D_;
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    int E_ = nondet();
    if (0 >= C) {
     E = E_;
     goto loc_f6;
    }
   }
  goto end;
 }
 loc_f6:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = nondet();
    int A_ = 1;
    if (E >= 1) {
     A = A_;
     E = E_;
     goto loc_f6;
    }
   }
   if (nondet_bool()) {
    int E_ = nondet();
    int A_ = 0;
    if (0 >= E) {
     A = A_;
     E = E_;
     goto loc_f6;
    }
   }
  goto end;
 }
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet());
}
