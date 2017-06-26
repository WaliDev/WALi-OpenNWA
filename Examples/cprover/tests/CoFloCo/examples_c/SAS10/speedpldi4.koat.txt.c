int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F) {
 goto loc_start0;

 loc_cut:
 {
 __cost++;
   if (nondet_bool()) {
    if (B == C && D == A && E == 0 && C >= 1 + A && A >= 1) {
     goto loc_stop;
    }
   }
  goto end;
 }
 loc_lbl111:
 {
 __cost++;
   if (nondet_bool()) {
    if (B == C && D == A && E == 0 && A >= 2 && C >= 1 + A) {
     goto loc_cut;
    }
   }
   if (nondet_bool()) {
    int E_ = -1 + E;
    if (B == C && D == A && A >= 2 + E && C >= 1 + E + A && E >= 0 && A >= 1 + E && E >= 1) {
     E = E_;
     goto loc_lbl111;
    }
   }
   if (nondet_bool()) {
    int E_ = E + -D;
    if (B == C && D == A && A >= 2 + E && C >= 1 + E + A && E >= 0 && E >= A && E >= 1) {
     E = E_;
     goto loc_lbl121;
    }
   }
  goto end;
 }
 loc_lbl121:
 {
 __cost++;
   if (nondet_bool()) {
    if (B == C && D == A && E == 0 && C >= A && A >= 1 && C >= 1 + A) {
     goto loc_cut;
    }
   }
   if (nondet_bool()) {
    int E_ = -1 + E;
    if (B == C && D == A && C >= E + A && E >= 0 && A >= 1 && C >= 1 + A && A >= 1 + E && E >= 1) {
     E = E_;
     goto loc_lbl111;
    }
   }
   if (nondet_bool()) {
    int E_ = E + -D;
    if (B == C && D == A && C >= E + A && E >= 0 && A >= 1 && C >= 1 + A && E >= A && E >= 1) {
     E = E_;
     goto loc_lbl121;
    }
   }
  goto end;
 }
 loc_lbl6:
 {
 __cost++;
   if (nondet_bool()) {
    if (B == C && D == A && E == F && A >= C && A >= 1) {
     goto loc_stop;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (E == F && D == A && B == C && 0 >= A) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    if (E == F && D == A && B == C && A >= C && A >= 1) {
     goto loc_lbl6;
    }
   }
   if (nondet_bool()) {
    int E_ = B + -D;
    if (E == F && D == A && B == C && C >= 1 + A && A >= 1) {
     E = E_;
     goto loc_lbl121;
    }
   }
  goto end;
 }
 loc_start0:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = F;
    int D_ = A;
    int B_ = C;
    if (1 >= 0) {
     B = B_;
     D = D_;
     E = E_;
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
