int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = nondet();
    int D_ = nondet();
    int F_ = 0;
    int C_ = 10;
    int B_ = 1;
    int A_ = 1;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     goto loc_f23;
    }
   }
  goto end;
 }
 loc_f23:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = 1 + F;
    if (C >= 1 + F) {
     F = F_;
     goto loc_f23;
    }
   }
   if (nondet_bool()) {
    int F_ = 0;
    if (F >= C) {
     F = F_;
     goto loc_f29;
    }
   }
  goto end;
 }
 loc_f29:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = 0;
    int G_ = 0;
    if (C >= 1 + F) {
     G = G_;
     H = H_;
     goto loc_f33;
    }
   }
   if (nondet_bool()) {
    int F_ = 0;
    if (F >= C) {
     F = F_;
     goto loc_f52;
    }
   }
  goto end;
 }
 loc_f33:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = 1;
    int H_ = 1 + H;
    int G_ = 1;
    if (C >= 1 + H && 0 >= 1 + G) {
     G = G_;
     H = H_;
     I = I_;
     goto loc_f33;
    }
   }
   if (nondet_bool()) {
    int I_ = 1;
    int H_ = 1 + H;
    int G_ = 1;
    if (C >= 1 + H && G >= 1) {
     G = G_;
     H = H_;
     I = I_;
     goto loc_f33;
    }
   }
   if (nondet_bool()) {
    int I_ = 1;
    int H_ = 1 + H;
    int G_ = 1;
    if (G == 0 && C >= 1 + H) {
     G = G_;
     H = H_;
     I = I_;
     goto loc_f33;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int I_ = 0;
    int H_ = 1 + H;
    int G_ = 0;
    if (G == 0 && C >= 1 + H && Y_0 >= 1 + Y_1) {
     G = G_;
     H = H_;
     I = I_;
     goto loc_f33;
    }
   }
   if (nondet_bool()) {
    int I_ = 0;
    int H_ = 1 + H;
    int G_ = 0;
    if (G == 0 && C >= 1 + H) {
     G = G_;
     H = H_;
     I = I_;
     goto loc_f33;
    }
   }
   if (nondet_bool()) {
    if (H >= C && 0 >= 1 + A) {
     goto loc_f44;
    }
   }
   if (nondet_bool()) {
    if (H >= C && A >= 1) {
     goto loc_f44;
    }
   }
   if (nondet_bool()) {
    int J_ = 0;
    int F_ = 1 + F;
    int A_ = 0;
    if (A == 0 && H >= C) {
     A = A_;
     F = F_;
     J = J_;
     goto loc_f29;
    }
   }
  goto end;
 }
 loc_f44:
 {
 __cost++;
   if (nondet_bool()) {
    int J_ = 1;
    int F_ = 1 + F;
    int A_ = 1;
    if (0 >= 1 + G) {
     A = A_;
     F = F_;
     J = J_;
     goto loc_f29;
    }
   }
   if (nondet_bool()) {
    int J_ = 1;
    int F_ = 1 + F;
    int A_ = 1;
    if (G >= 1) {
     A = A_;
     F = F_;
     J = J_;
     goto loc_f29;
    }
   }
   if (nondet_bool()) {
    int J_ = 0;
    int G_ = 0;
    int F_ = 1 + F;
    int A_ = 0;
    if (G == 0) {
     A = A_;
     F = F_;
     G = G_;
     J = J_;
     goto loc_f29;
    }
   }
  goto end;
 }
 loc_f52:
 {
 __cost++;
   if (nondet_bool()) {
    if (C >= 2 + F && 0 >= 1 + B) {
     goto loc_f55;
    }
   }
   if (nondet_bool()) {
    if (C >= 2 + F && B >= 1) {
     goto loc_f55;
    }
   }
   if (nondet_bool()) {
    int K_ = 0;
    int F_ = 1 + F;
    int B_ = 0;
    if (B == 0 && C >= 2 + F) {
     B = B_;
     F = F_;
     K = K_;
     goto loc_f52;
    }
   }
   if (nondet_bool()) {
    if (1 + F >= C && 0 >= 1 + A) {
     goto loc_f63;
    }
   }
   if (nondet_bool()) {
    if (1 + F >= C && A >= 1) {
     goto loc_f63;
    }
   }
   if (nondet_bool()) {
    int L_ = 1;
    int A_ = 0;
    if (A == 0 && 1 + F >= C) {
     A = A_;
     L = L_;
     goto loc_f71;
    }
   }
  goto end;
 }
 loc_f55:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int K_ = 1;
    int F_ = 1 + F;
    int B_ = 1;
    if (Y_0 >= 1 + Y_1) {
     B = B_;
     F = F_;
     K = K_;
     goto loc_f52;
    }
   }
   if (nondet_bool()) {
    int K_ = 0;
    int F_ = 1 + F;
    int B_ = 0;
    if (1 >= 0) {
     B = B_;
     F = F_;
     K = K_;
     goto loc_f52;
    }
   }
  goto end;
 }
 loc_f63:
 {
 __cost++;
   if (nondet_bool()) {
    int L_ = 0;
    if (0 >= 1 + B) {
     L = L_;
     goto loc_f71;
    }
   }
   if (nondet_bool()) {
    int L_ = 0;
    if (B >= 1) {
     L = L_;
     goto loc_f71;
    }
   }
   if (nondet_bool()) {
    int L_ = 1;
    int B_ = 0;
    if (B == 0) {
     B = B_;
     L = L_;
     goto loc_f71;
    }
   }
  goto end;
 }
 loc_f71:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
