int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_f2;

 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = -1 + A;
    if (A >= 0) {
     A = A_;
     B = B_;
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    if (0 >= 1 + A) {
     B = B_;
     goto loc_f3;
    }
   }
  goto end;
 }
 loc_f3:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
