int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F) {
 goto loc_start0;

 loc_lbl101:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 2 * B;
    if (F == A && A >= D && 2 * D >= 2 + B && B >= 2 && D >= 1 + B) {
     B = B_;
     goto loc_lbl101;
    }
   }
   if (nondet_bool()) {
    int D_ = -1 + D;
    if (F == A && A >= D && 2 * D >= 2 + B && B >= 2 && B >= D) {
     D = D_;
     goto loc_lbl121;
    }
   }
  goto end;
 }
 loc_lbl121:
 {
 __cost++;
   if (nondet_bool()) {
    if (F == A && 1 + D == 0 && B >= 1 && B >= 0 && A >= 0) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int D_ = -1 + D;
    int B_ = 1;
    if (F == A && 1 + D >= 0 && B >= 1 && B >= 1 + D && A >= 1 + D && 1 >= D && D >= 0) {
     B = B_;
     D = D_;
     goto loc_lbl121;
    }
   }
   if (nondet_bool()) {
    int B_ = 2;
    if (F == A && 1 + D >= 0 && B >= 1 && B >= 1 + D && A >= 1 + D && D >= 2) {
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
    int D_ = F;
    if (F == A && D == E && B == C && 0 >= 1 + A) {
     D = D_;
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int D_ = -1 + F;
    int B_ = 1;
    if (F == A && D == E && B == C && 1 >= A && A >= 0) {
     B = B_;
     D = D_;
     goto loc_lbl121;
    }
   }
   if (nondet_bool()) {
    int D_ = F;
    int B_ = 2;
    if (F == A && D == E && B == C && A >= 2) {
     B = B_;
     D = D_;
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
