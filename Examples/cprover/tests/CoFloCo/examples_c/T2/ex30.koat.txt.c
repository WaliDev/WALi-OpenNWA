int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = nondet();
    int B_ = nondet();
    int A_ = nondet();
    int D_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     goto loc_f12;
    }
   }
  goto end;
 }
 loc_f12:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (C >= 1 + D) {
     D = D_;
     goto loc_f12;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = 0;
    int E_ = A;
    if (D >= C) {
     E = E_;
     F = F_;
     G = G_;
     goto loc_f25;
    }
   }
  goto end;
 }
 loc_f25:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = 1 + F;
    if (E >= 1 + F) {
     F = F_;
     goto loc_f25;
    }
   }
   if (nondet_bool()) {
    if (F >= E) {
     goto loc_f34;
    }
   }
  goto end;
 }
 loc_f34:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
