int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = D;
    int A_ = B;
    if (1 >= 0) {
     A = A_;
     C = C_;
     goto loc_f6;
    }
   }
  goto end;
 }
 loc_f6:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = -1 + A;
    if (0 >= 1 + A) {
     A = A_;
     C = C_;
     goto loc_f6;
    }
   }
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = -1 + A;
    if (A >= 1) {
     A = A_;
     C = C_;
     goto loc_f6;
    }
   }
   if (nondet_bool()) {
    int A_ = 0;
    if (A == 0 && D >= 1 + B) {
     A = A_;
     goto loc_f14;
    }
   }
   if (nondet_bool()) {
    int A_ = 0;
    if (A == 0 && B >= 1 + D) {
     A = A_;
     goto loc_f14;
    }
   }
   if (nondet_bool()) {
    int D_ = B;
    int A_ = 0;
    if (B == D && A == 0) {
     A = A_;
     D = D_;
     goto loc_f14;
    }
   }
  goto end;
 }
 loc_f14:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
