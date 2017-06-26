int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_evalinsertsortstart;

 loc_evalinsertsortbb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = -1 + D;
    if (1 >= 0) {
     D = D_;
     goto loc_evalinsertsortbb2in;
    }
   }
  goto end;
 }
 loc_evalinsertsortbb2in:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + D) {
     goto loc_evalinsertsortbb4in;
    }
   }
   if (nondet_bool()) {
    if (D >= 0) {
     goto loc_evalinsertsortbb3in;
    }
   }
  goto end;
 }
 loc_evalinsertsortbb3in:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (Y_0 >= 1 + C) {
     goto loc_evalinsertsortbb1in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (C >= Y_0) {
     goto loc_evalinsertsortbb4in;
    }
   }
  goto end;
 }
 loc_evalinsertsortbb4in:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (1 >= 0) {
     A = A_;
     goto loc_evalinsertsortbb5in;
    }
   }
  goto end;
 }
 loc_evalinsertsortbb5in:
 {
 __cost++;
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_evalinsertsortbbin;
    }
   }
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_evalinsertsortreturnin;
    }
   }
  goto end;
 }
 loc_evalinsertsortbbin:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = nondet();
    int D_ = -1 + A;
    if (1 >= 0) {
     C = C_;
     D = D_;
     goto loc_evalinsertsortbb2in;
    }
   }
  goto end;
 }
 loc_evalinsertsortentryin:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1;
    if (1 >= 0) {
     A = A_;
     goto loc_evalinsertsortbb5in;
    }
   }
  goto end;
 }
 loc_evalinsertsortreturnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalinsertsortstop;
    }
   }
  goto end;
 }
 loc_evalinsertsortstart:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalinsertsortentryin;
    }
   }
  goto end;
 }
 loc_evalinsertsortstop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
