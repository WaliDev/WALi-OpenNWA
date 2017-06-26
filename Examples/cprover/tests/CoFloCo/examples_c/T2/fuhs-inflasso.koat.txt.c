int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 0;
    int A_ = 0;
    if (C >= 1) {
     A = A_;
     B = B_;
     goto loc_f4;
    }
   }
  goto end;
 }
 loc_f4:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (A >= 1 + B) {
     B = B_;
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int B_ = 0;
    int A_ = 1 + A;
    if (B >= A && C >= 2 + A) {
     A = A_;
     B = B_;
     goto loc_f4;
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
