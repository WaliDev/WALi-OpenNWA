int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    if (E >= 0 && 3 >= E && D >= 0 && 3 >= C && 3 >= B && B >= 0 && 3 >= A && A >= 0) {
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = 1 + D;
    if (1 + B >= 2 * D) {
     F = F_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int F_ = -1 + D;
    if (2 * D >= 4 + B) {
     F = F_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int F_ = D;
    if (2 + B == 2 * D) {
     F = F_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int F_ = D;
    if (3 + B == 2 * D) {
     F = F_;
     goto loc_f2;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = 1 + E;
    if (D + A >= 1 + 2 * E) {
     G = G_;
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    int G_ = -1 + E;
    if (2 * E >= 2 + D + A) {
     G = G_;
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    int G_ = E;
    if (D + A == 2 * E) {
     G = G_;
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    int G_ = E;
    if (1 + D + A == 2 * E) {
     G = G_;
     goto loc_f3;
    }
   }
  goto end;
 }
 loc_f3:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = G;
    int D_ = F;
    if (D >= 1 + F) {
     D = D_;
     E = E_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int E_ = G;
    int D_ = F;
    if (F >= 1 + D) {
     D = D_;
     E = E_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int E_ = G;
    int D_ = F;
    if (E >= 1 + G) {
     D = D_;
     E = E_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int E_ = G;
    int D_ = F;
    if (G >= 1 + E) {
     D = D_;
     E = E_;
     goto loc_f1;
    }
   }
  goto end;
 }
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
