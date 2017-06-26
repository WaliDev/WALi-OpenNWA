int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = nondet();
    int B_ = nondet();
    int A_ = nondet();
    if (0 >= 2 + B_) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f17;
    }
   }
   if (nondet_bool()) {
    int C_ = nondet();
    int B_ = nondet();
    int A_ = nondet();
    if (B_ >= 0) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f17;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int E_ = nondet();
    int A_ = nondet();
    int D_ = 0;
    int B_ = -1;
    if (1 >= 0) {
     A = A_;
     B = B_;
     D = D_;
     E = E_;
     F = F_;
     goto loc_f17;
    }
   }
  goto end;
 }
 loc_f17:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
