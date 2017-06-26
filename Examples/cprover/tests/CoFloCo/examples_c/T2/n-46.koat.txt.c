int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_f1;

 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    if (C >= B && B >= A && A >= C) {
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    if (C >= B && B >= A && C >= 1 + A) {
     D = D_;
     goto loc_f300;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    if (B >= 1 + C && B >= A) {
     D = D_;
     goto loc_f300;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    if (A >= 1 + B) {
     D = D_;
     goto loc_f300;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int B_ = 1 + B;
    int A_ = 1 + A;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f2;
    }
   }
  goto end;
 }
 loc_f300:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
