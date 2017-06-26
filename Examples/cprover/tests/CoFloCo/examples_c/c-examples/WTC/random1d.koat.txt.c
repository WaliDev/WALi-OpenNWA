int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_evalrandom1dstart;

 loc_evalrandom1dbb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int B_ = 1 + B;
    if (0 >= 1 + Y_0) {
     B = B_;
     goto loc_evalrandom1dbb5in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int B_ = 1 + B;
    if (Y_0 >= 1) {
     B = B_;
     goto loc_evalrandom1dbb5in;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (1 >= 0) {
     B = B_;
     goto loc_evalrandom1dbb5in;
    }
   }
  goto end;
 }
 loc_evalrandom1dbb5in:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_evalrandom1dbb1in;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_evalrandom1dreturnin;
    }
   }
  goto end;
 }
 loc_evalrandom1dentryin:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1;
    if (A >= 1) {
     B = B_;
     goto loc_evalrandom1dbb5in;
    }
   }
   if (nondet_bool()) {
    if (0 >= A) {
     goto loc_evalrandom1dreturnin;
    }
   }
  goto end;
 }
 loc_evalrandom1dreturnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalrandom1dstop;
    }
   }
  goto end;
 }
 loc_evalrandom1dstart:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalrandom1dentryin;
    }
   }
  goto end;
 }
 loc_evalrandom1dstop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
