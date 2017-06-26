int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = A;
    if (A == B) {
     B = B_;
     goto loc_f1;
    }
   }
  goto end;
 }
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
    if (1 >= 0) {
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    if (A >= 1 + B) {
     goto loc_f10000;
    }
   }
  goto end;
 }
 loc_f2:
 loc_f10000:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
