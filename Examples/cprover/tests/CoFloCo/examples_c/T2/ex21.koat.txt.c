int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E) {
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
 loc_f20:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = B;
    int B_ = 1 + B;
    if (199 >= B) {
     B = B_;
     C = C_;
     goto loc_f20;
    }
   }
   if (nondet_bool()) {
    if (B >= 200) {
     goto loc_f31;
    }
   }
  goto end;
 }
 loc_f4:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = A;
    int D_ = A;
    int A_ = 1 + A;
    if (99 >= A) {
     A = A_;
     D = D_;
     E = E_;
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int B_ = 100;
    if (A >= 100) {
     B = B_;
     goto loc_f20;
    }
   }
  goto end;
 }
 loc_f31:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet());
}
