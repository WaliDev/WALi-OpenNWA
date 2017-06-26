int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_f3;

 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = nondet();
    if (B >= C) {
     D = D_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int B_ = 1 + B;
    int A_ = 1;
    if (A == 0 && 1 + B == C) {
     A = A_;
     B = B_;
     D = D_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int C_ = 1 + C;
    int B_ = 1 + B;
    int A_ = 0;
    if (A == 0 && C >= 1 + B && C >= 2 + B) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f3:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f2:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
