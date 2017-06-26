int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E) {
 goto loc_f30;

 loc_f30:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = nondet();
    int C_ = 2;
    int A_ = 2;
    if (1 >= 0) {
     A = A_;
     C = C_;
     D = D_;
     goto loc_f4;
    }
   }
  goto end;
 }
 loc_f4:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1;
    if (A >= 2) {
     B = B_;
     goto loc_f5;
    }
   }
   if (nondet_bool()) {
    int B_ = 0;
    if (1 >= A) {
     B = B_;
     goto loc_f5;
    }
   }
  goto end;
 }
 loc_f5:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = nondet();
    int A_ = -1 + A;
    if (D_ >= 1 && 0 >= D_) {
     A = A_;
     D = D_;
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int A_ = 1 + A;
    if (D_ >= 1) {
     A = A_;
     D = D_;
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int E_ = 0;
    if (0 >= B) {
     E = E_;
     goto loc_f3;
    }
   }
  goto end;
 }
 loc_f3:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet());
}
