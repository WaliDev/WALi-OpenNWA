int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 200 + A;
    if (99 + A >= 0) {
     A = A_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int A_ = 500 + A;
    if (A >= 1) {
     A = A_;
     goto loc_f2;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 700 + A;
    if (199 + A >= 0) {
     A = A_;
     goto loc_f2;
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
