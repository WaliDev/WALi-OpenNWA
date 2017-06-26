int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = nondet();
    int E_ = 8;
    int C_ = 0;
    int A_ = 8;
    if (B_ >= 1) {
     A = A_;
     B = B_;
     C = C_;
     E = E_;
     goto loc_f11;
    }
   }
  goto end;
 }
 loc_f11:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = nondet();
    int C_ = 1 + C;
    int B_ = -1 + B;
    int A_ = -1 + A;
    if (D_ >= 1 && A >= 1) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     goto loc_f11;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int A_ = -1 + A;
    if (A >= 1 + B && A >= 1 && 0 >= D_) {
     A = A_;
     D = D_;
     goto loc_f11;
    }
   }
   if (nondet_bool()) {
    if (0 >= A) {
     goto loc_f21;
    }
   }
  goto end;
 }
 loc_f21:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f21;
    }
   }
  goto end;
 }
 loc_f23:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f26;
    }
   }
  goto end;
 }
 loc_f26:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet());
}
