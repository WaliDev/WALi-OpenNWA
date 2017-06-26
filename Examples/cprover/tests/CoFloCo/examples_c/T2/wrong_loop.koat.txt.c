int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
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
    int Y_0 = nondet();
    int A_ = 1 + Y_0;
    if (A == 5) {
     A = A_;
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    int B_ = A;
    int A_ = 1 + A;
    if (4 >= A && 9 >= A) {
     A = A_;
     B = B_;
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    int B_ = A;
    int A_ = 1 + A;
    if (A >= 6 && 9 >= A) {
     A = A_;
     B = B_;
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    if (A >= 10) {
     goto loc_f12;
    }
   }
  goto end;
 }
 loc_f12:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
