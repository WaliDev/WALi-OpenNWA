int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 0;
    int C_ = 0;
    int B_ = 15;
    int A_ = 5;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     goto loc_f55;
    }
   }
  goto end;
 }
 loc_f55:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 1 + C;
    if (C == D && A >= 1 + D) {
     D = D_;
     goto loc_f55;
    }
   }
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (C >= 1 + D && A >= 1 + D) {
     D = D_;
     goto loc_f55;
    }
   }
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (D >= 1 + C && A >= 1 + D) {
     D = D_;
     goto loc_f55;
    }
   }
   if (nondet_bool()) {
    int D_ = 0;
    if (D >= A) {
     D = D_;
     goto loc_f63;
    }
   }
  goto end;
 }
 loc_f63:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 0;
    if (A >= 1 + D) {
     E = E_;
     goto loc_f66;
    }
   }
   if (nondet_bool()) {
    int D_ = 0;
    if (D >= A) {
     D = D_;
     goto loc_f77;
    }
   }
  goto end;
 }
 loc_f66:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = nondet();
    int E_ = 1 + E;
    if (B >= 1 + E) {
     E = E_;
     F = F_;
     G = G_;
     goto loc_f66;
    }
   }
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (E >= B) {
     D = D_;
     goto loc_f63;
    }
   }
  goto end;
 }
 loc_f77:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = nondet();
    if (B >= 1 + D) {
     F = F_;
     G = G_;
     goto loc_f83;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = nondet();
    int D_ = 1 + D;
    if (B >= 1 + D) {
     D = D_;
     F = F_;
     G = G_;
     goto loc_f77;
    }
   }
   if (nondet_bool()) {
    int D_ = 0;
    if (D >= B) {
     D = D_;
     goto loc_f87;
    }
   }
  goto end;
 }
 loc_f87:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (A >= 1 + D) {
     D = D_;
     goto loc_f87;
    }
   }
   if (nondet_bool()) {
    if (D >= A) {
     goto loc_f83;
    }
   }
  goto end;
 }
 loc_f83:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
