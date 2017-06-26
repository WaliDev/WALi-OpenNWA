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
    int A_ = 400;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_f7;
    }
   }
  goto end;
 }
 loc_f10:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 0;
    if (4 >= C) {
     D = D_;
     goto loc_f13;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (C >= 5) {
     B = B_;
     goto loc_f7;
    }
   }
  goto end;
 }
 loc_f13:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 0;
    if (4 >= D) {
     E = E_;
     goto loc_f16;
    }
   }
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (D >= 5) {
     C = C_;
     goto loc_f10;
    }
   }
  goto end;
 }
 loc_f16:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int E_ = 1 + E;
    if (A >= 1 + Y_0 && 4 >= E) {
     E = E_;
     goto loc_f16;
    }
   }
   if (nondet_bool()) {
    int E_ = 1 + E;
    if (4 >= E) {
     E = E_;
     goto loc_f16;
    }
   }
   if (nondet_bool()) {
    if (4 >= E) {
     goto loc_f31;
    }
   }
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (E >= 5) {
     D = D_;
     goto loc_f13;
    }
   }
  goto end;
 }
 loc_f7:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 0;
    if (4 >= B) {
     C = C_;
     goto loc_f10;
    }
   }
   if (nondet_bool()) {
    if (B >= 5) {
     goto loc_f31;
    }
   }
  goto end;
 }
 loc_f31:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet());
}
