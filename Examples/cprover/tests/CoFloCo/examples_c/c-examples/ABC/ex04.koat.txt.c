int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_evalfstart;

 loc_evalfbb2in:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (1 >= 0) {
     D = D_;
     goto loc_evalfbb3in;
    }
   }
  goto end;
 }
 loc_evalfbb3in:
 {
 __cost++;
   if (nondet_bool()) {
    if (C >= 1 + D) {
     goto loc_evalfbb2in;
    }
   }
   if (nondet_bool()) {
    if (D >= C) {
     goto loc_evalfbb4in;
    }
   }
  goto end;
 }
 loc_evalfbb4in:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (1 >= 0) {
     C = C_;
     goto loc_evalfbb5in;
    }
   }
  goto end;
 }
 loc_evalfbb5in:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 0;
    if (B >= 1 + C) {
     D = D_;
     goto loc_evalfbb3in;
    }
   }
   if (nondet_bool()) {
    if (C >= B) {
     goto loc_evalfbb6in;
    }
   }
  goto end;
 }
 loc_evalfbb6in:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (1 >= 0) {
     A = A_;
     goto loc_evalfbb7in;
    }
   }
  goto end;
 }
 loc_evalfbb7in:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 0;
    if (0 >= 1 + A && 0 >= B*B*B && B*B*B >= 0) {
     C = C_;
     goto loc_evalfbb5in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int C_ = 0;
    if (Y_0 >= 1 + A && 1 + 2 * Y_0 >= B*B*B && B*B*B >= 2 * Y_0 && Y_0 >= 0 && B*B*B >= 1) {
     C = C_;
     goto loc_evalfbb5in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int C_ = 0;
    if (1 + B*B*B >= 2 * Y_0 && 2 * Y_0 >= B*B*B && Y_0 >= 1 + A && 0 >= Y_0 && 0 >= 1 + B*B*B) {
     C = C_;
     goto loc_evalfbb5in;
    }
   }
   if (nondet_bool()) {
    if (A >= 0 && 0 >= B*B*B && B*B*B >= 0) {
     goto loc_evalfreturnin;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (A >= Y_0 && 1 + 2 * Y_0 >= B*B*B && B*B*B >= 2 * Y_0 && Y_0 >= 0 && B*B*B >= 1) {
     goto loc_evalfreturnin;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (1 + B*B*B >= 2 * Y_0 && 2 * Y_0 >= B*B*B && A >= Y_0 && 0 >= Y_0 && 0 >= 1 + B*B*B) {
     goto loc_evalfreturnin;
    }
   }
  goto end;
 }
 loc_evalfentryin:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     goto loc_evalfbb7in;
    }
   }
  goto end;
 }
 loc_evalfreturnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalfstop;
    }
   }
  goto end;
 }
 loc_evalfstart:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalfentryin;
    }
   }
  goto end;
 }
 loc_evalfstop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
