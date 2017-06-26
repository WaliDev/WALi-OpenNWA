int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E) {
 goto loc_evalrealshellsortstart;

 loc_evalrealshellsortbb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = nondet();
    int E_ = C;
    if (1 >= 0) {
     D = D_;
     E = E_;
     goto loc_evalrealshellsortbb3in;
    }
   }
  goto end;
 }
 loc_evalrealshellsortbb2in:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = E + -B;
    if (1 >= 0) {
     E = E_;
     goto loc_evalrealshellsortbb3in;
    }
   }
  goto end;
 }
 loc_evalrealshellsortbb3in:
 {
 __cost++;
   if (nondet_bool()) {
    if (B >= 1 + E) {
     goto loc_evalrealshellsortbb5in;
    }
   }
   if (nondet_bool()) {
    if (E >= B) {
     goto loc_evalrealshellsortbb4in;
    }
   }
  goto end;
 }
 loc_evalrealshellsortbb4in:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (Y_0 >= 1 + D) {
     goto loc_evalrealshellsortbb2in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (D >= Y_0) {
     goto loc_evalrealshellsortbb5in;
    }
   }
  goto end;
 }
 loc_evalrealshellsortbb5in:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (1 >= 0) {
     C = C_;
     goto loc_evalrealshellsortbb6in;
    }
   }
  goto end;
 }
 loc_evalrealshellsortbb6in:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1 + C) {
     goto loc_evalrealshellsortbb1in;
    }
   }
   if (nondet_bool()) {
    if (C >= A) {
     goto loc_evalrealshellsortbb7in;
    }
   }
  goto end;
 }
 loc_evalrealshellsortbb7in:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 0;
    if (B == 0) {
     B = B_;
     goto loc_evalrealshellsortbb8in;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    if (1 + 2 * B_ >= B && B >= 2 * B_ && B_ >= 0 && B >= 1) {
     B = B_;
     goto loc_evalrealshellsortbb8in;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    if (1 + B >= 2 * B_ && 2 * B_ >= B && 0 >= B_ && 0 >= 1 + B) {
     B = B_;
     goto loc_evalrealshellsortbb8in;
    }
   }
  goto end;
 }
 loc_evalrealshellsortbb8in:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 0;
    if (B >= 1) {
     C = C_;
     goto loc_evalrealshellsortbb6in;
    }
   }
   if (nondet_bool()) {
    if (0 >= B) {
     goto loc_evalrealshellsortreturnin;
    }
   }
  goto end;
 }
 loc_evalrealshellsortentryin:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 0;
    if (A == 0) {
     B = B_;
     goto loc_evalrealshellsortbb8in;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    if (1 + 2 * B_ >= A && A >= 2 * B_ && B_ >= 0 && A >= 1) {
     B = B_;
     goto loc_evalrealshellsortbb8in;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    if (1 + A >= 2 * B_ && 2 * B_ >= A && 0 >= B_ && 0 >= 1 + A) {
     B = B_;
     goto loc_evalrealshellsortbb8in;
    }
   }
  goto end;
 }
 loc_evalrealshellsortreturnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalrealshellsortstop;
    }
   }
  goto end;
 }
 loc_evalrealshellsortstart:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalrealshellsortentryin;
    }
   }
  goto end;
 }
 loc_evalrealshellsortstop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet());
}
