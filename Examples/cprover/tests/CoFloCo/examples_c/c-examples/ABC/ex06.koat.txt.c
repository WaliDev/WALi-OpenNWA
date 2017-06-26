int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_evalfstart;

 loc_evalfbb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (1 >= 0) {
     C = C_;
     goto loc_evalfbb2in;
    }
   }
  goto end;
 }
 loc_evalfbb2in:
 {
 __cost++;
   if (nondet_bool()) {
    if (B >= C) {
     goto loc_evalfbb1in;
    }
   }
   if (nondet_bool()) {
    if (C >= 1 + B) {
     goto loc_evalfbb3in;
    }
   }
  goto end;
 }
 loc_evalfbb3in:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (1 >= 0) {
     A = A_;
     goto loc_evalfbb4in;
    }
   }
  goto end;
 }
 loc_evalfbb4in:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = A;
    if (B >= A) {
     C = C_;
     goto loc_evalfbb2in;
    }
   }
   if (nondet_bool()) {
    if (A >= 1 + B) {
     goto loc_evalfreturnin;
    }
   }
  goto end;
 }
 loc_evalfentryin:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1;
    if (1 >= 0) {
     A = A_;
     goto loc_evalfbb4in;
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
  foo(nondet(), nondet(), nondet());
}
