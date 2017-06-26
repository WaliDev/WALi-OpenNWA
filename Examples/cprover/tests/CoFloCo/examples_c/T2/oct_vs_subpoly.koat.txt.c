int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_f20;

 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    int A_ = 1 + A;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    if (A >= 1 + B) {
     goto loc_f30;
    }
   }
  goto end;
 }
 loc_f20:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 0;
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f30:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
