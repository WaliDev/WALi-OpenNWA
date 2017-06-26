int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L) {
 goto loc_start0;

 loc_lbl111:
 {
 __cost++;
   if (nondet_bool()) {
    int J_ = F;
    if (B == C && D == E && L == A && H == 1 && F == A && C >= 1 + J && J >= 0 && E >= 1 && A >= 1 + J) {
     J = J_;
     goto loc_lbl121;
    }
   }
   if (nondet_bool()) {
    int F_ = 1 + F;
    if (B == C && D == E && L == A && H == 1 && C >= 1 + J && A >= F && J >= 0 && E >= 1 && F >= 1 + J && A >= 1 + F) {
     F = F_;
     goto loc_lbl111;
    }
   }
  goto end;
 }
 loc_lbl121:
 {
 __cost++;
   if (nondet_bool()) {
    int J_ = 1 + J;
    if (B == C && D == E && L == A && J == F && H == E && E >= 1 && F >= A && A >= 0 && A + C >= 1 + F) {
     J = J_;
     goto loc_lbl131;
    }
   }
   if (nondet_bool()) {
    int H_ = 1 + H;
    int F_ = J;
    if (B == C && D == E && L == A && J == F && H >= 1 && E >= H && A >= 0 && A + C >= 1 + F && F >= A && E >= 1 + H) {
     F = F_;
     H = H_;
     goto loc_lbl121;
    }
   }
   if (nondet_bool()) {
    int H_ = 1 + H;
    int F_ = 1 + J;
    if (B == C && D == E && L == A && J == F && H >= 1 && F >= A && E >= H && A >= 0 && A + C >= 1 + F && E >= 1 + H && A >= 1 + F) {
     F = F_;
     H = H_;
     goto loc_lbl111;
    }
   }
  goto end;
 }
 loc_lbl131:
 {
 __cost++;
   if (nondet_bool()) {
    if (B == C && D == E && L == A && H == E && J >= 1 && A + C >= J && C >= 1 && A >= 0 && E >= 0 && J >= C) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int J_ = 1 + J;
    int H_ = 0;
    if (B == C && E == 0 && L == A && D == 0 && H == 0 && J >= 1 && A + C >= J && C >= 1 && A >= 0 && C >= 1 + J) {
     H = H_;
     J = J_;
     goto loc_lbl131;
    }
   }
   if (nondet_bool()) {
    int H_ = 1;
    int F_ = J;
    if (B == C && D == E && L == A && H == E && J >= 1 && A + C >= J && C >= 1 && A >= 0 && E >= 0 && J >= A && C >= 1 + J && E >= 1) {
     F = F_;
     H = H_;
     goto loc_lbl121;
    }
   }
   if (nondet_bool()) {
    int H_ = 1;
    int F_ = 1 + J;
    if (B == C && D == E && L == A && H == E && J >= 1 && A + C >= J && C >= 1 && A >= 0 && E >= 0 && C >= 1 + J && E >= 1 && A >= 1 + J) {
     F = F_;
     H = H_;
     goto loc_lbl111;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (L == A && J == K && H == I && F == G && D == E && B == C && 0 >= 1 + A) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    if (L == A && J == K && H == I && F == G && D == E && B == C && 0 >= 1 + E) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    if (L == A && J == K && H == I && F == G && D == E && B == C && 0 >= 1 + C) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int J_ = 0;
    if (L == A && J == K && H == I && F == G && D == E && C == 0 && B == 0 && E >= 0 && A >= 0) {
     J = J_;
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int J_ = 1;
    int H_ = 0;
    if (L == A && J == K && H == I && F == G && E == 0 && B == C && D == 0 && C >= 1 && A >= 0) {
     H = H_;
     J = J_;
     goto loc_lbl131;
    }
   }
   if (nondet_bool()) {
    int J_ = 0;
    int H_ = 1;
    int F_ = 0;
    if (A == 0 && J == K && H == I && F == G && D == E && B == C && L == 0 && C >= 1 && E >= 1) {
     F = F_;
     H = H_;
     J = J_;
     goto loc_lbl121;
    }
   }
   if (nondet_bool()) {
    int J_ = 0;
    int H_ = 1;
    int F_ = 1;
    if (L == A && J == K && H == I && F == G && D == E && B == C && C >= 1 && E >= 1 && A >= 1) {
     F = F_;
     H = H_;
     J = J_;
     goto loc_lbl111;
    }
   }
  goto end;
 }
 loc_start0:
 {
 __cost++;
   if (nondet_bool()) {
    int L_ = A;
    int J_ = K;
    int H_ = I;
    int F_ = G;
    int D_ = E;
    int B_ = C;
    if (1 >= 0) {
     B = B_;
     D = D_;
     F = F_;
     H = H_;
     J = J_;
     L = L_;
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
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
