int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     goto loc_f5;
    }
   }
  goto end;
 }
 loc_f5:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (1 >= A) {
     A = A_;
     goto loc_f5;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (0 >= 1 + Y_0 && A >= 2) {
     goto loc_f13;
    }
   }
   if (nondet_bool()) {
    if (A >= 2) {
     goto loc_f13;
    }
   }
  goto end;
 }
 loc_f13:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet());
}
