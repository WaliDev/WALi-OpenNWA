int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = nondet();
    int B_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_f10;
    }
   }
  goto end;
 }
 loc_f10:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (C >= 1 + B) {
     B = B_;
     goto loc_f10;
    }
   }
   if (nondet_bool()) {
    int E_ = 0;
    int D_ = C;
    if (B >= C) {
     D = D_;
     E = E_;
     goto loc_f18;
    }
   }
  goto end;
 }
 loc_f18:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = 0;
    if (D >= 2 + E) {
     F = F_;
     goto loc_f21;
    }
   }
   if (nondet_bool()) {
    int E_ = 0;
    if (1 + E >= D) {
     E = E_;
     goto loc_f32;
    }
   }
  goto end;
 }
 loc_f21:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = 1 + F;
    if (D >= 2 + E + F) {
     F = F_;
     goto loc_f21;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = 1 + F;
    if (D >= 2 + E + F) {
     F = F_;
     G = G_;
     goto loc_f21;
    }
   }
   if (nondet_bool()) {
    int E_ = 1 + E;
    if (1 + F + E >= D) {
     E = E_;
     goto loc_f18;
    }
   }
  goto end;
 }
 loc_f32:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 1 + E;
    if (D >= 2 + E) {
     E = E_;
     goto loc_f32;
    }
   }
   if (nondet_bool()) {
    if (1 + E >= D) {
     goto loc_f41;
    }
   }
  goto end;
 }
 loc_f41:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
