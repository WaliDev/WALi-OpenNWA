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
     goto loc_f3;
    }
   }
  goto end;
 }
 loc_f3:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (41 >= A) {
     A = A_;
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int A_ = 1 + A;
    if (0 >= 1 + Y_0 && 41 >= A) {
     A = A_;
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    if (A >= 42) {
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
