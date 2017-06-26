int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_evalfstart;

 loc_evalfbb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (1 >= 0) {
     A = A_;
     goto loc_evalfbb3in;
    }
   }
  goto end;
 }
 loc_evalfbb2in:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = A + -B;
    if (1 >= 0) {
     A = A_;
     goto loc_evalfbb3in;
    }
   }
  goto end;
 }
 loc_evalfbb3in:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= A) {
     goto loc_evalfreturnin;
    }
   }
   if (nondet_bool()) {
    if (A >= 1) {
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
     goto loc_evalfbbin;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (Y_0 >= 1) {
     goto loc_evalfbbin;
    }
   }
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalfreturnin;
    }
   }
  goto end;
 }
 loc_evalfbbin:
 {
 __cost++;
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_evalfbb1in;
    }
   }
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_evalfbb2in;
    }
   }
  goto end;
 }
 loc_evalfentryin:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = A;
    int A_ = B;
    if (B >= 1 + A && A >= 1) {
     A = A_;
     B = B_;
     goto loc_evalfbb3in;
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
  foo(nondet(), nondet());
}
