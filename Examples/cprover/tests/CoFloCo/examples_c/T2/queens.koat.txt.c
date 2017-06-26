int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = nondet();
    int E_ = 1;
    int D_ = 9;
    int C_ = 0;
    int A_ = 1;
    if (1 >= 0) {
     A = A_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     goto loc_f10;
    }
   }
  goto end;
 }
 loc_f10:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (D >= 1 + C) {
     C = C_;
     goto loc_f10;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    if (C >= D) {
     C = C_;
     goto loc_f16;
    }
   }
  goto end;
 }
 loc_f16:
 {
 __cost++;
   if (nondet_bool()) {
    if (D >= 1 + C && 0 >= 1 + E) {
     goto loc_f19;
    }
   }
   if (nondet_bool()) {
    if (D >= 1 + C && E >= 1) {
     goto loc_f19;
    }
   }
   if (nondet_bool()) {
    int G_ = 0;
    int E_ = 0;
    int B_ = 0;
    if (E == 0 && D >= 1 + C) {
     B = B_;
     E = E_;
     G = G_;
     goto loc_f27;
    }
   }
   if (nondet_bool()) {
    if (C >= D && 0 >= 1 + E) {
     goto loc_f49;
    }
   }
   if (nondet_bool()) {
    if (C >= D && E >= 1) {
     goto loc_f49;
    }
   }
   if (nondet_bool()) {
    int J_ = 1;
    int E_ = 0;
    if (E == 0 && C >= D) {
     E = E_;
     J = J_;
     goto loc_f56;
    }
   }
  goto end;
 }
 loc_f19:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int G_ = 1;
    int E_ = 1;
    int B_ = 0;
    if (D >= 1 + Y_0) {
     B = B_;
     E = E_;
     G = G_;
     goto loc_f27;
    }
   }
   if (nondet_bool()) {
    int G_ = 0;
    int E_ = 0;
    int B_ = 0;
    if (1 >= 0) {
     B = B_;
     E = E_;
     G = G_;
     goto loc_f27;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int G_ = 0;
    int E_ = 0;
    int B_ = 0;
    if (0 >= 1 + Y_0) {
     B = B_;
     E = E_;
     G = G_;
     goto loc_f27;
    }
   }
  goto end;
 }
 loc_f27:
 {
 __cost++;
   if (nondet_bool()) {
    if (B >= 1 + C && D >= 1 + B) {
     goto loc_f30;
    }
   }
   if (nondet_bool()) {
    if (D >= 1 + B && C >= 1 + B) {
     goto loc_f30;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + C;
    if (C == B && D >= 1 + B) {
     B = B_;
     goto loc_f27;
    }
   }
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (B >= D) {
     C = C_;
     goto loc_f16;
    }
   }
  goto end;
 }
 loc_f30:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + A) {
     goto loc_f31;
    }
   }
   if (nondet_bool()) {
    if (A >= 1) {
     goto loc_f31;
    }
   }
   if (nondet_bool()) {
    int H_ = 0;
    int A_ = 0;
    if (A == 0) {
     A = A_;
     H = H_;
     goto loc_f36;
    }
   }
  goto end;
 }
 loc_f31:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int H_ = 1;
    int A_ = 1;
    if (Y_0 >= 1 + Y_1) {
     A = A_;
     H = H_;
     goto loc_f36;
    }
   }
   if (nondet_bool()) {
    int H_ = 1;
    int A_ = 1;
    if (1 >= 0) {
     A = A_;
     H = H_;
     goto loc_f36;
    }
   }
   if (nondet_bool()) {
    int H_ = 0;
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     H = H_;
     goto loc_f36;
    }
   }
  goto end;
 }
 loc_f36:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + A) {
     goto loc_f37;
    }
   }
   if (nondet_bool()) {
    if (A >= 1) {
     goto loc_f37;
    }
   }
   if (nondet_bool()) {
    int I_ = 0;
    int B_ = 1 + B;
    int A_ = 0;
    if (A == 0) {
     A = A_;
     B = B_;
     I = I_;
     goto loc_f27;
    }
   }
  goto end;
 }
 loc_f37:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    if (B + Y_0 >= 1 + C + Y_1) {
     goto loc_f38;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    if (C + Y_0 >= 1 + B + Y_1) {
     goto loc_f38;
    }
   }
   if (nondet_bool()) {
    int I_ = 0;
    int B_ = 1 + B;
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     I = I_;
     goto loc_f27;
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
    int B_ = 1 + B;
    int A_ = 1;
    if (B + Y_0 >= 1 + C + Y_1) {
     A = A_;
     B = B_;
     I = I_;
     goto loc_f27;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int I_ = 1;
    int B_ = 1 + B;
    int A_ = 1;
    if (C + Y_0 >= 1 + B + Y_1) {
     A = A_;
     B = B_;
     I = I_;
     goto loc_f27;
    }
   }
   if (nondet_bool()) {
    int I_ = 0;
    int B_ = 1 + B;
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     I = I_;
     goto loc_f27;
    }
   }
  goto end;
 }
 loc_f49:
 {
 __cost++;
   if (nondet_bool()) {
    int J_ = 0;
    if (0 >= 1 + A) {
     J = J_;
     goto loc_f56;
    }
   }
   if (nondet_bool()) {
    int J_ = 0;
    if (A >= 1) {
     J = J_;
     goto loc_f56;
    }
   }
   if (nondet_bool()) {
    int J_ = 1;
    int A_ = 0;
    if (A == 0) {
     A = A_;
     J = J_;
     goto loc_f56;
    }
   }
  goto end;
 }
 loc_f56:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
