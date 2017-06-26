int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = nondet();
    int D_ = nondet();
    int F_ = 0;
    int C_ = 285;
    int B_ = 35;
    int A_ = 10;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     goto loc_f15;
    }
   }
  goto end;
 }
 loc_f15:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = nondet();
    int H_ = 1;
    if (A >= 1 + F) {
     G = G_;
     H = H_;
     goto loc_f25;
    }
   }
   if (nondet_bool()) {
    if (F >= A) {
     goto loc_f48;
    }
   }
  goto end;
 }
 loc_f25:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = nondet();
    int H_ = 1 + H;
    if (E >= 1 + H) {
     G = G_;
     H = H_;
     goto loc_f25;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    if (Y_0 >= 1 + Y_1 && H >= E) {
     goto loc_f41;
    }
   }
   if (nondet_bool()) {
    if (H >= E) {
     goto loc_f41;
    }
   }
   if (nondet_bool()) {
    int F_ = 1 + F;
    int E_ = -1 + E;
    if (H >= E) {
     E = E_;
     F = F_;
     goto loc_f15;
    }
   }
  goto end;
 }
 loc_f41:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = 1 + F;
    if (E >= B) {
     F = F_;
     goto loc_f15;
    }
   }
   if (nondet_bool()) {
    int F_ = 1 + F;
    int E_ = 1 + E;
    if (B >= 1 + E) {
     E = E_;
     F = F_;
     goto loc_f15;
    }
   }
  goto end;
 }
 loc_f48:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
