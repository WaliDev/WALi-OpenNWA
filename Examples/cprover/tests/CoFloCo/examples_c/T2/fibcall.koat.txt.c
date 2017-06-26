int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 2;
    int D_ = 0;
    int C_ = 1;
    int B_ = 30;
    int A_ = 30;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f7;
    }
   }
  goto end;
 }
 loc_f7:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = C;
    int E_ = 1 + E;
    int D_ = C;
    int C_ = C + D;
    if (B >= E) {
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    int I_ = C;
    int H_ = C;
    int G_ = C;
    if (E >= 1 + B) {
     G = G_;
     H = H_;
     I = I_;
     goto loc_f19;
    }
   }
  goto end;
 }
 loc_f19:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
