int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L) {
 goto loc_start0;

 loc_lbl13:
 {
 __cost++;
   if (nondet_bool()) {
    if (B == 0 && I == 1 && H == A && D == 1 && K == 0 && A >= 2) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int I_ = 0;
    int D_ = 0;
    if (I == 1 + B && K == B && H == A && D == 1 && A >= 2 + B && B >= 0 && B >= 1) {
     D = D_;
     F = F_;
     I = I_;
     goto loc_lbl71;
    }
   }
   if (nondet_bool()) {
    int I_ = 1;
    int D_ = 0;
    int B_ = 0;
    if (I == 1 + B && K == B && H == A && D == 1 && A >= 2 + B && B >= 0 && B >= 1) {
     B = B_;
     D = D_;
     I = I_;
     goto loc_lbl53;
    }
   }
  goto end;
 }
 loc_lbl53:
 {
 __cost++;
   if (nondet_bool()) {
    if (H == A && K == 1 + B && D == 0 && I == 1 + B && A >= 2 + B && B >= 0) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int K_ = -1 + K;
    if (H == A && K == 1 + B && I == 1 + B && A >= 2 + B && 1 >= D && D >= 0 && B >= 0 && 0 >= 1 + D) {
     K = K_;
     goto loc_lbl13;
    }
   }
   if (nondet_bool()) {
    int K_ = -1 + K;
    if (H == A && K == 1 + B && I == 1 + B && D == 1 && A >= 2 + B && B >= 0) {
     K = K_;
     goto loc_lbl13;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    if (H == A && I == 1 + B && A >= 1 + K && 1 >= D && K >= 1 + B && D >= 0 && B >= 0 && K >= 2 + B) {
     F = F_;
     goto loc_lbl71;
    }
   }
   if (nondet_bool()) {
    int I_ = 1 + I;
    int B_ = I;
    if (H == A && I == 1 + B && A >= 1 + K && 1 >= D && K >= 1 + B && D >= 0 && B >= 0 && K >= 2 + B) {
     B = B_;
     I = I_;
     goto loc_lbl53;
    }
   }
  goto end;
 }
 loc_lbl71:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = 1 + I;
    int D_ = 1;
    int B_ = I;
    if (H == A && A >= 1 + K && K >= 1 + I && I >= D && D >= 0) {
     B = B_;
     D = D_;
     I = I_;
     goto loc_lbl53;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    int K_ = -1 + H;
    if (K == L && I == J && H == A && F == G && D == E && B == C && 1 >= A) {
     K = K_;
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    int K_ = -1 + H;
    int I_ = 0;
    int D_ = 0;
    if (K == L && I == J && H == A && F == G && D == E && B == C && A >= 2) {
     D = D_;
     F = F_;
     I = I_;
     K = K_;
     goto loc_lbl71;
    }
   }
   if (nondet_bool()) {
    int K_ = -1 + H;
    int I_ = 1;
    int D_ = 0;
    int B_ = 0;
    if (K == L && I == J && H == A && F == G && D == E && B == C && A >= 2) {
     B = B_;
     D = D_;
     I = I_;
     K = K_;
     goto loc_lbl53;
    }
   }
  goto end;
 }
 loc_start0:
 {
 __cost++;
   if (nondet_bool()) {
    int K_ = L;
    int I_ = J;
    int H_ = A;
    int F_ = G;
    int D_ = E;
    int B_ = C;
    if (1 >= 0) {
     B = B_;
     D = D_;
     F = F_;
     H = H_;
     I = I_;
     K = K_;
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
