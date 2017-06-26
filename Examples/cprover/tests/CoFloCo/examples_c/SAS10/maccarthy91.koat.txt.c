int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H) {
 goto loc_start0;

 loc_lbl111:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = 11 + F;
    int D_ = 1 + D;
    if (11 + F == 11 * D + A && B == C && H == A && 11 * D >= 22 && 122 >= 11 * D + A && 111 >= 11 * D + A) {
     D = D_;
     F = F_;
     goto loc_lbl111;
    }
   }
   if (nondet_bool()) {
    if (11 + F == 11 * D + A && B == C && H == A && 11 * D >= 22 && 122 >= 11 * D + A && 1 >= D && 11 * D + A >= 112) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int F_ = -10 + F;
    int D_ = -1 + D;
    int B_ = -20 + F;
    if (A == 100 && B == C && H == 100 && D == 2 && F == 111) {
     B = B_;
     D = D_;
     F = F_;
     goto loc_lbl161;
    }
   }
   if (nondet_bool()) {
    int F_ = 1 + F;
    if (11 + F == 11 * D + A && B == C && H == A && 11 * D >= 22 && 122 >= 11 * D + A && 121 >= 11 * D + A && D >= 3 && 11 * D + A >= 112) {
     F = F_;
     goto loc_lbl221;
    }
   }
   if (nondet_bool()) {
    int F_ = 1 + F;
    if (11 + F == 11 * D + A && B == C && H == A && 11 * D >= 22 && 122 >= 11 * D + A && 121 >= 11 * D + A && D >= 2 && 11 * D + A >= 112) {
     F = F_;
     goto loc_lbl221;
    }
   }
   if (nondet_bool()) {
    int F_ = -9 + F;
    int D_ = -1 + D;
    if (A + 11 * D == 122 && B == C && H + 11 * D == 122 && F == 111 && 11 * D >= 22 && D >= 3) {
     D = D_;
     F = F_;
     goto loc_lbl221;
    }
   }
  goto end;
 }
 loc_lbl161:
 {
 __cost++;
   if (nondet_bool()) {
    if (B == 91 && F == 101 && H == A && D == 1 && 89 >= A) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int F_ = -10 + F;
    int D_ = -1 + D;
    int B_ = -20 + F;
    if (B == 91 && F == 101 && H == A && D == 2 && 0 >= 1 && 89 >= A && 0 >= 10) {
     B = B_;
     D = D_;
     F = F_;
     goto loc_lbl161;
    }
   }
   if (nondet_bool()) {
    int F_ = 1 + F;
    if (B == 91 && D == 1 && F == 101 && H == A && 89 >= A && 0 >= 2) {
     F = F_;
     goto loc_lbl221;
    }
   }
   if (nondet_bool()) {
    int F_ = 1 + F;
    if (B == 91 && D == 1 && F == 101 && H == A && 89 >= A && 0 >= 1) {
     F = F_;
     goto loc_lbl221;
    }
   }
   if (nondet_bool()) {
    int F_ = -9 + F;
    int D_ = -1 + D;
    if (B == 91 && D == 1 && F == 101 && H == A && 89 >= A && 0 >= 2 && 0 >= 10) {
     D = D_;
     F = F_;
     goto loc_lbl221;
    }
   }
  goto end;
 }
 loc_lbl221:
 {
 __cost++;
   if (nondet_bool()) {
    if (B == C && H == A && 89 >= A && 10 + F >= A + 11 * D && 111 >= F && F >= 102 && D >= 2 && 1 >= D) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int F_ = -10 + F;
    int D_ = -1 + D;
    int B_ = -20 + F;
    if (B == C && H == A && D == 2 && F == 111 && 89 >= A && 99 >= A) {
     B = B_;
     D = D_;
     F = F_;
     goto loc_lbl161;
    }
   }
   if (nondet_bool()) {
    int F_ = 1 + F;
    if (B == C && H == A && 89 >= A && 10 + F >= A + 11 * D && 111 >= F && F >= 102 && D >= 2 && 110 >= F && D >= 3) {
     F = F_;
     goto loc_lbl221;
    }
   }
   if (nondet_bool()) {
    int F_ = 1 + F;
    if (B == C && H == A && 89 >= A && 10 + F >= A + 11 * D && 111 >= F && F >= 102 && 110 >= F && D >= 2) {
     F = F_;
     goto loc_lbl221;
    }
   }
   if (nondet_bool()) {
    int F_ = -9 + F;
    int D_ = -1 + D;
    if (B == C && H == A && F == 111 && 89 >= A && 121 >= A + 11 * D && D >= 2 && D >= 3) {
     D = D_;
     F = F_;
     goto loc_lbl221;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = H;
    int D_ = 1;
    int B_ = -10 + H;
    if (H == A && F == G && D == E && B == C && A >= 101) {
     B = B_;
     D = D_;
     F = F_;
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int F_ = 11 + H;
    int D_ = 2;
    if (H == A && F == G && D == E && B == C && 100 >= A) {
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
