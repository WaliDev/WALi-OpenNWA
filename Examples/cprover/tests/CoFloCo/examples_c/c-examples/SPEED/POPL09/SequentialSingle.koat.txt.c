int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_evalSequentialSinglestart;

 loc_evalSequentialSinglebb1in:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_evalSequentialSinglebb5in;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_evalSequentialSinglebb2in;
    }
   }
  goto end;
 }
 loc_evalSequentialSinglebb2in:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (0 >= 1 + Y_0) {
     goto loc_evalSequentialSinglebbin;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (Y_0 >= 1) {
     goto loc_evalSequentialSinglebbin;
    }
   }
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalSequentialSinglebb5in;
    }
   }
  goto end;
 }
 loc_evalSequentialSinglebb4in:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (1 >= 0) {
     A = A_;
     goto loc_evalSequentialSinglebb5in;
    }
   }
  goto end;
 }
 loc_evalSequentialSinglebb5in:
 {
 __cost++;
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_evalSequentialSinglebb4in;
    }
   }
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_evalSequentialSinglereturnin;
    }
   }
  goto end;
 }
 loc_evalSequentialSinglebbin:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (1 >= 0) {
     A = A_;
     goto loc_evalSequentialSinglebb1in;
    }
   }
  goto end;
 }
 loc_evalSequentialSingleentryin:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     goto loc_evalSequentialSinglebb1in;
    }
   }
  goto end;
 }
 loc_evalSequentialSinglereturnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalSequentialSinglestop;
    }
   }
  goto end;
 }
 loc_evalSequentialSinglestart:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalSequentialSingleentryin;
    }
   }
  goto end;
 }
 loc_evalSequentialSinglestop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
