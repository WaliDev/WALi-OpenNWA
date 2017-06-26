int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_evalwisestart;

 loc_evalwisebb3in:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1 + B) {
     goto loc_evalwisebb4in;
    }
   }
   if (nondet_bool()) {
    if (B >= A) {
     goto loc_evalwisebb5in;
    }
   }
  goto end;
 }
 loc_evalwisebb4in:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (1 >= 0) {
     B = B_;
     goto loc_evalwisebb6in;
    }
   }
  goto end;
 }
 loc_evalwisebb5in:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (1 >= 0) {
     A = A_;
     goto loc_evalwisebb6in;
    }
   }
  goto end;
 }
 loc_evalwisebb6in:
 {
 __cost++;
   if (nondet_bool()) {
    if (B >= 3 + A) {
     goto loc_evalwisebb3in;
    }
   }
   if (nondet_bool()) {
    if (A >= 3 + B) {
     goto loc_evalwisebb3in;
    }
   }
   if (nondet_bool()) {
    if (2 + B >= A && 2 + A >= B) {
     goto loc_evalwisereturnin;
    }
   }
  goto end;
 }
 loc_evalwiseentryin:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + A) {
     goto loc_evalwisereturnin;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + B) {
     goto loc_evalwisereturnin;
    }
   }
   if (nondet_bool()) {
    int B_ = A;
    int A_ = B;
    if (B >= 0 && A >= 0) {
     A = A_;
     B = B_;
     goto loc_evalwisebb6in;
    }
   }
  goto end;
 }
 loc_evalwisereturnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalwisestop;
    }
   }
  goto end;
 }
 loc_evalwisestart:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalwiseentryin;
    }
   }
  goto end;
 }
 loc_evalwisestop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
