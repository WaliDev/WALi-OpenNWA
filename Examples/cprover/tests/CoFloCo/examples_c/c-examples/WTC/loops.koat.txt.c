int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_evalloopsstart;

 loc_evalloopsbb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1;
    if (A >= 2) {
     B = B_;
     goto loc_evalloopsbb4in;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    if (1 >= A) {
     B = B_;
     goto loc_evalloopsbb5in;
    }
   }
  goto end;
 }
 loc_evalloopsbb3in:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 2 * B;
    if (1 >= 0) {
     B = B_;
     goto loc_evalloopsbb4in;
    }
   }
  goto end;
 }
 loc_evalloopsbb4in:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1 + B) {
     goto loc_evalloopsbb3in;
    }
   }
   if (nondet_bool()) {
    if (B >= A) {
     goto loc_evalloopsbb5in;
    }
   }
  goto end;
 }
 loc_evalloopsbb5in:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (1 >= 0) {
     A = A_;
     goto loc_evalloopsbb6in;
    }
   }
  goto end;
 }
 loc_evalloopsbb6in:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 0) {
     goto loc_evalloopsbb1in;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + A) {
     goto loc_evalloopsreturnin;
    }
   }
  goto end;
 }
 loc_evalloopsentryin:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 0) {
     goto loc_evalloopsbb6in;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + A) {
     goto loc_evalloopsreturnin;
    }
   }
  goto end;
 }
 loc_evalloopsreturnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalloopsstop;
    }
   }
  goto end;
 }
 loc_evalloopsstart:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalloopsentryin;
    }
   }
  goto end;
 }
 loc_evalloopsstop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
