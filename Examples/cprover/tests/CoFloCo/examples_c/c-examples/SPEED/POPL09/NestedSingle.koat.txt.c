int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_evalNestedSinglestart;

 loc_evalNestedSinglebb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (1 >= 0) {
     C = C_;
     goto loc_evalNestedSinglebb2in;
    }
   }
  goto end;
 }
 loc_evalNestedSinglebb2in:
 {
 __cost++;
   if (nondet_bool()) {
    if (C >= B) {
     goto loc_evalNestedSinglebb4in;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + C) {
     goto loc_evalNestedSinglebb3in;
    }
   }
  goto end;
 }
 loc_evalNestedSinglebb3in:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (0 >= 1 + Y_0) {
     goto loc_evalNestedSinglebb1in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (Y_0 >= 1) {
     goto loc_evalNestedSinglebb1in;
    }
   }
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalNestedSinglebb4in;
    }
   }
  goto end;
 }
 loc_evalNestedSinglebb4in:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + C;
    if (1 >= 0) {
     A = A_;
     goto loc_evalNestedSinglebb5in;
    }
   }
  goto end;
 }
 loc_evalNestedSinglebb5in:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = A;
    if (B >= 1 + A) {
     C = C_;
     goto loc_evalNestedSinglebb2in;
    }
   }
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_evalNestedSinglereturnin;
    }
   }
  goto end;
 }
 loc_evalNestedSingleentryin:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     goto loc_evalNestedSinglebb5in;
    }
   }
  goto end;
 }
 loc_evalNestedSinglereturnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalNestedSinglestop;
    }
   }
  goto end;
 }
 loc_evalNestedSinglestart:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalNestedSingleentryin;
    }
   }
  goto end;
 }
 loc_evalNestedSinglestop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
