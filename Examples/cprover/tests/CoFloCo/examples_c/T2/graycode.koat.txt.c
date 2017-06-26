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
    int F_ = 0;
    int D_ = 1;
    int C_ = 1;
    int B_ = 12;
    int A_ = 1;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     goto loc_f13;
    }
   }
  goto end;
 }
 loc_f13:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = 1 + F;
    if (B >= 1 + F) {
     F = F_;
     goto loc_f13;
    }
   }
   if (nondet_bool()) {
    int F_ = 0;
    if (F >= B) {
     F = F_;
     goto loc_f19;
    }
   }
  goto end;
 }
 loc_f19:
 {
 __cost++;
   if (nondet_bool()) {
    if (B >= 1 + F && 0 >= 1 + C) {
     goto loc_f22;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + F && C >= 1) {
     goto loc_f22;
    }
   }
   if (nondet_bool()) {
    int G_ = 0;
    int F_ = 1 + F;
    int C_ = 0;
    if (C == 0 && B >= 1 + F) {
     C = C_;
     F = F_;
     G = G_;
     goto loc_f19;
    }
   }
   if (nondet_bool()) {
    int F_ = 0;
    if (F >= B) {
     F = F_;
     goto loc_f32;
    }
   }
  goto end;
 }
 loc_f22:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int G_ = 1;
    int F_ = 1 + F;
    int C_ = 1;
    if (B >= 1 + Y_0 && Y_1 >= 0) {
     C = C_;
     F = F_;
     G = G_;
     goto loc_f19;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int G_ = 0;
    int F_ = 1 + F;
    int C_ = 0;
    if (Y_0 >= 0) {
     C = C_;
     F = F_;
     G = G_;
     goto loc_f19;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int G_ = 0;
    int F_ = 1 + F;
    int C_ = 0;
    if (0 >= 1 + Y_0) {
     C = C_;
     F = F_;
     G = G_;
     goto loc_f19;
    }
   }
  goto end;
 }
 loc_f32:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = 1 + F;
    if (B >= 2 + F) {
     H = H_;
     goto loc_f35;
    }
   }
   if (nondet_bool()) {
    int F_ = 0;
    if (1 + F >= B) {
     F = F_;
     goto loc_f48;
    }
   }
  goto end;
 }
 loc_f35:
 {
 __cost++;
   if (nondet_bool()) {
    if (B >= 1 + H && 0 >= 1 + A) {
     goto loc_f38;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + H && A >= 1) {
     goto loc_f38;
    }
   }
   if (nondet_bool()) {
    int I_ = 0;
    int H_ = 1 + H;
    int A_ = 0;
    if (A == 0 && B >= 1 + H) {
     A = A_;
     H = H_;
     I = I_;
     goto loc_f35;
    }
   }
   if (nondet_bool()) {
    int F_ = 1 + F;
    if (H >= B) {
     F = F_;
     goto loc_f32;
    }
   }
  goto end;
 }
 loc_f38:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int I_ = 1;
    int H_ = 1 + H;
    int A_ = 1;
    if (Y_0 >= 1 + Y_1) {
     A = A_;
     H = H_;
     I = I_;
     goto loc_f35;
    }
   }
   if (nondet_bool()) {
    int I_ = 1;
    int H_ = 1 + H;
    int A_ = 1;
    if (1 >= 0) {
     A = A_;
     H = H_;
     I = I_;
     goto loc_f35;
    }
   }
   if (nondet_bool()) {
    int I_ = 0;
    int H_ = 1 + H;
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     H = H_;
     I = I_;
     goto loc_f35;
    }
   }
  goto end;
 }
 loc_f48:
 {
 __cost++;
   if (nondet_bool()) {
    int J_ = nondet();
    if (B >= 2 + F && 0 >= 1 + D) {
     J = J_;
     goto loc_f52;
    }
   }
   if (nondet_bool()) {
    int J_ = nondet();
    if (B >= 2 + F && D >= 1) {
     J = J_;
     goto loc_f52;
    }
   }
   if (nondet_bool()) {
    int J_ = nondet();
    int K_ = 0;
    int F_ = 1 + F;
    int D_ = 0;
    if (D == 0 && B >= 2 + F) {
     D = D_;
     F = F_;
     J = J_;
     K = K_;
     goto loc_f48;
    }
   }
   if (nondet_bool()) {
    if (1 + F >= B && 0 >= 1 + C) {
     goto loc_f62;
    }
   }
   if (nondet_bool()) {
    if (1 + F >= B && C >= 1) {
     goto loc_f62;
    }
   }
   if (nondet_bool()) {
    int L_ = 1;
    int C_ = 0;
    if (C == 0 && 1 + F >= B) {
     C = C_;
     L = L_;
     goto loc_f71;
    }
   }
  goto end;
 }
 loc_f52:
 {
 __cost++;
   if (nondet_bool()) {
    int K_ = 1;
    int F_ = 1 + F;
    int D_ = 1;
    if (0 >= 1 + J) {
     D = D_;
     F = F_;
     K = K_;
     goto loc_f48;
    }
   }
   if (nondet_bool()) {
    int K_ = 1;
    int F_ = 1 + F;
    int D_ = 1;
    if (J >= 1) {
     D = D_;
     F = F_;
     K = K_;
     goto loc_f48;
    }
   }
   if (nondet_bool()) {
    int K_ = 0;
    int J_ = 0;
    int F_ = 1 + F;
    int D_ = 0;
    if (J == 0) {
     D = D_;
     F = F_;
     J = J_;
     K = K_;
     goto loc_f48;
    }
   }
   if (nondet_bool()) {
    int K_ = 0;
    int F_ = 1 + F;
    int D_ = 0;
    if (1 >= 0) {
     D = D_;
     F = F_;
     K = K_;
     goto loc_f48;
    }
   }
  goto end;
 }
 loc_f62:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + A) {
     goto loc_f63;
    }
   }
   if (nondet_bool()) {
    if (A >= 1) {
     goto loc_f63;
    }
   }
   if (nondet_bool()) {
    int L_ = 1;
    int A_ = 0;
    if (A == 0) {
     A = A_;
     L = L_;
     goto loc_f71;
    }
   }
  goto end;
 }
 loc_f63:
 {
 __cost++;
   if (nondet_bool()) {
    int L_ = 0;
    if (0 >= 1 + D) {
     L = L_;
     goto loc_f71;
    }
   }
   if (nondet_bool()) {
    int L_ = 0;
    if (D >= 1) {
     L = L_;
     goto loc_f71;
    }
   }
   if (nondet_bool()) {
    int L_ = 1;
    int D_ = 0;
    if (D == 0) {
     D = D_;
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
