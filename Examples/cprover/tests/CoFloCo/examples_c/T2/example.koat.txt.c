int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_f1;

 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    if (B >= 1 && A >= 1) {
     goto loc_f2;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (B >= 1 && A >= 2) {
     A = A_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = -1 + B;
    if (B >= 2 && A >= 1) {
     B = B_;
     goto loc_f2;
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
