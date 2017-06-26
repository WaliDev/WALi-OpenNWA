int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G) {
 goto loc_evalfstart;

 loc_evalfbb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = E;
    int B_ = 1 + F;
    if (F >= G) {
     B = B_;
     C = C_;
     goto loc_evalfbb5in;
    }
   }
   if (nondet_bool()) {
    int F_ = 1 + F;
    if (G >= 1 + F) {
     F = F_;
     goto loc_evalfbb1in;
    }
   }
  goto end;
 }
 loc_evalfbb3in:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = E;
    int A_ = 1 + F;
    if (F >= G) {
     A = A_;
     C = C_;
     goto loc_evalfbb5in;
    }
   }
   if (nondet_bool()) {
    int F_ = 1 + F;
    if (G >= 1 + F) {
     F = F_;
     goto loc_evalfbb3in;
    }
   }
  goto end;
 }
 loc_evalfbb5in:
 {
 __cost++;
   if (nondet_bool()) {
    if (C >= D) {
     goto loc_evalfreturnin;
    }
   }
   if (nondet_bool()) {
    int E_ = 1 + C;
    if (D >= 1 + C) {
     E = E_;
     goto loc_evalfbbin;
    }
   }
  goto end;
 }
 loc_evalfbbin:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int F_ = B;
    if (0 >= 1 + Y_0) {
     F = F_;
     goto loc_evalfbb1in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int F_ = B;
    if (Y_0 >= 1) {
     F = F_;
     goto loc_evalfbb1in;
    }
   }
   if (nondet_bool()) {
    int F_ = A;
    if (1 >= 0) {
     F = F_;
     goto loc_evalfbb3in;
    }
   }
  goto end;
 }
 loc_evalfentryin:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 0;
    int B_ = 0;
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_evalfbb5in;
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
