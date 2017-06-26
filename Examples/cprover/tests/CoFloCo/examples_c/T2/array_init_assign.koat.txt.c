int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     goto loc_f4;
    }
   }
  goto end;
 }
 loc_f10:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (1 >= B) {
     B = B_;
     goto loc_f10;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (0 >= 1 + Y_0 && B >= 2) {
     goto loc_f18;
    }
   }
   if (nondet_bool()) {
    if (B >= 2) {
     goto loc_f18;
    }
   }
  goto end;
 }
 loc_f4:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (1 >= A) {
     A = A_;
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int B_ = 0;
    if (A >= 2) {
     B = B_;
     goto loc_f10;
    }
   }
  goto end;
 }
 loc_f18:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
