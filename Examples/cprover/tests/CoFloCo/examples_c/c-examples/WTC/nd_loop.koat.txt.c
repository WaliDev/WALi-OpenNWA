int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A) {
 goto loc_evalndloopstart;

 loc_evalndloopbbin:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = nondet();
    if (2 + A >= A_ && A_ >= 1 + A && 9 >= A_) {
     A = A_;
     goto loc_evalndloopbbin;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (Y_0 >= 3 + A) {
     goto loc_evalndloopreturnin;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (A >= Y_0) {
     goto loc_evalndloopreturnin;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (Y_0 >= 10) {
     goto loc_evalndloopreturnin;
    }
   }
  goto end;
 }
 loc_evalndloopentryin:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     goto loc_evalndloopbbin;
    }
   }
  goto end;
 }
 loc_evalndloopreturnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalndloopstop;
    }
   }
  goto end;
 }
 loc_evalndloopstart:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalndloopentryin;
    }
   }
  goto end;
 }
 loc_evalndloopstop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet());
}
