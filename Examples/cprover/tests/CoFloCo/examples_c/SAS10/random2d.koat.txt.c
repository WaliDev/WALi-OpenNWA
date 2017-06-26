int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J) {
 goto loc_start0;

 loc_lbl121:
 {
 __cost++;
   if (nondet_bool()) {
    if (J == A && F == 0 && H == A && A >= D + B && A + B >= D && A + D >= 2 + B && A + D + B >= 2) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int H_ = 1 + H;
    if (J == A && F == 0 && H >= D + B && H + B >= D && H + D >= 2 + B && H + D + B >= 2 && A >= H && A >= 1 + H) {
     F = F_;
     H = H_;
     goto loc_lbl81;
    }
   }
  goto end;
 }
 loc_lbl141:
 {
 __cost++;
   if (nondet_bool()) {
    if (J == A && F == 1 && H == A && A >= 2 + D + B && A + B >= 2 + D && A + D >= B && A + D + B >= 0) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int H_ = 1 + H;
    if (J == A && F == 1 && H >= 2 + D + B && H + B >= 2 + D && H + D >= B && H + D + B >= 0 && A >= H && A >= 1 + H) {
     F = F_;
     H = H_;
     goto loc_lbl81;
    }
   }
  goto end;
 }
 loc_lbl171:
 {
 __cost++;
   if (nondet_bool()) {
    if (J == A && F == 2 && H == A && A >= D + B && A + B >= 2 + D && A + D >= B && A + D + B >= 2) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int H_ = 1 + H;
    if (J == A && F == 2 && H >= D + B && H + B >= 2 + D && H + D >= B && H + D + B >= 2 && A >= H && A >= 1 + H) {
     F = F_;
     H = H_;
     goto loc_lbl81;
    }
   }
  goto end;
 }
 loc_lbl191:
 {
 __cost++;
   if (nondet_bool()) {
    if (J == A && F == 3 && H == A && A >= 2 + D + B && A + B >= D && A + D >= 2 + B && A + D + B >= 0) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int H_ = 1 + H;
    if (J == A && F == 3 && H >= 2 + D + B && H + B >= D && H + D >= 2 + B && H + D + B >= 0 && A >= H && A >= 1 + H) {
     F = F_;
     H = H_;
     goto loc_lbl81;
    }
   }
  goto end;
 }
 loc_lbl21:
 {
 __cost++;
   if (nondet_bool()) {
    if (J == A && H == A && A >= 1 + D + B && A + B >= 1 + D && A + D >= 1 + B && A + D + B >= 1) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int H_ = 1 + H;
    if (J == A && H >= 1 + D + B && H + B >= 1 + D && H + D >= 1 + B && H + D + B >= 1 && A >= H && A >= 1 + H) {
     F = F_;
     H = H_;
     goto loc_lbl81;
    }
   }
  goto end;
 }
 loc_lbl81:
 {
 __cost++;
   if (nondet_bool()) {
    if (J == A && A >= H && H + D + B >= 1 && H >= 1 + D + B && H + B >= 1 + D && H + D >= 1 + B && 0 >= 1 + F) {
     goto loc_lbl21;
    }
   }
   if (nondet_bool()) {
    if (J == A && A >= H && H + D + B >= 1 && H >= 1 + D + B && H + B >= 1 + D && H + D >= 1 + B && F >= 4) {
     goto loc_lbl21;
    }
   }
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (J == A && F == 0 && A >= H && H + D + B >= 1 && H >= 1 + D + B && H + B >= 1 + D && H + D >= 1 + B) {
     D = D_;
     goto loc_lbl121;
    }
   }
   if (nondet_bool()) {
    int D_ = -1 + D;
    if (J == A && F == 1 && A >= H && H + D + B >= 1 && H >= 1 + D + B && H + B >= 1 + D && H + D >= 1 + B) {
     D = D_;
     goto loc_lbl141;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (J == A && F == 2 && A >= H && H + D + B >= 1 && H >= 1 + D + B && H + B >= 1 + D && H + D >= 1 + B) {
     B = B_;
     goto loc_lbl171;
    }
   }
   if (nondet_bool()) {
    int B_ = -1 + B;
    if (J == A && F == 3 && A >= H && H + D + B >= 1 && H >= 1 + D + B && H + B >= 1 + D && H + D >= 1 + B) {
     B = B_;
     goto loc_lbl191;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = 0;
    int D_ = 0;
    int B_ = 0;
    if (J == A && H == I && F == G && D == E && B == C && 0 >= A) {
     B = B_;
     D = D_;
     H = H_;
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int H_ = 1;
    int D_ = 0;
    int B_ = 0;
    if (J == A && H == I && F == G && D == E && B == C && A >= 1) {
     B = B_;
     D = D_;
     F = F_;
     H = H_;
     goto loc_lbl81;
    }
   }
  goto end;
 }
 loc_start0:
 {
 __cost++;
   if (nondet_bool()) {
    int J_ = A;
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
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
