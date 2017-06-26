int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_evalgcdstart;

 loc_evalgcdbb4in:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1 + B) {
     goto loc_evalgcdbb5in;
    }
   }
   if (nondet_bool()) {
    if (B >= A) {
     goto loc_evalgcdbb6in;
    }
   }
  goto end;
 }
 loc_evalgcdbb5in:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = A + -B;
    if (1 >= 0) {
     A = A_;
     goto loc_evalgcdbb7in;
    }
   }
  goto end;
 }
 loc_evalgcdbb6in:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = B + -A;
    if (1 >= 0) {
     B = B_;
     goto loc_evalgcdbb7in;
    }
   }
  goto end;
 }
 loc_evalgcdbb7in:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1 + B) {
     goto loc_evalgcdbb4in;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_evalgcdbb4in;
    }
   }
   if (nondet_bool()) {
    if (B == A) {
     goto loc_evalgcdreturnin;
    }
   }
  goto end;
 }
 loc_evalgcdentryin:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= A) {
     goto loc_evalgcdreturnin;
    }
   }
   if (nondet_bool()) {
    if (0 >= B) {
     goto loc_evalgcdreturnin;
    }
   }
   if (nondet_bool()) {
    int B_ = A;
    int A_ = B;
    if (B >= 1 && A >= 1) {
     A = A_;
     B = B_;
     goto loc_evalgcdbb7in;
    }
   }
  goto end;
 }
 loc_evalgcdreturnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalgcdstop;
    }
   }
  goto end;
 }
 loc_evalgcdstart:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalgcdentryin;
    }
   }
  goto end;
 }
 loc_evalgcdstop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
