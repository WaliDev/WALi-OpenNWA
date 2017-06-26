int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_evalrealselectstart;

 loc_evalrealselectbb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int C_ = 1 + C;
    if (Y_0 >= 1 + Y_1) {
     C = C_;
     goto loc_evalrealselectbb4in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int C_ = 1 + C;
    if (Y_0 >= Y_1) {
     C = C_;
     goto loc_evalrealselectbb4in;
    }
   }
  goto end;
 }
 loc_evalrealselectbb4in:
 {
 __cost++;
   if (nondet_bool()) {
    if (B >= 1 + C) {
     goto loc_evalrealselectbb1in;
    }
   }
   if (nondet_bool()) {
    if (C >= B) {
     goto loc_evalrealselectbb5in;
    }
   }
  goto end;
 }
 loc_evalrealselectbb5in:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (1 >= 0) {
     A = A_;
     goto loc_evalrealselectbb6in;
    }
   }
  goto end;
 }
 loc_evalrealselectbb6in:
 {
 __cost++;
   if (nondet_bool()) {
    if (B >= 2 + A) {
     goto loc_evalrealselectbbin;
    }
   }
   if (nondet_bool()) {
    if (1 + A >= B) {
     goto loc_evalrealselectreturnin;
    }
   }
  goto end;
 }
 loc_evalrealselectbbin:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + A;
    if (1 >= 0) {
     C = C_;
     goto loc_evalrealselectbb4in;
    }
   }
  goto end;
 }
 loc_evalrealselectentryin:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     goto loc_evalrealselectbb6in;
    }
   }
  goto end;
 }
 loc_evalrealselectreturnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalrealselectstop;
    }
   }
  goto end;
 }
 loc_evalrealselectstart:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalrealselectentryin;
    }
   }
  goto end;
 }
 loc_evalrealselectstop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
