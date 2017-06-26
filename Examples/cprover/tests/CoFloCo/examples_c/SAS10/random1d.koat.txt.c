int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F) {
 goto loc_start0;

 loc_lbl101:
 {
 __cost++;
   if (nondet_bool()) {
    if (D == A && B == 1 + A && A >= E && A >= 1 && E + A >= 0) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int E_ = 1 + E;
    int B_ = 1 + B;
    if (D == A && B >= 1 + E && B >= 2 && 1 + A >= B && E + B >= 1 && A >= B) {
     B = B_;
     E = E_;
     goto loc_lbl101;
    }
   }
   if (nondet_bool()) {
    int E_ = -1 + E;
    int B_ = 1 + B;
    if (D == A && B >= 1 + E && B >= 2 && 1 + A >= B && E + B >= 1 && A >= B) {
     B = B_;
     E = E_;
     goto loc_lbl101;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (E == F && D == A && B == C && 0 >= A) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int E_ = 1;
    int B_ = 2;
    if (E == F && D == A && B == C && A >= 1) {
     B = B_;
     E = E_;
     goto loc_lbl101;
    }
   }
   if (nondet_bool()) {
    int E_ = -1;
    int B_ = 2;
    if (E == F && D == A && B == C && A >= 1) {
     B = B_;
     E = E_;
     goto loc_lbl101;
    }
   }
  goto end;
 }
 loc_start0:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = F;
    int D_ = A;
    int B_ = C;
    if (1 >= 0) {
     B = B_;
     D = D_;
     E = E_;
     goto loc_start;
    }
   }
  goto end;
 }
 loc_stop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
