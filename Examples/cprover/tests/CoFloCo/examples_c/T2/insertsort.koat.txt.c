int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 2;
    if (1 >= 0) {
     A = A_;
     goto loc_f15;
    }
   }
  goto end;
 }
 loc_f15:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = A;
    if (10 >= A) {
     B = B_;
     goto loc_f18;
    }
   }
   if (nondet_bool()) {
    if (A >= 11) {
     goto loc_f28;
    }
   }
  goto end;
 }
 loc_f18:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int C_ = nondet();
    int B_ = -1 + B;
    if (Y_0 >= 1 + Y_1) {
     B = B_;
     C = C_;
     goto loc_f18;
    }
   }
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (1 >= 0) {
     A = A_;
     goto loc_f15;
    }
   }
  goto end;
 }
 loc_f28:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
