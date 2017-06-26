int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I) {
 goto loc_evalbinsearchStepSize2start;

 loc_evalbinsearchStepSize2bb10in:
 {
 __cost++;
   if (nondet_bool()) {
    if (B == 0 && A == 2) {
     goto loc_evalbinsearchStepSize2bb12in;
    }
   }
   if (nondet_bool()) {
    int I_ = F;
    int H_ = B;
    if (1 >= A) {
     H = H_;
     I = I_;
     goto loc_evalbinsearchStepSize2bb13in;
    }
   }
   if (nondet_bool()) {
    int I_ = F;
    int H_ = B;
    if (A >= 3) {
     H = H_;
     I = I_;
     goto loc_evalbinsearchStepSize2bb13in;
    }
   }
   if (nondet_bool()) {
    int I_ = F;
    int H_ = B;
    if (0 >= 1 + B) {
     H = H_;
     I = I_;
     goto loc_evalbinsearchStepSize2bb13in;
    }
   }
   if (nondet_bool()) {
    int I_ = F;
    int H_ = B;
    if (B >= 1) {
     H = H_;
     I = I_;
     goto loc_evalbinsearchStepSize2bb13in;
    }
   }
  goto end;
 }
 loc_evalbinsearchStepSize2bb12in:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = 0;
    int H_ = 1;
    if (F == 0) {
     H = H_;
     I = I_;
     goto loc_evalbinsearchStepSize2bb13in;
    }
   }
   if (nondet_bool()) {
    int I_ = nondet();
    int H_ = 1;
    if (1 + 2 * I_ >= F && F >= 2 * I_ && I_ >= 0 && F >= 1) {
     H = H_;
     I = I_;
     goto loc_evalbinsearchStepSize2bb13in;
    }
   }
   if (nondet_bool()) {
    int I_ = nondet();
    int H_ = 1;
    if (1 + F >= 2 * I_ && 2 * I_ >= F && 0 >= I_ && 0 >= 1 + F) {
     H = H_;
     I = I_;
     goto loc_evalbinsearchStepSize2bb13in;
    }
   }
  goto end;
 }
 loc_evalbinsearchStepSize2bb13in:
 {
 __cost++;
   if (nondet_bool()) {
    if (I >= 1 + D) {
     goto loc_evalbinsearchStepSize2returnin;
    }
   }
   if (nondet_bool()) {
    int D_ = D + -I;
    int C_ = I;
    int B_ = H;
    int A_ = 1;
    if (D >= I) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     goto loc_evalbinsearchStepSize2bbin;
    }
   }
  goto end;
 }
 loc_evalbinsearchStepSize2bb1in:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + B) {
     goto loc_evalbinsearchStepSize2bb2in;
    }
   }
   if (nondet_bool()) {
    if (B >= 1) {
     goto loc_evalbinsearchStepSize2bb2in;
    }
   }
   if (nondet_bool()) {
    int F_ = C;
    if (B == 0) {
     F = F_;
     goto loc_evalbinsearchStepSize2bb3in;
    }
   }
  goto end;
 }
 loc_evalbinsearchStepSize2bb2in:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = 0;
    if (C == 0) {
     F = F_;
     goto loc_evalbinsearchStepSize2bb3in;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    if (1 + 2 * F_ >= C && C >= 2 * F_ && F_ >= 0 && C >= 1) {
     F = F_;
     goto loc_evalbinsearchStepSize2bb3in;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    if (1 + C >= 2 * F_ && 2 * F_ >= C && 0 >= F_ && 0 >= 1 + C) {
     F = F_;
     goto loc_evalbinsearchStepSize2bb3in;
    }
   }
  goto end;
 }
 loc_evalbinsearchStepSize2bb3in:
 {
 __cost++;
   if (nondet_bool()) {
    if (E >= 1 + G) {
     goto loc_evalbinsearchStepSize2bb4in;
    }
   }
   if (nondet_bool()) {
    if (G >= E) {
     goto loc_evalbinsearchStepSize2bb9in;
    }
   }
  goto end;
 }
 loc_evalbinsearchStepSize2bb4in:
 {
 __cost++;
   if (nondet_bool()) {
    if (B == 0 && A == 1) {
     goto loc_evalbinsearchStepSize2bb6in;
    }
   }
   if (nondet_bool()) {
    int I_ = F;
    int H_ = B;
    if (0 >= A) {
     H = H_;
     I = I_;
     goto loc_evalbinsearchStepSize2bb7in;
    }
   }
   if (nondet_bool()) {
    int I_ = F;
    int H_ = B;
    if (A >= 2) {
     H = H_;
     I = I_;
     goto loc_evalbinsearchStepSize2bb7in;
    }
   }
   if (nondet_bool()) {
    int I_ = F;
    int H_ = B;
    if (0 >= 1 + B) {
     H = H_;
     I = I_;
     goto loc_evalbinsearchStepSize2bb7in;
    }
   }
   if (nondet_bool()) {
    int I_ = F;
    int H_ = B;
    if (B >= 1) {
     H = H_;
     I = I_;
     goto loc_evalbinsearchStepSize2bb7in;
    }
   }
  goto end;
 }
 loc_evalbinsearchStepSize2bb6in:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = 0;
    int H_ = 1;
    if (F == 0) {
     H = H_;
     I = I_;
     goto loc_evalbinsearchStepSize2bb7in;
    }
   }
   if (nondet_bool()) {
    int I_ = nondet();
    int H_ = 1;
    if (1 + 2 * I_ >= F && F >= 2 * I_ && I_ >= 0 && F >= 1) {
     H = H_;
     I = I_;
     goto loc_evalbinsearchStepSize2bb7in;
    }
   }
   if (nondet_bool()) {
    int I_ = nondet();
    int H_ = 1;
    if (1 + F >= 2 * I_ && 2 * I_ >= F && 0 >= I_ && 0 >= 1 + F) {
     H = H_;
     I = I_;
     goto loc_evalbinsearchStepSize2bb7in;
    }
   }
  goto end;
 }
 loc_evalbinsearchStepSize2bb7in:
 {
 __cost++;
   if (nondet_bool()) {
    if (D + I >= 256) {
     goto loc_evalbinsearchStepSize2returnin;
    }
   }
   if (nondet_bool()) {
    int D_ = D + I;
    int C_ = I;
    int B_ = H;
    int A_ = 2;
    if (255 >= D + I) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     goto loc_evalbinsearchStepSize2bbin;
    }
   }
  goto end;
 }
 loc_evalbinsearchStepSize2bb9in:
 {
 __cost++;
   if (nondet_bool()) {
    if (G >= 1 + E) {
     goto loc_evalbinsearchStepSize2bb10in;
    }
   }
   if (nondet_bool()) {
    if (E >= G) {
     goto loc_evalbinsearchStepSize2returnin;
    }
   }
  goto end;
 }
 loc_evalbinsearchStepSize2bbin:
 {
 __cost++;
   if (nondet_bool()) {
    if (C == 1) {
     goto loc_evalbinsearchStepSize2returnin;
    }
   }
   if (nondet_bool()) {
    int E_ = nondet();
    if (0 >= C) {
     E = E_;
     goto loc_evalbinsearchStepSize2bb1in;
    }
   }
   if (nondet_bool()) {
    int E_ = nondet();
    if (C >= 2) {
     E = E_;
     goto loc_evalbinsearchStepSize2bb1in;
    }
   }
  goto end;
 }
 loc_evalbinsearchStepSize2entryin:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = A;
    int C_ = 4;
    int B_ = 0;
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     goto loc_evalbinsearchStepSize2bbin;
    }
   }
  goto end;
 }
 loc_evalbinsearchStepSize2returnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalbinsearchStepSize2stop;
    }
   }
  goto end;
 }
 loc_evalbinsearchStepSize2start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalbinsearchStepSize2entryin;
    }
   }
  goto end;
 }
 loc_evalbinsearchStepSize2stop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
