int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F) {
 goto loc_f10;

 loc_f10:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 0;
    int B_ = 0;
    int A_ = 4;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f4;
    }
   }
  goto end;
 }
 loc_f4:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = C;
    int D_ = C;
    if (1 >= 0) {
     D = D_;
     E = E_;
     goto loc_f9;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int B_ = 1 + B;
    if (1 >= 0) {
     B = B_;
     C = C_;
     D = D_;
     F = F_;
     goto loc_f4;
    }
   }
  goto end;
 }
 loc_f6:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int B_ = 1 + B;
    if (1 >= 0) {
     B = B_;
     C = C_;
     D = D_;
     F = F_;
     goto loc_f4;
    }
   }
  goto end;
 }
 loc_f7:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int B_ = 1 + B;
    if (1 >= 0) {
     B = B_;
     C = C_;
     D = D_;
     F = F_;
     goto loc_f4;
    }
   }
  goto end;
 }
 loc_f9:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
