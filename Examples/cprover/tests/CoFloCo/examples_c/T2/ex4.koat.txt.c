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
    int B_ = 0;
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f9;
    }
   }
  goto end;
 }
 loc_f10:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = nondet();
    int B_ = 1 + A;
    int A_ = 1 + A;
    if (9 >= A) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f9;
    }
   }
   if (nondet_bool()) {
    int A_ = 0;
    if (A >= 10) {
     A = A_;
     goto loc_f16;
    }
   }
  goto end;
 }
 loc_f16:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 10) {
     goto loc_f28;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = nondet();
    int E_ = A;
    int A_ = 1 + A;
    if (0 >= 1 + G_ && 9 >= A) {
     A = A_;
     E = E_;
     F = F_;
     G = G_;
     goto loc_f16;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = nondet();
    int E_ = A;
    int A_ = 1 + A;
    if (G_ >= 1 && 9 >= A) {
     A = A_;
     E = E_;
     F = F_;
     G = G_;
     goto loc_f16;
    }
   }
   if (nondet_bool()) {
    int G_ = 0;
    int F_ = 0;
    int E_ = A;
    if (9 >= A) {
     E = E_;
     F = F_;
     G = G_;
     goto loc_f28;
    }
   }
  goto end;
 }
 loc_f9:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = C;
    if (0 >= 1 + C) {
     D = D_;
     goto loc_f10;
    }
   }
   if (nondet_bool()) {
    int D_ = C;
    if (C >= 1) {
     D = D_;
     goto loc_f10;
    }
   }
   if (nondet_bool()) {
    int D_ = 0;
    int C_ = 0;
    int A_ = 0;
    if (C == 0) {
     A = A_;
     C = C_;
     D = D_;
     goto loc_f16;
    }
   }
  goto end;
 }
 loc_f28:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
