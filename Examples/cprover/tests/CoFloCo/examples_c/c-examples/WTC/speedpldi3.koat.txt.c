int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_evalspeedpldi3start;

 loc_evalspeedpldi3bb2in:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1 + C) {
     goto loc_evalspeedpldi3bb3in;
    }
   }
   if (nondet_bool()) {
    if (C >= A) {
     goto loc_evalspeedpldi3bb4in;
    }
   }
  goto end;
 }
 loc_evalspeedpldi3bb3in:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (1 >= 0) {
     C = C_;
     goto loc_evalspeedpldi3bb5in;
    }
   }
  goto end;
 }
 loc_evalspeedpldi3bb4in:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 1 + D;
    int C_ = 0;
    if (1 >= 0) {
     C = C_;
     D = D_;
     goto loc_evalspeedpldi3bb5in;
    }
   }
  goto end;
 }
 loc_evalspeedpldi3bb5in:
 {
 __cost++;
   if (nondet_bool()) {
    if (B >= 1 + D) {
     goto loc_evalspeedpldi3bb2in;
    }
   }
   if (nondet_bool()) {
    if (D >= B) {
     goto loc_evalspeedpldi3returnin;
    }
   }
  goto end;
 }
 loc_evalspeedpldi3entryin:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= A) {
     goto loc_evalspeedpldi3returnin;
    }
   }
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_evalspeedpldi3returnin;
    }
   }
   if (nondet_bool()) {
    int D_ = 0;
    int C_ = 0;
    if (B >= 1 + A && A >= 1) {
     C = C_;
     D = D_;
     goto loc_evalspeedpldi3bb5in;
    }
   }
  goto end;
 }
 loc_evalspeedpldi3returnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalspeedpldi3stop;
    }
   }
  goto end;
 }
 loc_evalspeedpldi3start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalspeedpldi3entryin;
    }
   }
  goto end;
 }
 loc_evalspeedpldi3stop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
