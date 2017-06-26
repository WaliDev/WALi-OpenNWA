int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H) {
 goto loc_evalNestedLoopstart;

 loc_evalNestedLoopbb10in:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int F_ = D;
    int E_ = 0;
    if (0 >= 1 + Y_0) {
     E = E_;
     F = F_;
     goto loc_evalNestedLoopbb6in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int F_ = D;
    int E_ = 0;
    if (Y_0 >= 1) {
     E = E_;
     F = F_;
     goto loc_evalNestedLoopbb6in;
    }
   }
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalNestedLoopreturnin;
    }
   }
  goto end;
 }
 loc_evalNestedLoopbb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = F;
    int G_ = 1 + E;
    if (1 >= 0) {
     G = G_;
     H = H_;
     goto loc_evalNestedLoopbb3in;
    }
   }
  goto end;
 }
 loc_evalNestedLoopbb2in:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = 1 + H;
    if (1 >= 0) {
     H = H_;
     goto loc_evalNestedLoopbb3in;
    }
   }
  goto end;
 }
 loc_evalNestedLoopbb3in:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = H;
    int E_ = G;
    if (H >= C) {
     E = E_;
     F = F_;
     goto loc_evalNestedLoopbb6in;
    }
   }
   if (nondet_bool()) {
    if (C >= 1 + H) {
     goto loc_evalNestedLoopbb4in;
    }
   }
  goto end;
 }
 loc_evalNestedLoopbb4in:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (0 >= 1 + Y_0) {
     goto loc_evalNestedLoopbb2in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (Y_0 >= 1) {
     goto loc_evalNestedLoopbb2in;
    }
   }
   if (nondet_bool()) {
    int F_ = H;
    int E_ = G;
    if (1 >= 0) {
     E = E_;
     F = F_;
     goto loc_evalNestedLoopbb6in;
    }
   }
  goto end;
 }
 loc_evalNestedLoopbb6in:
 {
 __cost++;
   if (nondet_bool()) {
    if (E >= B) {
     goto loc_evalNestedLoopbb8in;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + E) {
     goto loc_evalNestedLoopbb7in;
    }
   }
  goto end;
 }
 loc_evalNestedLoopbb7in:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (0 >= 1 + Y_0) {
     goto loc_evalNestedLoopbb1in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (Y_0 >= 1) {
     goto loc_evalNestedLoopbb1in;
    }
   }
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalNestedLoopbb8in;
    }
   }
  goto end;
 }
 loc_evalNestedLoopbb8in:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 1 + F;
    if (1 >= 0) {
     D = D_;
     goto loc_evalNestedLoopbb9in;
    }
   }
  goto end;
 }
 loc_evalNestedLoopbb9in:
 {
 __cost++;
   if (nondet_bool()) {
    if (D >= A) {
     goto loc_evalNestedLoopreturnin;
    }
   }
   if (nondet_bool()) {
    if (A >= 1 + D) {
     goto loc_evalNestedLoopbb10in;
    }
   }
  goto end;
 }
 loc_evalNestedLoopentryin:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 0;
    if (C >= 0 && B >= 0 && A >= 0) {
     D = D_;
     goto loc_evalNestedLoopbb9in;
    }
   }
  goto end;
 }
 loc_evalNestedLoopreturnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalNestedLoopstop;
    }
   }
  goto end;
 }
 loc_evalNestedLoopstart:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalNestedLoopentryin;
    }
   }
  goto end;
 }
 loc_evalNestedLoopstop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
