int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_start0;

 loc_lbl151:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 2 + D;
    int B_ = -10 + B;
    if (7 * D + C >= B + 7 * A && 5 + D >= B && 5719 + 23 * B >= 56 * D + 140 * A + 28 * C && 140 * D + 23 * B >= 161 + 140 * A + 28 * C && 1561 + 2 * B >= 14 * D + 35 * A + 7 * C && 203 + B >= 2 * D + 5 * A + C && 29 >= A && D >= 1 + A && 5 * D + B >= 7 + 5 * A + C && 6 * D >= 7 + 5 * A + C && B >= D) {
     B = B_;
     D = D_;
     goto loc_lbl171;
    }
   }
   if (nondet_bool()) {
    int D_ = 1 + D;
    int B_ = 7 + B;
    if (7 * D + C >= B + 7 * A && 5 + D >= B && 5719 + 23 * B >= 56 * D + 140 * A + 28 * C && 140 * D + 23 * B >= 161 + 140 * A + 28 * C && 1561 + 2 * B >= 14 * D + 35 * A + 7 * C && 203 + B >= 2 * D + 5 * A + C && 29 >= A && D >= 1 + A && 5 * D + B >= 7 + 5 * A + C && 6 * D >= 7 + 5 * A + C && B >= 6 && D >= 1 + B) {
     B = B_;
     D = D_;
     goto loc_lbl151;
    }
   }
   if (nondet_bool()) {
    int D_ = 1 + D;
    int B_ = 2 + B;
    if (7 * D + C >= B + 7 * A && 5 + D >= B && 5719 + 23 * B >= 56 * D + 140 * A + 28 * C && 140 * D + 23 * B >= 161 + 140 * A + 28 * C && 1561 + 2 * B >= 14 * D + 35 * A + 7 * C && 203 + B >= 2 * D + 5 * A + C && 29 >= A && D >= 1 + A && 5 * D + B >= 7 + 5 * A + C && 6 * D >= 7 + 5 * A + C && 5 >= B && D >= 1 + B) {
     B = B_;
     D = D_;
     goto loc_lbl151;
    }
   }
  goto end;
 }
 loc_lbl171:
 {
 __cost++;
   if (nondet_bool()) {
    if (12 + B >= D && 1674 + 7 * B >= 19 * D + 35 * A + 7 * C && 7 * D + C >= 24 + B + 7 * A && 5 * D + B >= 5 * A + C && 29 >= A && D >= 30) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int D_ = 2 + D;
    int B_ = -10 + B;
    if (12 + B >= D && 1674 + 7 * B >= 19 * D + 35 * A + 7 * C && 7 * D + C >= 24 + B + 7 * A && 5 * D + B >= 5 * A + C && 29 >= A && 29 >= D && B >= D) {
     B = B_;
     D = D_;
     goto loc_lbl171;
    }
   }
   if (nondet_bool()) {
    int D_ = 1 + D;
    int B_ = 7 + B;
    if (12 + B >= D && 1674 + 7 * B >= 19 * D + 35 * A + 7 * C && 7 * D + C >= 24 + B + 7 * A && 5 * D + B >= 5 * A + C && 29 >= A && 29 >= D && B >= 6 && D >= 1 + B) {
     B = B_;
     D = D_;
     goto loc_lbl151;
    }
   }
   if (nondet_bool()) {
    int D_ = 1 + D;
    int B_ = 2 + B;
    if (12 + B >= D && 1674 + 7 * B >= 19 * D + 35 * A + 7 * C && 7 * D + C >= 24 + B + 7 * A && 5 * D + B >= 5 * A + C && 29 >= A && 29 >= D && 5 >= B && D >= 1 + B) {
     B = B_;
     D = D_;
     goto loc_lbl151;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (D == A && B == C && A >= 30) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int D_ = 2 + D;
    int B_ = -10 + B;
    if (D == A && B == C && 29 >= A && C >= A) {
     B = B_;
     D = D_;
     goto loc_lbl171;
    }
   }
   if (nondet_bool()) {
    int D_ = 1 + D;
    int B_ = 7 + B;
    if (D == A && B == C && 29 >= A && C >= 6 && A >= 1 + C) {
     B = B_;
     D = D_;
     goto loc_lbl151;
    }
   }
   if (nondet_bool()) {
    int D_ = 1 + D;
    int B_ = 2 + B;
    if (D == A && B == C && 29 >= A && 5 >= C && A >= 1 + C) {
     B = B_;
     D = D_;
     goto loc_lbl151;
    }
   }
  goto end;
 }
 loc_start0:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = A;
    int B_ = C;
    if (1 >= 0) {
     B = B_;
     D = D_;
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
  foo(nondet(), nondet(), nondet(), nondet());
}
