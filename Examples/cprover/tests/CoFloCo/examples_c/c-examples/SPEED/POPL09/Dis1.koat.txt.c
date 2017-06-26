int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_evalDis1start;

 loc_evalDis1bb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (1 >= 0) {
     D = D_;
     goto loc_evalDis1bb3in;
    }
   }
  goto end;
 }
 loc_evalDis1bb2in:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (1 >= 0) {
     B = B_;
     goto loc_evalDis1bb3in;
    }
   }
  goto end;
 }
 loc_evalDis1bb3in:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1 + B) {
     goto loc_evalDis1bbin;
    }
   }
   if (nondet_bool()) {
    if (B >= A) {
     goto loc_evalDis1returnin;
    }
   }
  goto end;
 }
 loc_evalDis1bbin:
 {
 __cost++;
   if (nondet_bool()) {
    if (C >= 1 + D) {
     goto loc_evalDis1bb1in;
    }
   }
   if (nondet_bool()) {
    if (D >= C) {
     goto loc_evalDis1bb2in;
    }
   }
  goto end;
 }
 loc_evalDis1entryin:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = C;
    int C_ = D;
    int B_ = A;
    int A_ = B;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     goto loc_evalDis1bb3in;
    }
   }
  goto end;
 }
 loc_evalDis1returnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalDis1stop;
    }
   }
  goto end;
 }
 loc_evalDis1start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalDis1entryin;
    }
   }
  goto end;
 }
 loc_evalDis1stop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
