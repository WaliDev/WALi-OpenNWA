int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = nondet();
    int A_ = nondet();
    int E_ = 0;
    int B_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     E = E_;
     F = F_;
     goto loc_f10;
    }
   }
  goto end;
 }
 loc_f10:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = nondet();
    int C_ = nondet();
    int B_ = 0;
    if (0 >= A) {
     B = B_;
     C = C_;
     D = D_;
     goto loc_f16;
    }
   }
   if (nondet_bool()) {
    if (A >= 1) {
     goto loc_f25;
    }
   }
  goto end;
 }
 loc_f16:
 {
 __cost++;
   if (nondet_bool()) {
    if (D >= 1) {
     goto loc_f16;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int A_ = nondet();
    int E_ = 0;
    if (0 >= D) {
     A = A_;
     E = E_;
     F = F_;
     goto loc_f10;
    }
   }
  goto end;
 }
 loc_f25:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f25;
    }
   }
  goto end;
 }
 loc_f27:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f30;
    }
   }
  goto end;
 }
 loc_f30:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
