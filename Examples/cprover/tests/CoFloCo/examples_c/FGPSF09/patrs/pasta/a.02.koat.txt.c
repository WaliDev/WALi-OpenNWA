int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_start;

 loc_eval:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int A_ = nondet();
    if (1 + Y_0 >= A_ && 3 * Y_0 >= 1 + A + B && A + B >= 2 * Y_0 && A_ >= 1 + Y_1 && 3 * Y_1 >= 1 + A + B && A + B >= 2 * Y_1 && B >= 1 + A && B >= 0 && A >= 0) {
     A = A_;
     goto loc_eval;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int B_ = nondet();
    if (Y_0 >= B_ && 3 * Y_0 >= 1 + A + B && A + B >= 2 * Y_0 && B_ >= Y_1 && 3 * Y_1 >= 1 + A + B && A + B >= 2 * Y_1 && B >= 1 + A && B >= 0 && A >= 0) {
     B = B_;
     goto loc_eval;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_eval;
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
