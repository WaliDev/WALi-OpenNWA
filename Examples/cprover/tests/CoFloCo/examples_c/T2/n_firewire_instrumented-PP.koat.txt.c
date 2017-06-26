int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = nondet();
    int C_ = nondet();
    int E_ = 1;
    int D_ = 0;
    int B_ = 0;
    int A_ = 0;
    if (G_ >= 1 && C_ >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     G = G_;
     goto loc_f11;
    }
   }
  goto end;
 }
 loc_f11:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = nondet();
    int D_ = 1 + D;
    int A_ = 0;
    if (0 >= D && 0 >= C && G >= 1 && 1 >= B_ && B_ >= 0) {
     A = A_;
     B = B_;
     D = D_;
     goto loc_f34;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int D_ = 1 + D;
    int A_ = 1;
    if (D == 1 && 0 >= C && G >= 1 && 1 >= B_ && B_ >= 0) {
     A = A_;
     B = B_;
     D = D_;
     goto loc_f34;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int D_ = 1 + D;
    int A_ = 0;
    if (D == 2 && 0 >= C && G >= 1 && 1 >= B_ && B_ >= 0) {
     A = A_;
     B = B_;
     D = D_;
     goto loc_f34;
    }
   }
   if (nondet_bool()) {
    int C_ = nondet();
    int B_ = nondet();
    int A_ = nondet();
    int E_ = 1 + E;
    int D_ = 0;
    if (D >= 3 && 0 >= C && G >= 1 && C_ >= 0 && 1 >= A_ && A_ >= 0 && 1 >= B_ && B_ >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f34;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = nondet();
    int C_ = -1 + C;
    if (G >= 1 && C >= 1 && 1 >= A_ && A_ >= 0 && 1 >= B_ && B_ >= 0) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f34;
    }
   }
   if (nondet_bool()) {
    if (0 >= G) {
     goto loc_f53;
    }
   }
  goto end;
 }
 loc_f34:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (0 >= D && 0 >= C && 0 >= B) {
     D = D_;
     goto loc_f38;
    }
   }
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (D == 1 && 0 >= C && B >= 1) {
     D = D_;
     goto loc_f38;
    }
   }
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (D == 2 && 0 >= C && 0 >= B) {
     D = D_;
     goto loc_f38;
    }
   }
   if (nondet_bool()) {
    int C_ = nondet();
    int E_ = 1 + E;
    int D_ = 0;
    if (D >= 3 && 0 >= C && C_ >= 0) {
     C = C_;
     D = D_;
     E = E_;
     goto loc_f38;
    }
   }
   if (nondet_bool()) {
    int C_ = -1 + C;
    if (C >= 1) {
     C = C_;
     goto loc_f38;
    }
   }
  goto end;
 }
 loc_f38:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = A;
    if (A == B) {
     B = B_;
     goto loc_f11;
    }
   }
   if (nondet_bool()) {
    int G_ = -1 + G;
    if (B >= 1 + A) {
     G = G_;
     goto loc_f11;
    }
   }
   if (nondet_bool()) {
    int G_ = -1 + G;
    if (A >= 1 + B) {
     G = G_;
     goto loc_f11;
    }
   }
  goto end;
 }
 loc_f53:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
