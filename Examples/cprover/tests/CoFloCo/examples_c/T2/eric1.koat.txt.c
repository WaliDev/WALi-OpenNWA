int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_f999;

 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (A >= 1) {
     A = A_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = -1 + B;
    int A_ = 1 + A;
    if (B >= 1) {
     A = A_;
     B = B_;
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    int A_ = -1 + A;
    if (A >= 1) {
     A = A_;
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = -1 + B;
    int A_ = 1 + A;
    if (B >= 1) {
     A = A_;
     B = B_;
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f999:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = -1 + B;
    int A_ = 1;
    if (A == 0 && B >= 1) {
     A = A_;
     B = B_;
     goto loc_f1;
    }
   }
  goto end;
 }
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
