int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K) {
 goto loc_f2;

 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f5;
    }
   }
  goto end;
 }
 loc_f23:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= D) {
     goto loc_f26;
    }
   }
   if (nondet_bool()) {
    if (D >= 1 + A) {
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f26:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = nondet();
    if (D >= 1 + B) {
     G = G_;
     goto loc_f30;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    if (B >= D) {
     C = C_;
     goto loc_f40;
    }
   }
  goto end;
 }
 loc_f30:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = nondet();
    int H_ = 1 + H;
    if (B >= 1 + H) {
     G = G_;
     H = H_;
     goto loc_f30;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (H >= B) {
     B = B_;
     goto loc_f26;
    }
   }
  goto end;
 }
 loc_f40:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = nondet();
    if (A >= B) {
     G = G_;
     goto loc_f44;
    }
   }
   if (nondet_bool()) {
    if (K >= 1 + D && B >= 1 + A) {
     goto loc_f59;
    }
   }
   if (nondet_bool()) {
    if (D >= 1 + K && B >= 1 + A) {
     goto loc_f59;
    }
   }
   if (nondet_bool()) {
    int K_ = D;
    if (D == K && B >= 1 + A) {
     K = K_;
     goto loc_f69;
    }
   }
  goto end;
 }
 loc_f44:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = nondet();
    int H_ = 1 + H;
    if (D >= 1 + H) {
     G = G_;
     H = H_;
     goto loc_f44;
    }
   }
   if (nondet_bool()) {
    int J_ = nondet();
    int I_ = nondet();
    int B_ = 1 + B;
    if (H >= D && C >= 1 + I_) {
     B = B_;
     I = I_;
     J = J_;
     goto loc_f40;
    }
   }
   if (nondet_bool()) {
    int J_ = nondet();
    int I_ = nondet();
    int C_ = nondet();
    int K_ = B;
    int B_ = 1 + B;
    if (H >= D && I_ >= C) {
     B = B_;
     C = C_;
     I = I_;
     J = J_;
     K = K_;
     goto loc_f40;
    }
   }
  goto end;
 }
 loc_f5:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 0;
    if (A >= B) {
     C = C_;
     goto loc_f9;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_f23;
    }
   }
  goto end;
 }
 loc_f59:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = nondet();
    int H_ = 1 + H;
    if (A >= H) {
     H = H_;
     I = I_;
     goto loc_f59;
    }
   }
   if (nondet_bool()) {
    if (H >= 1 + A) {
     goto loc_f69;
    }
   }
  goto end;
 }
 loc_f69:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (0 >= 1 + Y_0) {
     goto loc_f71;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (Y_0 >= 1) {
     goto loc_f71;
    }
   }
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f71;
    }
   }
  goto end;
 }
 loc_f71:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = nondet();
    if (A >= 1 + D) {
     I = I_;
     goto loc_f74;
    }
   }
   if (nondet_bool()) {
    int I_ = nondet();
    if (D >= 1 + A) {
     I = I_;
     goto loc_f74;
    }
   }
   if (nondet_bool()) {
    int D_ = 1 + A;
    if (A == D) {
     D = D_;
     goto loc_f23;
    }
   }
  goto end;
 }
 loc_f74:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (A >= B) {
     B = B_;
     goto loc_f74;
    }
   }
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (B >= 1 + A) {
     D = D_;
     goto loc_f23;
    }
   }
  goto end;
 }
 loc_f9:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = nondet();
    int E_ = nondet();
    int D_ = 1 + D;
    if (A >= D && C >= F_) {
     D = D_;
     E = E_;
     F = F_;
     goto loc_f9;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int E_ = nondet();
    int C_ = nondet();
    int D_ = 1 + D;
    if (A >= D && F_ >= 1 + C) {
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     goto loc_f9;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (D >= 1 + A && 0 >= 1 + C) {
     B = B_;
     goto loc_f5;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (D >= 1 + A && C >= 1) {
     B = B_;
     goto loc_f5;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    int B_ = 1 + B;
    if (C == 0 && D >= 1 + A) {
     B = B_;
     C = C_;
     goto loc_f5;
    }
   }
  goto end;
 }
 loc_f1:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
