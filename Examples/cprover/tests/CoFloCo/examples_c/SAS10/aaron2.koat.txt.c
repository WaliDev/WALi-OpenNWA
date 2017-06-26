int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F) {
 goto loc_start0;

 loc_lbl101:
 {
 __cost++;
   if (nondet_bool()) {
    if (F == A && 1 + A + D >= B && B >= 1 + A + C && A >= 0 && E >= D && B >= 1 + D) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    if (F == A && 1 + A + D >= B && B >= 1 + A + C && A >= 0 && E >= D && 0 >= 1 + A && D >= B) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int D_ = -1 + D + -F;
    if (F == A && 1 + A + D >= B && B >= 1 + A + C && E >= D && D >= B && A >= 0) {
     D = D_;
     goto loc_lbl91;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + F + B;
    if (F == A && 1 + A + D >= B && B >= 1 + A + C && E >= D && D >= B && A >= 0) {
     B = B_;
     goto loc_lbl101;
    }
   }
  goto end;
 }
 loc_lbl91:
 {
 __cost++;
   if (nondet_bool()) {
    if (F == A && E >= 1 + A + D && 1 + A + D >= B && A >= 0 && B >= C && B >= 1 + D) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    if (F == A && E >= 1 + A + D && 1 + A + D >= B && A >= 0 && B >= C && 0 >= 1 + A && D >= B) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int D_ = -1 + D + -F;
    if (F == A && E >= 1 + A + D && 1 + A + D >= B && B >= C && D >= B && A >= 0) {
     D = D_;
     goto loc_lbl91;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + F + B;
    if (F == A && E >= 1 + A + D && 1 + A + D >= B && B >= C && D >= B && A >= 0) {
     B = B_;
     goto loc_lbl101;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (F == A && D == E && B == C && 0 >= 1 + A) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    if (F == A && D == E && B == C && C >= 1 + E && A >= 0) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int D_ = -1 + D + -F;
    if (F == A && D == E && B == C && E >= C && A >= 0) {
     D = D_;
     goto loc_lbl91;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + F + B;
    if (F == A && D == E && B == C && E >= C && A >= 0) {
     B = B_;
     goto loc_lbl101;
    }
   }
  goto end;
 }
 loc_start0:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = A;
    int D_ = E;
    int B_ = C;
    if (1 >= 0) {
     B = B_;
     D = D_;
     F = F_;
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
