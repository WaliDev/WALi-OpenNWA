int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (0 >= 1 + Y_0) {
     goto loc_f8;
    }
   }
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f8;
    }
   }
   if (nondet_bool()) {
    int A_ = nondet();
    if (1 >= 0) {
     A = A_;
     goto loc_f8;
    }
   }
  goto end;
 }
 loc_f8:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet());
}
