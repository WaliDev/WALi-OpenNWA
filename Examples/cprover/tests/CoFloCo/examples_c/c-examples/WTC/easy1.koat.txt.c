int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_evaleasy1start;

 loc_evaleasy1bb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (1 >= 0) {
     A = A_;
     goto loc_evaleasy1bb3in;
    }
   }
  goto end;
 }
 loc_evaleasy1bb2in:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 2 + A;
    if (1 >= 0) {
     A = A_;
     goto loc_evaleasy1bb3in;
    }
   }
  goto end;
 }
 loc_evaleasy1bb3in:
 {
 __cost++;
   if (nondet_bool()) {
    if (39 >= A) {
     goto loc_evaleasy1bbin;
    }
   }
   if (nondet_bool()) {
    if (A >= 40) {
     goto loc_evaleasy1returnin;
    }
   }
  goto end;
 }
 loc_evaleasy1bbin:
 {
 __cost++;
   if (nondet_bool()) {
    if (B == 0) {
     goto loc_evaleasy1bb1in;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + B) {
     goto loc_evaleasy1bb2in;
    }
   }
   if (nondet_bool()) {
    if (B >= 1) {
     goto loc_evaleasy1bb2in;
    }
   }
  goto end;
 }
 loc_evaleasy1entryin:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     goto loc_evaleasy1bb3in;
    }
   }
  goto end;
 }
 loc_evaleasy1returnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evaleasy1stop;
    }
   }
  goto end;
 }
 loc_evaleasy1start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evaleasy1entryin;
    }
   }
  goto end;
 }
 loc_evaleasy1stop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
