int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = nondet();
    int D_ = 0;
    int B_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     D = D_;
     goto loc_f9;
    }
   }
  goto end;
 }
 loc_f14:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    if (C >= 1) {
     C = C_;
     goto loc_f14;
    }
   }
   if (nondet_bool()) {
    int A_ = nondet();
    int D_ = 0;
    if (0 >= C) {
     A = A_;
     D = D_;
     goto loc_f9;
    }
   }
  goto end;
 }
 loc_f22:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f22;
    }
   }
  goto end;
 }
 loc_f24:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f27;
    }
   }
  goto end;
 }
 loc_f9:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = nondet();
    int B_ = 0;
    if (0 >= A) {
     B = B_;
     C = C_;
     goto loc_f14;
    }
   }
   if (nondet_bool()) {
    if (A >= 1) {
     goto loc_f22;
    }
   }
  goto end;
 }
 loc_f27:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
