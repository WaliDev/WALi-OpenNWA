int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A) {
 goto loc_start;

 loc_a:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int A_ = A * Y_0;
    if (A >= 2 && 3 * Y_0 >= 2 && 1 >= 2 * Y_0) {
     A = A_;
     goto loc_a;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1) {
     goto loc_a;
    }
   }
   if (nondet_bool()) {
    if (A >= 2) {
     goto loc_a;
    }
   }
   if (nondet_bool()) {
    if (A >= 4) {
     goto loc_a;
    }
   }
  goto end;
 }
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet());
}
