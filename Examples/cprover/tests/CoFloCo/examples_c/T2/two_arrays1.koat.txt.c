int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = nondet();
    int B_ = nondet();
    int D_ = 0;
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     goto loc_f17;
    }
   }
  goto end;
 }
 loc_f17:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (49 >= D) {
     D = D_;
     goto loc_f17;
    }
   }
   if (nondet_bool()) {
    int E_ = 0;
    if (D >= 50) {
     E = E_;
     goto loc_f27;
    }
   }
  goto end;
 }
 loc_f27:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 1 + E;
    if (49 >= E) {
     E = E_;
     goto loc_f27;
    }
   }
   if (nondet_bool()) {
    int F_ = 0;
    if (E >= 50) {
     F = F_;
     goto loc_f37;
    }
   }
  goto end;
 }
 loc_f37:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = 1 + F;
    if (49 >= F) {
     F = F_;
     goto loc_f37;
    }
   }
   if (nondet_bool()) {
    int A_ = 0;
    if (F >= 50) {
     A = A_;
     goto loc_f45;
    }
   }
  goto end;
 }
 loc_f45:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (49 >= A) {
     A = A_;
     goto loc_f45;
    }
   }
   if (nondet_bool()) {
    int G_ = 0;
    if (A >= 50) {
     G = G_;
     goto loc_f55;
    }
   }
  goto end;
 }
 loc_f55:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = 1 + G;
    if (49 >= G) {
     G = G_;
     goto loc_f55;
    }
   }
   if (nondet_bool()) {
    int H_ = 0;
    if (G >= 50) {
     H = H_;
     goto loc_f65;
    }
   }
  goto end;
 }
 loc_f65:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = 1 + H;
    if (49 >= H) {
     H = H_;
     goto loc_f65;
    }
   }
   if (nondet_bool()) {
    int I_ = 0;
    if (H >= 50) {
     I = I_;
     goto loc_f75;
    }
   }
  goto end;
 }
 loc_f75:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = 1 + I;
    if (49 >= I) {
     I = I_;
     goto loc_f75;
    }
   }
   if (nondet_bool()) {
    int A_ = 0;
    if (I >= 50) {
     A = A_;
     goto loc_f83;
    }
   }
  goto end;
 }
 loc_f83:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (49 >= A) {
     A = A_;
     goto loc_f83;
    }
   }
   if (nondet_bool()) {
    if (A >= 50) {
     goto loc_f93;
    }
   }
  goto end;
 }
 loc_f93:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
