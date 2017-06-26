int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_f1;

 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f2;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    int A_ = 1 + A;
    if (1 >= A) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    int A_ = 1 + A;
    if (A >= 2 && 2 >= B) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int C_ = nondet();
    if (A >= 2 && B >= 3) {
     C = C_;
     goto loc_f300;
    }
   }
  goto end;
 }
 loc_f300:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
