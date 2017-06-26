int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E) {
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
    if (D >= 1 + B) {
     goto loc_evalfbb5in;
    }
   }
   if (nondet_bool()) {
    if (B >= D) {
     goto loc_evalfbb4in;
    }
   }
  goto end;
 }
 loc_evalfbb4in:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (0 >= 1 + Y_0) {
     goto loc_evalfbb2in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (Y_0 >= 1) {
     goto loc_evalfbb2in;
    }
   }
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalfbb5in;
    }
   }
  goto end;
 }
 loc_evalfbb5in:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = D;
    int D_ = -1 + A;
    if (1 >= 0) {
     D = D_;
     E = E_;
     goto loc_evalfbb6in;
    }
   }
  goto end;
 }
 loc_evalfbb6in:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + E;
    int A_ = D;
    if (1 >= 0) {
     A = A_;
     C = C_;
     goto loc_evalfbb7in;
    }
   }
  goto end;
 }
 loc_evalfbb7in:
 {
 __cost++;
   if (nondet_bool()) {
    if (C >= 0 && A >= 0) {
     goto loc_evalfbbin;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + A) {
     goto loc_evalfreturnin;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + C) {
     goto loc_evalfreturnin;
    }
   }
  goto end;
 }
 loc_evalfbbin:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int D_ = C;
    if (0 >= 1 + Y_0) {
     D = D_;
     goto loc_evalfbb3in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int D_ = C;
    if (Y_0 >= 1) {
     D = D_;
     goto loc_evalfbb3in;
    }
   }
   if (nondet_bool()) {
    int E_ = C;
    int D_ = A;
    if (1 >= 0) {
     D = D_;
     E = E_;
     goto loc_evalfbb6in;
    }
   }
  goto end;
 }
 loc_evalfentryin:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 0;
    int A_ = B;
    if (1 >= 0) {
     A = A_;
     C = C_;
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
  foo(nondet(), nondet(), nondet(), nondet(), nondet());
}
