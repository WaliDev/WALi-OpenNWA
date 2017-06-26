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
    int B_ = 8;
    int A_ = 5;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     goto loc_f34;
    }
   }
  goto end;
 }
 loc_f34:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 1 + C;
    if (C == D && A >= 1 + D) {
     D = D_;
     goto loc_f34;
    }
   }
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (C >= 1 + D && A >= 1 + D) {
     D = D_;
     goto loc_f34;
    }
   }
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (D >= 1 + C && A >= 1 + D) {
     D = D_;
     goto loc_f34;
    }
   }
   if (nondet_bool()) {
    int D_ = 0;
    if (D >= A) {
     D = D_;
     goto loc_f42;
    }
   }
  goto end;
 }
 loc_f42:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 0;
    if (A >= 1 + D) {
     E = E_;
     goto loc_f45;
    }
   }
   if (nondet_bool()) {
    int D_ = 0;
    if (D >= A) {
     D = D_;
     goto loc_f56;
    }
   }
  goto end;
 }
 loc_f45:
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
     goto loc_f45;
    }
   }
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (E >= B) {
     D = D_;
     goto loc_f42;
    }
   }
  goto end;
 }
 loc_f56:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = nondet();
    if (B >= 1 + D) {
     F = F_;
     G = G_;
     goto loc_f62;
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
     goto loc_f56;
    }
   }
   if (nondet_bool()) {
    int D_ = 0;
    if (D >= B) {
     D = D_;
     goto loc_f66;
    }
   }
  goto end;
 }
 loc_f66:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (A >= 1 + D) {
     D = D_;
     goto loc_f66;
    }
   }
   if (nondet_bool()) {
    if (D >= A) {
     goto loc_f62;
    }
   }
  goto end;
 }
 loc_f62:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
