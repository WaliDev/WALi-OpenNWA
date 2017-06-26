int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F) {
 goto loc_f1;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = nondet();
    if (0 >= A) {
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int D_ = nondet();
    int E_ = 0;
    int C_ = -1 + C;
    if (C >= 3 && A >= 1) {
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     goto loc_f0;
    }
   }
   if (nondet_bool()) {
    int E_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int A_ = -1 + A;
    if (A >= 1 && 0 >= 1 + E_) {
     A = A_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f0;
    }
   }
   if (nondet_bool()) {
    int E_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int A_ = -1 + A;
    if (A >= 1 && E_ >= 1) {
     A = A_;
     C = C_;
     D = D_;
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
    if (1 >= 0) {
     goto loc_f0;
    }
   }
  goto end;
 }
 loc_f2:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
