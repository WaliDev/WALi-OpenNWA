int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F) {
 goto loc_evalfstart;

 loc_evalfbb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = D + -E;
    if (1 >= 0) {
     F = F_;
     goto loc_evalfbb3in;
    }
   }
  goto end;
 }
 loc_evalfbb2in:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = 1 + F;
    if (1 >= 0) {
     F = F_;
     goto loc_evalfbb3in;
    }
   }
  goto end;
 }
 loc_evalfbb3in:
 {
 __cost++;
   if (nondet_bool()) {
    if (D + E >= F) {
     goto loc_evalfbb2in;
    }
   }
   if (nondet_bool()) {
    if (F >= 1 + D + E) {
     goto loc_evalfbb4in;
    }
   }
  goto end;
 }
 loc_evalfbb4in:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 1 + E;
    if (1 >= 0) {
     E = E_;
     goto loc_evalfbb5in;
    }
   }
  goto end;
 }
 loc_evalfbb5in:
 {
 __cost++;
   if (nondet_bool()) {
    if (C >= E) {
     goto loc_evalfbb1in;
    }
   }
   if (nondet_bool()) {
    if (E >= 1 + C) {
     goto loc_evalfbb6in;
    }
   }
  goto end;
 }
 loc_evalfbb6in:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (1 >= 0) {
     D = D_;
     goto loc_evalfbb7in;
    }
   }
  goto end;
 }
 loc_evalfbb7in:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = B;
    if (A >= D) {
     E = E_;
     goto loc_evalfbb5in;
    }
   }
   if (nondet_bool()) {
    if (D >= 1 + A) {
     goto loc_evalfreturnin;
    }
   }
  goto end;
 }
 loc_evalfentryin:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = A;
    int C_ = D;
    int B_ = C;
    int A_ = B;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
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
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
