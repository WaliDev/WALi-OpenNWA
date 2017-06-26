int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_evalSimpleMultipleDepstart;

 loc_evalSimpleMultipleDepbb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (1 >= 0) {
     A = A_;
     goto loc_evalSimpleMultipleDepbb3in;
    }
   }
  goto end;
 }
 loc_evalSimpleMultipleDepbb2in:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_evalSimpleMultipleDepbb3in;
    }
   }
  goto end;
 }
 loc_evalSimpleMultipleDepbb3in:
 {
 __cost++;
   if (nondet_bool()) {
    if (C >= 1 + B) {
     goto loc_evalSimpleMultipleDepbbin;
    }
   }
   if (nondet_bool()) {
    if (B >= C) {
     goto loc_evalSimpleMultipleDepreturnin;
    }
   }
  goto end;
 }
 loc_evalSimpleMultipleDepbbin:
 {
 __cost++;
   if (nondet_bool()) {
    if (D >= 1 + A) {
     goto loc_evalSimpleMultipleDepbb1in;
    }
   }
   if (nondet_bool()) {
    if (A >= D) {
     goto loc_evalSimpleMultipleDepbb2in;
    }
   }
  goto end;
 }
 loc_evalSimpleMultipleDepentryin:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 0;
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_evalSimpleMultipleDepbb3in;
    }
   }
  goto end;
 }
 loc_evalSimpleMultipleDepreturnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalSimpleMultipleDepstop;
    }
   }
  goto end;
 }
 loc_evalSimpleMultipleDepstart:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalSimpleMultipleDepentryin;
    }
   }
  goto end;
 }
 loc_evalSimpleMultipleDepstop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
