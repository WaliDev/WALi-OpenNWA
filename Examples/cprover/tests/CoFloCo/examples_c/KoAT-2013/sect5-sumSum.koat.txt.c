int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_l0;

 loc_l0:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     goto loc_l1;
    }
   }
  goto end;
 }
 loc_l1:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 0;
    int C_ = 0;
    if (B >= 1) {
     C = C_;
     D = D_;
     goto loc_l2;
    }
   }
  goto end;
 }
 loc_l2:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = D + C;
    int C_ = 1 + C;
    if (B >= 1 + C) {
     C = C_;
     D = D_;
     goto loc_l2;
    }
   }
   if (nondet_bool()) {
    int B_ = -1 + B;
    int A_ = A + D;
    if (C >= B) {
     A = A_;
     B = B_;
     goto loc_l1;
    }
   }
  goto end;
 }
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
