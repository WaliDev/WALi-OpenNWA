int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 1;
    int B_ = 999;
    if (1 >= 0) {
     B = B_;
     D = D_;
     goto loc_f19;
    }
   }
  goto end;
 }
 loc_f19:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = -1 + B;
    if (B >= 0) {
     B = B_;
     goto loc_f19;
    }
   }
   if (nondet_bool()) {
    int C_ = 999;
    if (0 >= 1 + B) {
     C = C_;
     goto loc_f28;
    }
   }
  goto end;
 }
 loc_f28:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    if (C >= 0) {
     C = C_;
     goto loc_f28;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + C) {
     goto loc_f36;
    }
   }
  goto end;
 }
 loc_f8:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (A >= 0) {
     A = A_;
     goto loc_f8;
    }
   }
   if (nondet_bool()) {
    int B_ = 999;
    if (0 >= 1 + A) {
     B = B_;
     goto loc_f19;
    }
   }
  goto end;
 }
 loc_f36:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
