int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_evalSimpleSinglestart;

 loc_evalSimpleSinglebb3in:
 {
 __cost++;
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_evalSimpleSinglebbin;
    }
   }
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_evalSimpleSinglereturnin;
    }
   }
  goto end;
 }
 loc_evalSimpleSinglebbin:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (1 >= 0) {
     A = A_;
     goto loc_evalSimpleSinglebb3in;
    }
   }
  goto end;
 }
 loc_evalSimpleSingleentryin:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     goto loc_evalSimpleSinglebb3in;
    }
   }
  goto end;
 }
 loc_evalSimpleSinglereturnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalSimpleSinglestop;
    }
   }
  goto end;
 }
 loc_evalSimpleSinglestart:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalSimpleSingleentryin;
    }
   }
  goto end;
 }
 loc_evalSimpleSinglestop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
