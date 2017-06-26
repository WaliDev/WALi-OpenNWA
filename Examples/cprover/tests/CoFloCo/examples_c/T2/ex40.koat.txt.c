int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = nondet();
    int B_ = 0;
    if (1 >= 0) {
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
    int Y_0 = nondet();
    int B_ = 1 + B;
    if (0 >= 1 + Y_0) {
     B = B_;
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (1 >= 0) {
     B = B_;
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + A && A >= 0) {
     goto loc_f14;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + A) {
     goto loc_f14;
    }
   }
   if (nondet_bool()) {
    if (A >= B && A >= 0) {
     goto loc_f14;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (B >= 1 + A && A >= 0 && 0 >= 1 + Y_0) {
     goto loc_f14;
    }
   }
  goto end;
 }
 loc_f14:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
