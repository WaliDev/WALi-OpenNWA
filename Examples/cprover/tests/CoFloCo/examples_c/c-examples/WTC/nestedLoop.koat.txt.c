int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H) {
 goto loc_evalnestedLoopstart;

 loc_evalnestedLoopbb4in:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = F;
    int G_ = 1 + E;
    if (1 >= 0) {
     G = G_;
     H = H_;
     goto loc_evalnestedLoopbb6in;
    }
   }
  goto end;
 }
 loc_evalnestedLoopbb5in:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = 1 + H;
    if (1 >= 0) {
     H = H_;
     goto loc_evalnestedLoopbb6in;
    }
   }
  goto end;
 }
 loc_evalnestedLoopbb6in:
 {
 __cost++;
   if (nondet_bool()) {
    if (C >= 1 + H) {
     goto loc_evalnestedLoopbb5in;
    }
   }
   if (nondet_bool()) {
    int F_ = H;
    int E_ = G;
    if (H >= C) {
     E = E_;
     F = F_;
     goto loc_evalnestedLoopbb7in;
    }
   }
  goto end;
 }
 loc_evalnestedLoopbb7in:
 {
 __cost++;
   if (nondet_bool()) {
    if (B >= 1 + E) {
     goto loc_evalnestedLoopbb4in;
    }
   }
   if (nondet_bool()) {
    if (E >= B) {
     goto loc_evalnestedLoopbb8in;
    }
   }
  goto end;
 }
 loc_evalnestedLoopbb8in:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 1 + F;
    if (1 >= 0) {
     D = D_;
     goto loc_evalnestedLoopbb9in;
    }
   }
  goto end;
 }
 loc_evalnestedLoopbb9in:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = D;
    int E_ = 0;
    if (A >= 1 + D) {
     E = E_;
     F = F_;
     goto loc_evalnestedLoopbb7in;
    }
   }
   if (nondet_bool()) {
    if (D >= A) {
     goto loc_evalnestedLoopreturnin;
    }
   }
  goto end;
 }
 loc_evalnestedLoopentryin:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 0;
    if (C >= 0 && B >= 0 && A >= 0) {
     D = D_;
     goto loc_evalnestedLoopbb9in;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + A) {
     goto loc_evalnestedLoopreturnin;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + B) {
     goto loc_evalnestedLoopreturnin;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + C) {
     goto loc_evalnestedLoopreturnin;
    }
   }
  goto end;
 }
 loc_evalnestedLoopreturnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalnestedLoopstop;
    }
   }
  goto end;
 }
 loc_evalnestedLoopstart:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalnestedLoopentryin;
    }
   }
  goto end;
 }
 loc_evalnestedLoopstop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
