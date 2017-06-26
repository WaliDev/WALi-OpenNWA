int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_evalaxstart;

 loc_evalaxbb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (1 >= 0) {
     B = B_;
     goto loc_evalaxbb2in;
    }
   }
  goto end;
 }
 loc_evalaxbb2in:
 {
 __cost++;
   if (nondet_bool()) {
    if (C >= 2 + B) {
     goto loc_evalaxbb1in;
    }
   }
   if (nondet_bool()) {
    if (1 + B >= C) {
     goto loc_evalaxbb3in;
    }
   }
  goto end;
 }
 loc_evalaxbb3in:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (C >= 3 + A && 1 + B >= C) {
     A = A_;
     goto loc_evalaxbbin;
    }
   }
   if (nondet_bool()) {
    if (C >= 2 + B) {
     goto loc_evalaxreturnin;
    }
   }
   if (nondet_bool()) {
    if (2 + A >= C) {
     goto loc_evalaxreturnin;
    }
   }
  goto end;
 }
 loc_evalaxbbin:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 0;
    if (1 >= 0) {
     B = B_;
     goto loc_evalaxbb2in;
    }
   }
  goto end;
 }
 loc_evalaxentryin:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     goto loc_evalaxbbin;
    }
   }
  goto end;
 }
 loc_evalaxreturnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalaxstop;
    }
   }
  goto end;
 }
 loc_evalaxstart:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalaxentryin;
    }
   }
  goto end;
 }
 loc_evalaxstop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
