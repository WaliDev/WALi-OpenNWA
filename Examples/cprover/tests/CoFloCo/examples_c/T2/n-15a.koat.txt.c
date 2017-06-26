int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_f300;

 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = nondet();
    if (0 >= 1 + A_ && A == 14) {
     A = A_;
     B = B_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int A_ = nondet();
    if (A == 14 && A_ >= 0) {
     A = A_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = -1 + A;
    if (0 >= A && A >= 15) {
     A = A_;
     B = B_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int A_ = -1 + A;
    if (0 >= A && 13 >= A) {
     A = A_;
     B = B_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (A >= 1 && A >= 15) {
     A = A_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (A >= 1 && 13 >= A) {
     A = A_;
     goto loc_f2;
    }
   }
  goto end;
 }
 loc_f300:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f2;
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
