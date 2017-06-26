int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K) {
 goto loc_f300;

 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = nondet();
    int C_ = nondet();
    int B_ = nondet();
    int E_ = F;
    if (A >= 2) {
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int C_ = nondet();
    int B_ = nondet();
    if (1 >= A) {
     B = B_;
     C = C_;
     G = G_;
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f300:
 {
 __cost++;
   if (nondet_bool()) {
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int F_ = nondet();
    int E_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int B_ = nondet();
    if (H >= 1) {
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     I = I_;
     J = J_;
     K = K_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int G_ = nondet();
    int F_ = nondet();
    int E_ = nondet();
    int C_ = nondet();
    int B_ = nondet();
    if (0 >= H) {
     B = B_;
     C = C_;
     E = E_;
     F = F_;
     G = G_;
     I = I_;
     J = J_;
     K = K_;
     goto loc_f1;
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
