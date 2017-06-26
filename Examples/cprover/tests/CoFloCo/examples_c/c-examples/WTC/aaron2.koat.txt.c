int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_evalaaron2start;

 loc_evalaaron2bb3in:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (0 >= 1 + Y_0) {
     goto loc_evalaaron2bb4in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (Y_0 >= 1) {
     goto loc_evalaaron2bb4in;
    }
   }
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalaaron2bb5in;
    }
   }
  goto end;
 }
 loc_evalaaron2bb4in:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C + -A;
    if (1 >= 0) {
     C = C_;
     goto loc_evalaaron2bb6in;
    }
   }
  goto end;
 }
 loc_evalaaron2bb5in:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B + A;
    if (1 >= 0) {
     B = B_;
     goto loc_evalaaron2bb6in;
    }
   }
  goto end;
 }
 loc_evalaaron2bb6in:
 {
 __cost++;
   if (nondet_bool()) {
    if (B >= 1 + C) {
     goto loc_evalaaron2returnin;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + A) {
     goto loc_evalaaron2returnin;
    }
   }
   if (nondet_bool()) {
    if (A >= 0 && C >= B) {
     goto loc_evalaaron2bb3in;
    }
   }
  goto end;
 }
 loc_evalaaron2entryin:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = B;
    int B_ = C;
    if (A >= 0) {
     B = B_;
     C = C_;
     goto loc_evalaaron2bb6in;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + A) {
     goto loc_evalaaron2returnin;
    }
   }
  goto end;
 }
 loc_evalaaron2returnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalaaron2stop;
    }
   }
  goto end;
 }
 loc_evalaaron2start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalaaron2entryin;
    }
   }
  goto end;
 }
 loc_evalaaron2stop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
