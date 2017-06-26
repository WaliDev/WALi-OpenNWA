int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int E_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int B_ = 1;
    int A_ = 0;
    if (99 >= Y_0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f23;
    }
   }
   if (nondet_bool()) {
    int E_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int B_ = 1;
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f23;
    }
   }
  goto end;
 }
 loc_f23:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet());
}
