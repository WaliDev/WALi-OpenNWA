int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_evalspeedpldi4start;

 loc_evalspeedpldi4bb2in:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1 + B) {
     goto loc_evalspeedpldi4bb3in;
    }
   }
   if (nondet_bool()) {
    if (B >= A) {
     goto loc_evalspeedpldi4bb4in;
    }
   }
  goto end;
 }
 loc_evalspeedpldi4bb3in:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = -1 + B;
    if (1 >= 0) {
     B = B_;
     goto loc_evalspeedpldi4bb5in;
    }
   }
  goto end;
 }
 loc_evalspeedpldi4bb4in:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = B + -A;
    if (1 >= 0) {
     B = B_;
     goto loc_evalspeedpldi4bb5in;
    }
   }
  goto end;
 }
 loc_evalspeedpldi4bb5in:
 {
 __cost++;
   if (nondet_bool()) {
    if (B >= 1) {
     goto loc_evalspeedpldi4bb2in;
    }
   }
   if (nondet_bool()) {
    if (0 >= B) {
     goto loc_evalspeedpldi4returnin;
    }
   }
  goto end;
 }
 loc_evalspeedpldi4entryin:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= A) {
     goto loc_evalspeedpldi4returnin;
    }
   }
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_evalspeedpldi4returnin;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + A && A >= 1) {
     goto loc_evalspeedpldi4bb5in;
    }
   }
  goto end;
 }
 loc_evalspeedpldi4returnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalspeedpldi4stop;
    }
   }
  goto end;
 }
 loc_evalspeedpldi4start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalspeedpldi4entryin;
    }
   }
  goto end;
 }
 loc_evalspeedpldi4stop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
