int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_evalperfectstart;

 loc_evalperfectbb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + A;
    int B_ = A;
    if (1 >= 0) {
     B = B_;
     C = C_;
     goto loc_evalperfectbb8in;
    }
   }
  goto end;
 }
 loc_evalperfectbb3in:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = D + -C;
    if (1 >= 0) {
     D = D_;
     goto loc_evalperfectbb4in;
    }
   }
  goto end;
 }
 loc_evalperfectbb4in:
 {
 __cost++;
   if (nondet_bool()) {
    if (D >= C) {
     goto loc_evalperfectbb3in;
    }
   }
   if (nondet_bool()) {
    if (C >= 1 + D) {
     goto loc_evalperfectbb5in;
    }
   }
  goto end;
 }
 loc_evalperfectbb5in:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int B_ = B + -C;
    if (D == 0) {
     B = B_;
     C = C_;
     goto loc_evalperfectbb8in;
    }
   }
   if (nondet_bool()) {
    int C_ = -1 + C;
    if (0 >= 1 + D) {
     C = C_;
     goto loc_evalperfectbb8in;
    }
   }
   if (nondet_bool()) {
    int C_ = -1 + C;
    if (D >= 1) {
     C = C_;
     goto loc_evalperfectbb8in;
    }
   }
  goto end;
 }
 loc_evalperfectbb8in:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = A;
    if (C >= 1) {
     D = D_;
     goto loc_evalperfectbb4in;
    }
   }
   if (nondet_bool()) {
    int A_ = B;
    if (0 >= C) {
     A = A_;
     goto loc_evalperfectbb9in;
    }
   }
  goto end;
 }
 loc_evalperfectbb9in:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + A) {
     goto loc_evalperfectreturnin;
    }
   }
   if (nondet_bool()) {
    if (A >= 1) {
     goto loc_evalperfectreturnin;
    }
   }
   if (nondet_bool()) {
    if (A == 0) {
     goto loc_evalperfectreturnin;
    }
   }
  goto end;
 }
 loc_evalperfectentryin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= A) {
     goto loc_evalperfectreturnin;
    }
   }
   if (nondet_bool()) {
    if (A >= 2) {
     goto loc_evalperfectbb1in;
    }
   }
  goto end;
 }
 loc_evalperfectreturnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalperfectstop;
    }
   }
  goto end;
 }
 loc_evalperfectstart:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalperfectentryin;
    }
   }
  goto end;
 }
 loc_evalperfectstop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
