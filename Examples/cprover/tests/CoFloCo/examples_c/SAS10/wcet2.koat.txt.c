int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H) {
 goto loc_start0;

 loc_lbl82:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = 1 + H;
    int D_ = H;
    if (F == 1 + B && 4 >= H && H >= 3 && H >= A && B >= 0 && 9 >= B && 2 >= H) {
     D = D_;
     H = H_;
     goto loc_lbl92;
    }
   }
   if (nondet_bool()) {
    int H_ = 1 + H;
    int D_ = H;
    if (B == 9 && F == 10 && 4 >= H && H >= 3 && H >= A) {
     D = D_;
     H = H_;
     goto loc_lbl92;
    }
   }
   if (nondet_bool()) {
    int F_ = 1 + F;
    int B_ = F;
    if (F == 1 + B && 4 >= H && H >= A && B >= 0 && 9 >= B && 8 >= B && H >= 3) {
     B = B_;
     F = F_;
     goto loc_lbl82;
    }
   }
  goto end;
 }
 loc_lbl92:
 {
 __cost++;
   if (nondet_bool()) {
    if (H == 1 + D && F >= 0 && 10 + F >= 5 * D && D >= A && D >= 4) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int H_ = 1 + H;
    int F_ = 0;
    int D_ = H;
    if (H == 1 + D && F >= 0 && 10 + F >= 5 * D && D >= A && 1 >= D) {
     D = D_;
     F = F_;
     H = H_;
     goto loc_lbl92;
    }
   }
   if (nondet_bool()) {
    int F_ = 1;
    int B_ = 0;
    if (H == 1 + D && F >= 0 && 10 + F >= 5 * D && D >= A && 3 >= D && D >= 2) {
     B = B_;
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
    if (H == A && F == G && D == E && B == C && A >= 5) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int H_ = 1 + H;
    int F_ = 0;
    int D_ = H;
    if (H == A && F == G && D == E && B == C && 2 >= A) {
     D = D_;
     F = F_;
     H = H_;
     goto loc_lbl92;
    }
   }
   if (nondet_bool()) {
    int F_ = 1;
    int B_ = 0;
    if (H == A && F == G && D == E && B == C && 4 >= A && A >= 3) {
     B = B_;
     F = F_;
     goto loc_lbl82;
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
