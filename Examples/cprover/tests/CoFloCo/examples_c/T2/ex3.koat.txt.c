int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 0;
    int B_ = 10;
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f8;
    }
   }
  goto end;
 }
 loc_f8:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    int A_ = 2 + A;
    if (B >= 1 + C) {
     A = A_;
     C = C_;
     goto loc_f8;
    }
   }
   if (nondet_bool()) {
    if (C >= B && 2 * B >= 1 + A) {
     goto loc_f6;
    }
   }
   if (nondet_bool()) {
    if (C >= B && A >= 2 * B) {
     goto loc_f6;
    }
   }
  goto end;
 }
 loc_f6:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
