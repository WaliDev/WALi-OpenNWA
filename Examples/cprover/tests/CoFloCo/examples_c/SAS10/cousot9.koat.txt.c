int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F) {
 goto loc_start0;

 loc_lbl52:
 {
 __cost++;
   if (nondet_bool()) {
    if (F == A && A >= D && D >= 1 && B >= 0 && 0 >= D) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int B_ = -1 + B;
    if (F == A && A >= D && B >= 0 && B >= 1 && D >= 1) {
     B = B_;
     goto loc_lbl52;
    }
   }
   if (nondet_bool()) {
    int D_ = -1 + D;
    int B_ = F;
    if (F == A && B == 0 && A >= D && D >= 1) {
     B = B_;
     D = D_;
     goto loc_lbl72;
    }
   }
  goto end;
 }
 loc_lbl72:
 {
 __cost++;
   if (nondet_bool()) {
    if (B == A && F == A && D == 0 && A >= 1) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int B_ = -1 + B;
    if (B == A && F == A && A >= 1 + D && D >= 0 && A >= 1 && D >= 1) {
     B = B_;
     goto loc_lbl52;
    }
   }
   if (nondet_bool()) {
    int D_ = -1 + D;
    int B_ = F;
    if (B == A && F == A && A >= 1 + D && D >= 0 && 0 >= A && D >= 1) {
     B = B_;
     D = D_;
     goto loc_lbl72;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = F;
    if (F == A && D == E && B == C && 0 >= A) {
     D = D_;
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int D_ = F;
    int B_ = -1 + B;
    if (F == A && D == E && B == C && C >= 1 && A >= 1) {
     B = B_;
     D = D_;
     goto loc_lbl52;
    }
   }
   if (nondet_bool()) {
    int D_ = -1 + F;
    int B_ = F;
    if (F == A && D == E && B == C && 0 >= C && A >= 1) {
     B = B_;
     D = D_;
     goto loc_lbl72;
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
