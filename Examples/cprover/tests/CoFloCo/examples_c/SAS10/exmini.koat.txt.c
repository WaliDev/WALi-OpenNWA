int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H) {
 goto loc_start0;

 loc_lbl71:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 + H + B + D == A + G + E && F == 1 + B && 100 >= B && 2 + 2 * D + B >= A + G + E && E >= G && 100 >= A && E >= 1 + D && A + G + E >= 102 + B + D) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    if (1 + H + B + D == A + G + E && F == 1 + B && 100 >= B && 2 + 2 * D + B >= A + G + E && E >= G && 100 >= A && E >= 1 + D && B >= D) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int H_ = F;
    int F_ = 1 + H;
    int D_ = -1 + D;
    int B_ = H;
    if (1 + H + B + D == A + G + E && F == 1 + B && 100 >= B && 2 + 2 * D + B >= A + G + E && E >= G && 100 >= A && E >= 1 + D && 101 + B + D >= A + G + E && D >= 1 + B) {
     B = B_;
     D = D_;
     F = F_;
     H = H_;
     goto loc_lbl71;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (H == A && F == G && D == E && B == C && A >= 101) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    if (H == A && F == G && D == E && B == C && G >= 1 + E) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int H_ = F;
    int F_ = 1 + H;
    int D_ = -1 + D;
    int B_ = H;
    if (H == A && F == G && D == E && B == C && 100 >= A && E >= G) {
     B = B_;
     D = D_;
     F = F_;
     H = H_;
     goto loc_lbl71;
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
