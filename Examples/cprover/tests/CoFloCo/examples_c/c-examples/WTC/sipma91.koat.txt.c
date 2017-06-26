int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_evalsipma91start;

 loc_evalsipma91bb11in:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 2) {
     goto loc_evalsipma91bb5in;
    }
   }
   if (nondet_bool()) {
    if (1 >= A) {
     goto loc_evalsipma91returnin;
    }
   }
  goto end;
 }
 loc_evalsipma91bb2in:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 11 + B;
    int A_ = 1 + A;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_evalsipma91bb3in;
    }
   }
  goto end;
 }
 loc_evalsipma91bb3in:
 {
 __cost++;
   if (nondet_bool()) {
    if (100 >= B) {
     goto loc_evalsipma91bb2in;
    }
   }
   if (nondet_bool()) {
    if (B >= 101) {
     goto loc_evalsipma91bb11in;
    }
   }
  goto end;
 }
 loc_evalsipma91bb5in:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = -1 + A;
    int C_ = -10 + B;
    if (110 >= B) {
     C = C_;
     D = D_;
     goto loc_evalsipma91bb8in;
    }
   }
   if (nondet_bool()) {
    int D_ = -1 + A;
    int C_ = -10 + B;
    if (1 >= A) {
     C = C_;
     D = D_;
     goto loc_evalsipma91bb8in;
    }
   }
   if (nondet_bool()) {
    int D_ = -1 + A;
    int C_ = -10 + B;
    if (A >= 3) {
     C = C_;
     D = D_;
     goto loc_evalsipma91bb8in;
    }
   }
   if (nondet_bool()) {
    int B_ = -10 + B;
    int A_ = -1 + A;
    if (A == 2 && B >= 111) {
     A = A_;
     B = B_;
     goto loc_evalsipma91bb11in;
    }
   }
  goto end;
 }
 loc_evalsipma91bb8in:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + C;
    int A_ = D;
    if (C >= 101) {
     A = A_;
     B = B_;
     goto loc_evalsipma91bb11in;
    }
   }
   if (nondet_bool()) {
    int B_ = 11 + C;
    int A_ = D;
    if (100 >= C && C >= 101) {
     A = A_;
     B = B_;
     goto loc_evalsipma91bb11in;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + C;
    int A_ = 1 + D;
    if (C >= 101 && 100 >= C) {
     A = A_;
     B = B_;
     goto loc_evalsipma91bb11in;
    }
   }
   if (nondet_bool()) {
    int B_ = 11 + C;
    int A_ = 1 + D;
    if (100 >= C) {
     A = A_;
     B = B_;
     goto loc_evalsipma91bb11in;
    }
   }
  goto end;
 }
 loc_evalsipma91entryin:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 101) {
     goto loc_evalsipma91returnin;
    }
   }
   if (nondet_bool()) {
    int B_ = A;
    int A_ = 1;
    if (100 >= A) {
     A = A_;
     B = B_;
     goto loc_evalsipma91bb3in;
    }
   }
  goto end;
 }
 loc_evalsipma91returnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalsipma91stop;
    }
   }
  goto end;
 }
 loc_evalsipma91start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalsipma91entryin;
    }
   }
  goto end;
 }
 loc_evalsipma91stop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
