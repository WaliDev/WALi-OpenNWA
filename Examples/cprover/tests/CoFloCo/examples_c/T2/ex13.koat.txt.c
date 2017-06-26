int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = nondet();
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_f5;
    }
   }
  goto end;
 }
 loc_f5:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
