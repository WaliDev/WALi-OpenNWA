int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = nondet();
    int A_ = 1;
    if (0 >= G_) {
     A = A_;
     F = F_;
     G = G_;
     goto loc_f18;
    }
   }
   if (nondet_bool()) {
    int J_ = nondet();
    int I_ = nondet();
    int F_ = nondet();
    int H_ = 1;
    int G_ = 0;
    int A_ = 1;
    if (J_ >= 1 && F_ >= 1) {
     A = A_;
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     goto loc_f18;
    }
   }
   if (nondet_bool()) {
    int J_ = nondet();
    int I_ = nondet();
    int F_ = nondet();
    int H_ = 1;
    int G_ = 0;
    int A_ = 1;
    if (0 >= J_ && F_ >= 1) {
     A = A_;
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     goto loc_f31;
    }
   }
  goto end;
 }
 loc_f18:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= A) {
     goto loc_f24;
    }
   }
   if (nondet_bool()) {
    int E_ = nondet();
    int C_ = nondet();
    int A_ = 0;
    if (A >= 1) {
     A = A_;
     C = C_;
     E = E_;
     goto loc_f24;
    }
   }
  goto end;
 }
 loc_f24:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = nondet();
    if (A >= 1) {
     B = B_;
     goto loc_f31;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int B_ = nondet();
    if (999 + C >= D_ && 0 >= A) {
     B = B_;
     D = D_;
     goto loc_f31;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int B_ = nondet();
    int A_ = 1;
    if (D_ >= 1000 + C && 0 >= A) {
     A = A_;
     B = B_;
     D = D_;
     goto loc_f31;
    }
   }
  goto end;
 }
 loc_f31:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f31;
    }
   }
  goto end;
 }
 loc_f33:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f36;
    }
   }
  goto end;
 }
 loc_f36:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
