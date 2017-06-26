int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_f2;

 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f300;
    }
   }
  goto end;
 }
 loc_f300:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = nondet();
    if (B >= 1 + A && C_ >= 1) {
     C = C_;
     goto loc_f300;
    }
   }
   if (nondet_bool()) {
    int C_ = nondet();
    if (B >= 1 + A && 0 >= 1 + C_) {
     C = C_;
     goto loc_f300;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    int B_ = -1 + B;
    if (B >= 1 + A) {
     B = B_;
     C = C_;
     goto loc_f300;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    if (A >= B) {
     D = D_;
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
  foo(nondet(), nondet(), nondet(), nondet());
}
