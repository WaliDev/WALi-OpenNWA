int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_start;

 loc_eval:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_1 = nondet();
    int Y_0 = nondet();
    int B_ = B + Y_0;
    int A_ = A + -Y_1;
    if (Y_0 >= 1 && Y_1 >= 0 && A >= 1 + B) {
     A = A_;
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
