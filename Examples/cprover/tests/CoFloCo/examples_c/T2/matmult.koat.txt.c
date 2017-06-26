int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = 0;
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     H = H_;
     goto loc_f16;
    }
   }
  goto end;
 }
 loc_f16:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 0;
    if (19 >= A) {
     B = B_;
     goto loc_f19;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    if (A >= 20) {
     C = C_;
     goto loc_f33;
    }
   }
  goto end;
 }
 loc_f19:
 {
 __cost++;
   if (nondet_bool()) {
    int J_ = nondet();
    int H_ = nondet();
    int B_ = 1 + B;
    if (19 >= B) {
     B = B_;
     H = H_;
     J = J_;
     goto loc_f19;
    }
   }
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (B >= 20) {
     A = A_;
     goto loc_f16;
    }
   }
  goto end;
 }
 loc_f33:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 0;
    if (19 >= C) {
     D = D_;
     goto loc_f36;
    }
   }
   if (nondet_bool()) {
    int E_ = 0;
    if (C >= 20) {
     E = E_;
     goto loc_f52;
    }
   }
  goto end;
 }
 loc_f36:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = nondet();
    int H_ = nondet();
    int D_ = 1 + D;
    if (19 >= D) {
     D = D_;
     H = H_;
     I = I_;
     goto loc_f36;
    }
   }
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (D >= 20) {
     C = C_;
     goto loc_f33;
    }
   }
  goto end;
 }
 loc_f52:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = 0;
    if (19 >= E) {
     F = F_;
     goto loc_f55;
    }
   }
   if (nondet_bool()) {
    if (E >= 20) {
     goto loc_f73;
    }
   }
  goto end;
 }
 loc_f55:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = 0;
    if (19 >= F) {
     G = G_;
     goto loc_f59;
    }
   }
   if (nondet_bool()) {
    int E_ = 1 + E;
    if (F >= 20) {
     E = E_;
     goto loc_f52;
    }
   }
  goto end;
 }
 loc_f59:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = 1 + G;
    if (19 >= G) {
     G = G_;
     goto loc_f59;
    }
   }
   if (nondet_bool()) {
    int F_ = 1 + F;
    if (G >= 20) {
     F = F_;
     goto loc_f55;
    }
   }
  goto end;
 }
 loc_f73:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
