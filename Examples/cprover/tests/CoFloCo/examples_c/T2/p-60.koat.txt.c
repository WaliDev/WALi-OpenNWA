int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E) {
 goto loc_f2;

 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1;
    if (B >= 1) {
     A = A_;
     goto loc_f300;
    }
   }
   if (nondet_bool()) {
    int E_ = 0;
    int D_ = 0;
    int C_ = 0;
    int A_ = 0;
    if (0 >= B) {
     A = A_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f3;
    }
   }
  goto end;
 }
 loc_f300:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (B >= 1 && 100 >= A) {
     A = A_;
     goto loc_f300;
    }
   }
   if (nondet_bool()) {
    int E_ = 0;
    int D_ = 0;
    int C_ = 0;
    if (A >= 101) {
     C = C_;
     D = D_;
     E = E_;
     goto loc_f3;
    }
   }
  goto end;
 }
 loc_f3:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet());
}
