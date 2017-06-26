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
    int B_ = -1 + B;
    int A_ = 1 + A;
    if (B >= 1) {
     A = A_;
     B = B_;
     goto loc_l1;
    }
   }
   if (nondet_bool()) {
    int C_ = A;
    if (0 >= B) {
     C = C_;
     goto loc_l2;
    }
   }
  goto end;
 }
 loc_l2:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = C;
    if (C >= 1) {
     D = D_;
     goto loc_l3;
    }
   }
  goto end;
 }
 loc_l3:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = -1 + D;
    if (C >= 1 && D >= 1) {
     D = D_;
     goto loc_l3;
    }
   }
   if (nondet_bool()) {
    int C_ = -1 + C;
    if (C >= 1 && 0 >= D) {
     C = C_;
     goto loc_l2;
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
