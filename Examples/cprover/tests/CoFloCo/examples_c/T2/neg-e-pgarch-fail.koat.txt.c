int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = nondet();
    int G_ = nondet();
    int A_ = 1;
    if (0 >= H_) {
     A = A_;
     G = G_;
     H = H_;
     goto loc_f18;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int G_ = nondet();
    int I_ = 1;
    int H_ = 0;
    int A_ = 1;
    if (Y_0 >= 1 && K_ >= 1) {
     A = A_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     goto loc_f18;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int G_ = nondet();
    int I_ = 1;
    int H_ = 0;
    int A_ = 1;
    if (Y_0 >= 1 && 0 >= K_) {
     A = A_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     goto loc_f32;
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
    int F_ = nondet();
    int C_ = nondet();
    int A_ = 0;
    if (A >= 1) {
     A = A_;
     C = C_;
     F = F_;
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
     goto loc_f32;
    }
   }
   if (nondet_bool()) {
    int E_ = nondet();
    int D_ = nondet();
    int B_ = nondet();
    if (999 + C >= E_ && 0 >= A) {
     B = B_;
     D = D_;
     E = E_;
     goto loc_f32;
    }
   }
   if (nondet_bool()) {
    int E_ = nondet();
    int D_ = nondet();
    int B_ = nondet();
    int A_ = 1;
    if (E_ >= 1000 + C && 0 >= A) {
     A = A_;
     B = B_;
     D = D_;
     E = E_;
     goto loc_f32;
    }
   }
  goto end;
 }
 loc_f32:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f32;
    }
   }
  goto end;
 }
 loc_f34:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f37;
    }
   }
  goto end;
 }
 loc_f37:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
