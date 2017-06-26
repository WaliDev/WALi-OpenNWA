int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_f300;

 loc_f300:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (A >= 1) {
     A = A_;
     goto loc_f5;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    if (0 >= A) {
     B = B_;
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f5:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (A >= 1) {
     A = A_;
     goto loc_f5;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    if (0 >= A) {
     B = B_;
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f1:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
