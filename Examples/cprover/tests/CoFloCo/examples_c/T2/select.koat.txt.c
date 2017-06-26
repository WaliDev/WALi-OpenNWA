int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = 0;
    int F_ = 0;
    int E_ = 20;
    int D_ = 1;
    int C_ = 20;
    int B_ = 10;
    if (1 >= 0) {
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
    int G_ = 1;
    if (G == 0 && D >= E) {
     G = G_;
     goto loc_f11;
    }
   }
   if (nondet_bool()) {
    int G_ = 1;
    if (G == 0 && E >= 2 + D && 1 + D >= E) {
     G = G_;
     goto loc_f11;
    }
   }
   if (nondet_bool()) {
    int G_ = 1;
    int E_ = 1 + D;
    if (G == 0 && E == 1 + D) {
     E = E_;
     G = G_;
     goto loc_f11;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int H_ = nondet();
    int G_ = 1;
    int E_ = 1 + D;
    if (G == 0 && E == 1 + D && Y_0 >= 1 + Y_1) {
     E = E_;
     G = G_;
     H = H_;
     goto loc_f11;
    }
   }
   if (nondet_bool()) {
    int K_ = nondet();
    int I_ = nondet();
    int H_ = nondet();
    int J_ = 1 + D;
    int G_ = 0;
    int A_ = E;
    if (G == 0 && E >= 2 + D) {
     A = A_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     goto loc_f40;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + G) {
     goto loc_f69;
    }
   }
   if (nondet_bool()) {
    if (G >= 1) {
     goto loc_f69;
    }
   }
  goto end;
 }
 loc_f40:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + F) {
     goto loc_f59;
    }
   }
   if (nondet_bool()) {
    if (F >= 1) {
     goto loc_f59;
    }
   }
   if (nondet_bool()) {
    int J_ = 1 + J;
    int F_ = 0;
    if (F == 0) {
     F = F_;
     J = J_;
     goto loc_f43;
    }
   }
  goto end;
 }
 loc_f43:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int J_ = 1 + J;
    if (K >= 1 + Y_0) {
     J = J_;
     goto loc_f43;
    }
   }
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (1 >= 0) {
     A = A_;
     goto loc_f48;
    }
   }
  goto end;
 }
 loc_f48:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (1 >= 0) {
     A = A_;
     goto loc_f48;
    }
   }
   if (nondet_bool()) {
    if (A >= J) {
     goto loc_f54;
    }
   }
   if (nondet_bool()) {
    int F_ = 1;
    if (J >= 1 + A) {
     F = F_;
     goto loc_f54;
    }
   }
  goto end;
 }
 loc_f54:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + F) {
     goto loc_f40;
    }
   }
   if (nondet_bool()) {
    if (F >= 1) {
     goto loc_f40;
    }
   }
   if (nondet_bool()) {
    int H_ = nondet();
    int F_ = 0;
    if (F == 0) {
     F = F_;
     H = H_;
     goto loc_f40;
    }
   }
  goto end;
 }
 loc_f59:
 {
 __cost++;
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_f63;
    }
   }
   if (nondet_bool()) {
    int E_ = -1 + A;
    if (A >= B) {
     E = E_;
     goto loc_f63;
    }
   }
  goto end;
 }
 loc_f63:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1 + B) {
     goto loc_f11;
    }
   }
   if (nondet_bool()) {
    int D_ = J;
    if (B >= A) {
     D = D_;
     goto loc_f11;
    }
   }
  goto end;
 }
 loc_f69:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
