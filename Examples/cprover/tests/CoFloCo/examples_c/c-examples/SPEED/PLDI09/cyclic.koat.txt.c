int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_evalcyclicstart;

 loc_evalcyclicbb3in:
 {
 __cost++;
   if (nondet_bool()) {
    if (C == A) {
     goto loc_evalcyclicreturnin;
    }
   }
   if (nondet_bool()) {
    if (A >= 1 + C) {
     goto loc_evalcyclicbb4in;
    }
   }
   if (nondet_bool()) {
    if (C >= 1 + A) {
     goto loc_evalcyclicbb4in;
    }
   }
  goto end;
 }
 loc_evalcyclicbb4in:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (0 >= 1 + Y_0) {
     goto loc_evalcyclicbbin;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (Y_0 >= 1) {
     goto loc_evalcyclicbbin;
    }
   }
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalcyclicreturnin;
    }
   }
  goto end;
 }
 loc_evalcyclicbbin:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (B >= C) {
     C = C_;
     goto loc_evalcyclicbb3in;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    if (C >= 1 + B) {
     C = C_;
     goto loc_evalcyclicbb3in;
    }
   }
  goto end;
 }
 loc_evalcyclicentryin:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + A;
    if (B >= 1 + A && A >= 0) {
     C = C_;
     goto loc_evalcyclicbb3in;
    }
   }
  goto end;
 }
 loc_evalcyclicreturnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalcyclicstop;
    }
   }
  goto end;
 }
 loc_evalcyclicstart:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalcyclicentryin;
    }
   }
  goto end;
 }
 loc_evalcyclicstop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
