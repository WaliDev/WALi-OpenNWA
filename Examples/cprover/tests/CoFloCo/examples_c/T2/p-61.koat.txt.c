int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E) {
 goto loc_f1;

 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1000;
    if (1 >= 0) {
     A = A_;
     goto loc_f300;
    }
   }
  goto end;
 }
 loc_f300:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (9 >= B && A >= 101) {
     A = A_;
     goto loc_f300;
    }
   }
   if (nondet_bool()) {
    int E_ = 0;
    int D_ = 0;
    int C_ = 0;
    if (9 >= B && 100 >= A) {
     C = C_;
     D = D_;
     E = E_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int E_ = 0;
    int D_ = 0;
    int C_ = 0;
    if (B >= 10) {
     C = C_;
     D = D_;
     E = E_;
     goto loc_f2;
    }
   }
  goto end;
 }
 loc_f2:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet());
}
