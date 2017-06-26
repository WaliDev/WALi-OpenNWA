int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_f1;

 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1 && 0 >= 1 + A + B) {
     goto loc_f2;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = A + -B;
    if (A >= 0) {
     A = A_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + A) {
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
