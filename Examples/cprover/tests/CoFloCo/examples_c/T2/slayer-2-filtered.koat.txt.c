int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_f3;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = nondet();
    if (A >= 10) {
     B = B_;
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int C_ = A;
    int A_ = 1 + A;
    if (9 >= A) {
     A = A_;
     C = C_;
     goto loc_f0;
    }
   }
  goto end;
 }
 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int A_ = 1;
    if (A == 0 && 9 >= Y_0) {
     A = A_;
     goto loc_f0;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 2;
    int A_ = 2;
    if (9 >= A) {
     A = A_;
     D = D_;
     goto loc_f0;
    }
   }
  goto end;
 }
 loc_f3:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     goto loc_f0;
    }
   }
  goto end;
 }
 loc_f4:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
