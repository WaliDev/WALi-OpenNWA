int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     goto loc_f8;
    }
   }
  goto end;
 }
 loc_f8:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (3 >= A) {
     A = A_;
     goto loc_f8;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int C_ = 1 + A;
    int B_ = A;
    int A_ = 1 + A;
    if (3 >= A) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     goto loc_f8;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (0 >= 1 + Y_0 && A >= 4) {
     goto loc_f23;
    }
   }
   if (nondet_bool()) {
    if (A >= 4) {
     goto loc_f23;
    }
   }
  goto end;
 }
 loc_f23:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
