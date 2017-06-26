int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_f1;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = A + B;
    if (B >= 1 && A >= 1) {
     A = A_;
     goto loc_f0;
    }
   }
   if (nondet_bool()) {
    int A_ = A + B;
    if (0 >= 1 + B && A >= 1) {
     A = A_;
     goto loc_f0;
    }
   }
  goto end;
 }
 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f0;
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
