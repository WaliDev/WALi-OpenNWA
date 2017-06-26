int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H) {
 goto loc_start0;

 loc_lM1:
 {
 __cost++;
   if (nondet_bool()) {
    if (F == G && H == A && D == 0 && B >= 1 && G >= B && A >= B) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int B_ = 0;
    if (F == G && H == A && B == A && D >= 0 && A >= 1 && G >= D + A && D >= 1) {
     B = B_;
     goto loc_lZZ1;
    }
   }
   if (nondet_bool()) {
    int D_ = -1 + D;
    int B_ = 1 + B;
    if (F == G && H == A && D >= 0 && B >= 1 && G >= D + B && A >= B && D >= 1 && A >= 1 + B) {
     B = B_;
     D = D_;
     goto loc_lM1;
    }
   }
  goto end;
 }
 loc_lZZ1:
 {
 __cost++;
   if (nondet_bool()) {
    if (F == G && H == A && B == 0 && D >= 1 && A >= 2 && G >= A + D && 0 >= D) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int B_ = 0;
    if (F == G && H == A && B == 0 && A >= 2 && G >= A + D && 0 >= A && D >= 1) {
     B = B_;
     goto loc_lZZ1;
    }
   }
   if (nondet_bool()) {
    int D_ = -1 + D;
    int B_ = 1 + B;
    if (F == G && H == A && B == 0 && A >= 2 && G >= A + D && D >= 1 && A >= 1) {
     B = B_;
     D = D_;
     goto loc_lM1;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (H == A && F == G && D == E && B == C && 0 >= A) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    if (H == A && F == G && D == E && B == C && 0 >= 1 + G) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int D_ = F;
    int B_ = 0;
    if (H == A && G == 0 && D == E && B == C && F == 0 && A >= 1) {
     B = B_;
     D = D_;
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int D_ = -1 + F;
    int B_ = 1;
    if (H == A && F == G && D == E && B == C && G >= 1 && A >= 1) {
     B = B_;
     D = D_;
     goto loc_lM1;
    }
   }
  goto end;
 }
 loc_start0:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = A;
    int F_ = G;
    int D_ = E;
    int B_ = C;
    if (1 >= 0) {
     B = B_;
     D = D_;
     F = F_;
     H = H_;
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
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
