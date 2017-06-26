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
    int D_ = 0;
    int A_ = 2;
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
    int E_ = 0;
    if (A >= 1 + D) {
     E = E_;
     goto loc_f15;
    }
   }
   if (nondet_bool()) {
    int D_ = 0;
    if (D >= A) {
     D = D_;
     goto loc_f23;
    }
   }
  goto end;
 }
 loc_f15:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 1 + E;
    if (A >= 1 + E) {
     E = E_;
     goto loc_f15;
    }
   }
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (E >= A) {
     D = D_;
     goto loc_f12;
    }
   }
  goto end;
 }
 loc_f23:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 0;
    if (A >= 1 + D) {
     E = E_;
     goto loc_f26;
    }
   }
   if (nondet_bool()) {
    int G_ = 0;
    if (D >= A) {
     G = G_;
     goto loc_f52;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int G_ = 1;
    if (49 >= Y_0 && D >= A) {
     G = G_;
     goto loc_f52;
    }
   }
   if (nondet_bool()) {
    int G_ = 1;
    if (D >= A) {
     G = G_;
     goto loc_f52;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int G_ = 1;
    if (42 >= Y_0 && D >= A) {
     G = G_;
     goto loc_f52;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int G_ = 1;
    if (21 >= Y_0 && D >= A) {
     G = G_;
     goto loc_f52;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int G_ = 1;
    if (18 >= Y_0 && D >= A) {
     G = G_;
     goto loc_f52;
    }
   }
  goto end;
 }
 loc_f26:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = 0;
    if (A >= 1 + E) {
     F = F_;
     goto loc_f30;
    }
   }
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (E >= A) {
     D = D_;
     goto loc_f23;
    }
   }
  goto end;
 }
 loc_f30:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = 1 + F;
    if (A >= 1 + F) {
     F = F_;
     goto loc_f30;
    }
   }
   if (nondet_bool()) {
    int E_ = 1 + E;
    if (F >= A) {
     E = E_;
     goto loc_f26;
    }
   }
  goto end;
 }
 loc_f52:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
