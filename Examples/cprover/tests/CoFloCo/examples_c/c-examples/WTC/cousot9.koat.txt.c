int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_evalcousot9start;

 loc_evalcousot9bb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (1 >= 0) {
     A = A_;
     goto loc_evalcousot9bb3in;
    }
   }
  goto end;
 }
 loc_evalcousot9bb2in:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = -1 + B;
    int A_ = C;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_evalcousot9bb3in;
    }
   }
  goto end;
 }
 loc_evalcousot9bb3in:
 {
 __cost++;
   if (nondet_bool()) {
    if (B >= 1) {
     goto loc_evalcousot9bbin;
    }
   }
   if (nondet_bool()) {
    if (0 >= B) {
     goto loc_evalcousot9returnin;
    }
   }
  goto end;
 }
 loc_evalcousot9bbin:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1) {
     goto loc_evalcousot9bb1in;
    }
   }
   if (nondet_bool()) {
    if (0 >= A) {
     goto loc_evalcousot9bb2in;
    }
   }
  goto end;
 }
 loc_evalcousot9entryin:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = nondet();
    int B_ = C;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_evalcousot9bb3in;
    }
   }
  goto end;
 }
 loc_evalcousot9returnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalcousot9stop;
    }
   }
  goto end;
 }
 loc_evalcousot9start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalcousot9entryin;
    }
   }
  goto end;
 }
 loc_evalcousot9stop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
