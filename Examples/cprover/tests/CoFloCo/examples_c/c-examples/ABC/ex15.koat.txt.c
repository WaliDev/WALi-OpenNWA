int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G) {
 goto loc_evalfstart;

 loc_evalfbb10in:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 1;
    if (D >= 1) {
     E = E_;
     goto loc_evalfbb8in;
    }
   }
   if (nondet_bool()) {
    if (0 >= D) {
     goto loc_evalfreturnin;
    }
   }
  goto end;
 }
 loc_evalfbb3in:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = 1 + G;
    if (1 >= 0) {
     G = G_;
     goto loc_evalfbb4in;
    }
   }
  goto end;
 }
 loc_evalfbb4in:
 {
 __cost++;
   if (nondet_bool()) {
    if (E >= G) {
     goto loc_evalfbb3in;
    }
   }
   if (nondet_bool()) {
    if (G >= 1 + E) {
     goto loc_evalfbb5in;
    }
   }
  goto end;
 }
 loc_evalfbb5in:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = 1 + F;
    if (1 >= 0) {
     F = F_;
     goto loc_evalfbb6in;
    }
   }
  goto end;
 }
 loc_evalfbb6in:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = C;
    if (B >= F) {
     G = G_;
     goto loc_evalfbb4in;
    }
   }
   if (nondet_bool()) {
    if (F >= 1 + B) {
     goto loc_evalfbb7in;
    }
   }
  goto end;
 }
 loc_evalfbb7in:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 1 + E;
    if (1 >= 0) {
     E = E_;
     goto loc_evalfbb8in;
    }
   }
  goto end;
 }
 loc_evalfbb8in:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = D;
    if (A >= E) {
     F = F_;
     goto loc_evalfbb6in;
    }
   }
   if (nondet_bool()) {
    if (E >= 1 + A) {
     goto loc_evalfbb9in;
    }
   }
  goto end;
 }
 loc_evalfbb9in:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = -1 + D;
    if (1 >= 0) {
     D = D_;
     goto loc_evalfbb10in;
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
     goto loc_evalfbb10in;
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
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
