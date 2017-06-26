int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_evalspeedFails4start;

 loc_evalspeedFails4bb3in:
 {
 __cost++;
   if (nondet_bool()) {
    if (C >= 1) {
     goto loc_evalspeedFails4bb4in;
    }
   }
   if (nondet_bool()) {
    if (0 >= C) {
     goto loc_evalspeedFails4bb5in;
    }
   }
  goto end;
 }
 loc_evalspeedFails4bb4in:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = D + A;
    if (1 >= 0) {
     D = D_;
     goto loc_evalspeedFails4bb6in;
    }
   }
  goto end;
 }
 loc_evalspeedFails4bb5in:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = D + -A;
    if (1 >= 0) {
     D = D_;
     goto loc_evalspeedFails4bb6in;
    }
   }
  goto end;
 }
 loc_evalspeedFails4bb6in:
 {
 __cost++;
   if (nondet_bool()) {
    if (B >= D) {
     goto loc_evalspeedFails4bb3in;
    }
   }
   if (nondet_bool()) {
    if (D >= 1 + B) {
     goto loc_evalspeedFails4returnin;
    }
   }
  goto end;
 }
 loc_evalspeedFails4entryin:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = B;
    int C_ = A;
    int B_ = C;
    int A_ = 1;
    if (A >= 1) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     goto loc_evalspeedFails4bb6in;
    }
   }
   if (nondet_bool()) {
    int D_ = B;
    int C_ = A;
    int B_ = C;
    int A_ = -1;
    if (0 >= A) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     goto loc_evalspeedFails4bb6in;
    }
   }
  goto end;
 }
 loc_evalspeedFails4returnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalspeedFails4stop;
    }
   }
  goto end;
 }
 loc_evalspeedFails4start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalspeedFails4entryin;
    }
   }
  goto end;
 }
 loc_evalspeedFails4stop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
