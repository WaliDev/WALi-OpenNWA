int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A) {
 goto loc_f300;

 loc_f3:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (0 >= A) {
     A = A_;
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (A >= 1) {
     A = A_;
     goto loc_f3;
    }
   }
  goto end;
 }
 loc_f300:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f3;
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
