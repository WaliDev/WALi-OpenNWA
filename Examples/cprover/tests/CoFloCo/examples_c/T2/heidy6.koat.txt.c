int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 0;
    if (C >= 1) {
     A = A_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int A_ = 0;
    if (0 >= C) {
     A = A_;
     goto loc_f3;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (1 >= 0) {
     A = A_;
     goto loc_f2;
    }
   }
  goto end;
 }
 loc_f3:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = -1 + B;
    if (B >= 1) {
     B = B_;
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    int A_ = 0;
    if (0 >= B) {
     A = A_;
     goto loc_f5;
    }
   }
  goto end;
 }
 loc_f5:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1;
    if (1 >= 0) {
     C = C_;
     goto loc_f5;
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
