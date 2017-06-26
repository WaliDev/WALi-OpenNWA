int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1;
    int B_ = -1;
    int A_ = -1;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f8;
    }
   }
  goto end;
 }
 loc_f18:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = 1;
    int E_ = 1;
    if (99 >= D) {
     E = E_;
     F = F_;
     goto loc_f22;
    }
   }
   if (nondet_bool()) {
    if (D >= 100) {
     goto loc_f40;
    }
   }
  goto end;
 }
 loc_f22:
 {
 __cost++;
   if (nondet_bool()) {
    if (F >= 100) {
     goto loc_f33;
    }
   }
   if (nondet_bool()) {
    if (F + D >= 101 && 99 >= F) {
     goto loc_f33;
    }
   }
   if (nondet_bool()) {
    int F_ = 1 + F;
    if (100 >= D + F && 99 >= F) {
     F = F_;
     goto loc_f22;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = 1 + F;
    int E_ = 0;
    if (100 >= D + F && 99 >= F) {
     E = E_;
     F = F_;
     G = G_;
     goto loc_f22;
    }
   }
  goto end;
 }
 loc_f33:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 0;
    int D_ = 1 + D;
    if (E == 0) {
     D = D_;
     E = E_;
     goto loc_f18;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + E) {
     goto loc_f40;
    }
   }
   if (nondet_bool()) {
    if (E >= 1) {
     goto loc_f40;
    }
   }
  goto end;
 }
 loc_f8:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (100 >= C) {
     C = C_;
     goto loc_f8;
    }
   }
   if (nondet_bool()) {
    int E_ = 0;
    int D_ = 1;
    if (C >= 101) {
     D = D_;
     E = E_;
     goto loc_f18;
    }
   }
  goto end;
 }
 loc_f40:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
