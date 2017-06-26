int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = nondet();
    int E_ = 0;
    int D_ = 0;
    int C_ = 1;
    int B_ = 1;
    int A_ = 0;
    if (F_ >= 1) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     goto loc_f9;
    }
   }
  goto end;
 }
 loc_f20:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = 0;
    int D_ = 0;
    if (I == 0) {
     D = D_;
     I = I_;
     goto loc_f32;
    }
   }
   if (nondet_bool()) {
    int D_ = 1;
    if (0 >= 1 + I) {
     D = D_;
     goto loc_f32;
    }
   }
   if (nondet_bool()) {
    int D_ = 1;
    if (I >= 1) {
     D = D_;
     goto loc_f32;
    }
   }
  goto end;
 }
 loc_f32:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = 0;
    int E_ = 0;
    if (H == 0) {
     E = E_;
     H = H_;
     goto loc_f9;
    }
   }
   if (nondet_bool()) {
    int E_ = 1;
    if (0 >= 1 + H) {
     E = E_;
     goto loc_f9;
    }
   }
   if (nondet_bool()) {
    int E_ = 1;
    if (H >= 1) {
     E = E_;
     goto loc_f9;
    }
   }
  goto end;
 }
 loc_f9:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = -2 + A;
    int H_ = 1;
    int G_ = -2 + A;
    int F_ = 0;
    int E_ = D;
    int C_ = -1 + C;
    if (F == 0 && D == E && C >= 1 && A >= 3) {
     C = C_;
     E = E_;
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     goto loc_f20;
    }
   }
   if (nondet_bool()) {
    int I_ = A;
    int H_ = 0;
    int G_ = A;
    int F_ = 0;
    int E_ = D;
    int C_ = -1 + C;
    if (F == 0 && D == E && C >= 1 && 2 >= A) {
     C = C_;
     E = E_;
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     goto loc_f20;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int E_ = D;
    int C_ = 1 + B;
    int B_ = 1 + B;
    int A_ = 0;
    if (F == 0 && D == E && 0 >= C && A >= 3 && F_ >= 1) {
     A = A_;
     B = B_;
     C = C_;
     E = E_;
     F = F_;
     goto loc_f20;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int E_ = D;
    int C_ = 1 + B;
    int B_ = 1 + B;
    int A_ = 1 + A;
    if (F == 0 && D == E && 0 >= C && 2 >= A && F_ >= 1) {
     A = A_;
     B = B_;
     C = C_;
     E = E_;
     F = F_;
     goto loc_f20;
    }
   }
   if (nondet_bool()) {
    int I_ = nondet();
    int H_ = nondet();
    int F_ = -1 + F;
    int E_ = D;
    if (D == E && 0 >= 1 + F) {
     E = E_;
     F = F_;
     H = H_;
     I = I_;
     goto loc_f20;
    }
   }
   if (nondet_bool()) {
    int I_ = nondet();
    int H_ = nondet();
    int F_ = -1 + F;
    int E_ = D;
    if (D == E && F >= 1) {
     E = E_;
     F = F_;
     H = H_;
     I = I_;
     goto loc_f20;
    }
   }
   if (nondet_bool()) {
    if (E >= 1 + D) {
     goto loc_f38;
    }
   }
   if (nondet_bool()) {
    if (D >= 1 + E) {
     goto loc_f38;
    }
   }
  goto end;
 }
 loc_f38:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
