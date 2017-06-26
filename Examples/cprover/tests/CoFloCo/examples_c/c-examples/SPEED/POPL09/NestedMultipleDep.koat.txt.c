int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E) {
 goto loc_evalNestedMultipleDepstart;

 loc_evalNestedMultipleDepbb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (1 >= 0) {
     D = D_;
     goto loc_evalNestedMultipleDepbb2in;
    }
   }
  goto end;
 }
 loc_evalNestedMultipleDepbb2in:
 {
 __cost++;
   if (nondet_bool()) {
    if (E >= 1 + D) {
     goto loc_evalNestedMultipleDepbb1in;
    }
   }
   if (nondet_bool()) {
    int A_ = C;
    if (D >= E) {
     A = A_;
     goto loc_evalNestedMultipleDepbb3in;
    }
   }
  goto end;
 }
 loc_evalNestedMultipleDepbb3in:
 {
 __cost++;
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_evalNestedMultipleDepbbin;
    }
   }
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_evalNestedMultipleDepreturnin;
    }
   }
  goto end;
 }
 loc_evalNestedMultipleDepbbin:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 0;
    int C_ = 1 + A;
    if (1 >= 0) {
     C = C_;
     D = D_;
     goto loc_evalNestedMultipleDepbb2in;
    }
   }
  goto end;
 }
 loc_evalNestedMultipleDepentryin:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     goto loc_evalNestedMultipleDepbb3in;
    }
   }
  goto end;
 }
 loc_evalNestedMultipleDepreturnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalNestedMultipleDepstop;
    }
   }
  goto end;
 }
 loc_evalNestedMultipleDepstart:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalNestedMultipleDepentryin;
    }
   }
  goto end;
 }
 loc_evalNestedMultipleDepstop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet());
}
