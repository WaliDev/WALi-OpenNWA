int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_start;

 loc_group3:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = -3 + A;
    if (A >= 4) {
     A = A_;
     goto loc_group3;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_zip3;
    }
   }
  goto end;
 }
 loc_zip3:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int B_ = -1 + B;
    int A_ = -1 + A;
    if (C >= 1 && B >= 1 && A >= 1) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_zip3;
    }
   }
  goto end;
 }
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
