int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H) {
 goto loc_start0;

 loc_lbl111:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = D + -F;
    if (H == A && D >= 0 && F >= 1 && A >= B && A >= F + D && A >= 1 + F && D >= F) {
     D = D_;
     goto loc_lbl111;
    }
   }
   if (nondet_bool()) {
    int F_ = -1 + F;
    int D_ = H;
    if (H == A && D >= 0 && F >= 1 && A >= B && A >= F + D && A >= 1 + F && 0 >= 1 + D && F >= 1 + D) {
     D = D_;
     F = F_;
     goto loc_lbl82;
    }
   }
   if (nondet_bool()) {
    int F_ = -1 + F;
    int D_ = H;
    if (H == A && D >= 0 && F >= 1 && A >= B && A >= F + D && A >= 1 + F && D >= 1 && F >= 1 + D) {
     D = D_;
     F = F_;
     goto loc_lbl82;
    }
   }
   if (nondet_bool()) {
    int F_ = -1 + F;
    int D_ = H;
    int B_ = B + -F;
    if (H == A && D == 0 && A >= B && A >= F && A >= 1 + F && F >= 1) {
     B = B_;
     D = D_;
     F = F_;
     goto loc_lbl82;
    }
   }
  goto end;
 }
 loc_lbl16:
 {
 __cost++;
   if (nondet_bool()) {
    if (D == A && H == A && F == 0 && A >= 1 + B && A >= 2) {
     goto loc_stop;
    }
   }
  goto end;
 }
 loc_lbl82:
 {
 __cost++;
   if (nondet_bool()) {
    if (D == A && H == A && F == 0 && A >= 1 + B && A >= B && A >= 2) {
     goto loc_lbl16;
    }
   }
   if (nondet_bool()) {
    int D_ = D + -F;
    if (D == A && H == A && A + F >= 1 + B && A >= B && A >= 2 + F && F >= 0 && A >= F && F >= 1) {
     D = D_;
     goto loc_lbl111;
    }
   }
   if (nondet_bool()) {
    int F_ = -1 + F;
    int D_ = H;
    if (D == A && H == A && A + F >= 1 + B && A >= B && A >= 2 + F && F >= 0 && A >= 1 && F >= 1 + A) {
     D = D_;
     F = F_;
     goto loc_lbl82;
    }
   }
   if (nondet_bool()) {
    int F_ = -1 + F;
    int D_ = H;
    if (D == A && H == A && A + F >= 1 + B && A >= B && A >= 2 + F && F >= 0 && 0 >= 1 + A && F >= 1) {
     D = D_;
     F = F_;
     goto loc_lbl82;
    }
   }
   if (nondet_bool()) {
    int F_ = -1 + F;
    int D_ = H;
    int B_ = B + -F;
    if (A == 0 && H == 0 && D == 0 && F >= 1 + B && 0 >= B && 0 >= 2 + F && F >= 0 && F >= 1) {
     B = B_;
     D = D_;
     F = F_;
     goto loc_lbl82;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (H == A && F == G && D == E && B == C && 1 >= A) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int F_ = -1 + H;
    int D_ = 1;
    int B_ = H;
    if (H == A && F == G && D == E && B == C && A >= 2) {
     B = B_;
     D = D_;
     F = F_;
     goto loc_lbl111;
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
