int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     goto loc_f4;
    }
   }
  goto end;
 }
 loc_f4:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + A;
    if (B >= 1 + A) {
     C = C_;
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_f19;
    }
   }
  goto end;
 }
 loc_f7:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 0;
    int C_ = 1 + C;
    if (B >= 1 + C) {
     C = C_;
     D = D_;
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int B_ = -1 + B;
    if (0 >= 1 + D_ && B >= 1 + C) {
     B = B_;
     D = D_;
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int B_ = -1 + B;
    if (D_ >= 1 && B >= 1 + C) {
     B = B_;
     D = D_;
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (C >= B) {
     A = A_;
     goto loc_f4;
    }
   }
  goto end;
 }
 loc_f19:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
