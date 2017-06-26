int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H) {
 goto loc_start0;

 loc_lbl121:
 {
 __cost++;
   if (nondet_bool()) {
    if (D == A && G == E && 1 + E >= B && B >= A && 2 * A >= 1 + E && A >= 1 + E) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int G_ = -1 + G;
    if (D == A && G == E && 1 + E >= B && B >= A && 2 * A >= 1 + E && E >= A) {
     G = G_;
     goto loc_lbl82;
    }
   }
   if (nondet_bool()) {
    int G_ = -1 + E;
    int E_ = -1 + E;
    int B_ = G;
    if (D == A && G == E && 1 + E >= B && B >= A && 2 * A >= 1 + E && E >= A) {
     B = B_;
     E = E_;
     G = G_;
     goto loc_lbl121;
    }
   }
  goto end;
 }
 loc_lbl82:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 + G == A && D == A && 2 * A >= E && E >= A) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int G_ = -1 + G;
    if (D == A && 1 + G >= A && 2 * A >= E && E >= 1 + G && G >= A) {
     G = G_;
     goto loc_lbl82;
    }
   }
   if (nondet_bool()) {
    int G_ = -1 + E;
    int E_ = -1 + E;
    int B_ = G;
    if (D == A && 1 + G >= A && 2 * A >= E && E >= 1 + G && G >= A) {
     B = B_;
     E = E_;
     G = G_;
     goto loc_lbl121;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (G == H && E == F && D == A && B == C && 0 >= 1 + A) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int G_ = -1 + 2 * D;
    int E_ = 2 * D;
    if (G == H && E == F && D == A && B == C && A >= 0) {
     E = E_;
     G = G_;
     goto loc_lbl82;
    }
   }
   if (nondet_bool()) {
    int G_ = -1 + 2 * D;
    int E_ = -1 + 2 * D;
    int B_ = 2 * D;
    if (G == H && E == F && D == A && B == C && A >= 0) {
     B = B_;
     E = E_;
     G = G_;
     goto loc_lbl121;
    }
   }
  goto end;
 }
 loc_start0:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = H;
    int E_ = F;
    int D_ = A;
    int B_ = C;
    if (1 >= 0) {
     B = B_;
     D = D_;
     E = E_;
     G = G_;
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
