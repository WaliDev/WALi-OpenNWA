int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = nondet();
    int D_ = nondet();
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     D = D_;
     E = E_;
     goto loc_f12;
    }
   }
  goto end;
 }
 loc_f12:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (9 >= A) {
     A = A_;
     goto loc_f12;
    }
   }
   if (nondet_bool()) {
    int C_ = nondet();
    int B_ = 0;
    if (A >= 10) {
     B = B_;
     C = C_;
     goto loc_f25;
    }
   }
  goto end;
 }
 loc_f25:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (9 >= B) {
     B = B_;
     goto loc_f25;
    }
   }
   if (nondet_bool()) {
    if (B >= 10) {
     goto loc_f36;
    }
   }
  goto end;
 }
 loc_f36:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet());
}
