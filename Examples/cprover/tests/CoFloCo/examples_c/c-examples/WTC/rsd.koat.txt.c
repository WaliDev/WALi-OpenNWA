int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_evalrsdstart;

 loc_evalrsdbb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (0 >= 1 + Y_0) {
     goto loc_evalrsdbb2in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (Y_0 >= 1) {
     goto loc_evalrsdbb2in;
    }
   }
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalrsdbb3in;
    }
   }
  goto end;
 }
 loc_evalrsdbb2in:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    if (1 >= 0) {
     C = C_;
     goto loc_evalrsdbb4in;
    }
   }
  goto end;
 }
 loc_evalrsdbb3in:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + B;
    int B_ = -1 + B;
    if (1 >= 0) {
     B = B_;
     C = C_;
     goto loc_evalrsdbb4in;
    }
   }
  goto end;
 }
 loc_evalrsdbb4in:
 {
 __cost++;
   if (nondet_bool()) {
    if (C >= A) {
     goto loc_evalrsdbb1in;
    }
   }
   if (nondet_bool()) {
    if (A >= 1 + C) {
     goto loc_evalrsdreturnin;
    }
   }
  goto end;
 }
 loc_evalrsdbbin:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 2 * A;
    int B_ = 2 * A;
    if (1 >= 0) {
     B = B_;
     C = C_;
     goto loc_evalrsdbb4in;
    }
   }
  goto end;
 }
 loc_evalrsdentryin:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 0) {
     goto loc_evalrsdbbin;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + A) {
     goto loc_evalrsdreturnin;
    }
   }
  goto end;
 }
 loc_evalrsdreturnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalrsdstop;
    }
   }
  goto end;
 }
 loc_evalrsdstart:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalrsdentryin;
    }
   }
  goto end;
 }
 loc_evalrsdstop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
