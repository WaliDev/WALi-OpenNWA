int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H) {
 goto loc_start0;

 loc_cut:
 {
 __cost++;
   if (nondet_bool()) {
    if (B == C && D == A && G == C && E == 0 && A >= 2 && C >= 1 + A) {
     goto loc_stop;
    }
   }
  goto end;
 }
 loc_lbl121:
 {
 __cost++;
   if (nondet_bool()) {
    if (B == C && D == A && C >= 1 + A && G >= 0 && E >= 1 && A >= E && A + C >= 3 + G && C >= 1 + G && G >= C) {
     goto loc_cut;
    }
   }
   if (nondet_bool()) {
    int E_ = 1 + E;
    if (B == C && D == A && C >= 1 + A && G >= 0 && E >= 1 && A >= E && A + C >= 3 + G && C >= 1 + G && A >= 1 + E) {
     E = E_;
     goto loc_lbl121;
    }
   }
   if (nondet_bool()) {
    int G_ = 1 + G;
    int E_ = 0;
    if (B == C && D == A && E == A && C >= 1 + A && G >= 0 && A >= 1 && A + C >= 3 + G && C >= 1 + G) {
     E = E_;
     G = G_;
     goto loc_lbl141;
    }
   }
  goto end;
 }
 loc_lbl141:
 {
 __cost++;
   if (nondet_bool()) {
    if (B == C && D == A && E == 0 && G == C && C >= 1 && C >= 1 + A && A >= 2) {
     goto loc_cut;
    }
   }
   if (nondet_bool()) {
    int E_ = 1 + E;
    if (B == C && D == A && E == 0 && C >= G && G >= 1 && C >= 1 + A && A >= 2 && C >= 1 + G && A >= 1) {
     E = E_;
     goto loc_lbl121;
    }
   }
   if (nondet_bool()) {
    int G_ = 1 + G;
    int E_ = 0;
    if (B == C && D == A && E == 0 && C >= G && G >= 1 && C >= 1 + A && A >= 2 && C >= 1 + G && 0 >= A) {
     E = E_;
     G = G_;
     goto loc_lbl141;
    }
   }
  goto end;
 }
 loc_lbl6:
 {
 __cost++;
   if (nondet_bool()) {
    if (B == C && D == A && E == F && G == H && A >= C && A >= 1) {
     goto loc_stop;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (G == H && E == F && D == A && B == C && 0 >= A) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    if (G == H && E == F && D == A && B == C && A >= C && A >= 1) {
     goto loc_lbl6;
    }
   }
   if (nondet_bool()) {
    int G_ = 0;
    int E_ = 1;
    if (G == H && E == F && D == A && B == C && C >= 1 + A && A >= 1) {
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
