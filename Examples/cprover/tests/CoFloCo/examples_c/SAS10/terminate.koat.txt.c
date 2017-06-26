int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H) {
 goto loc_start0;

 loc_lbl72:
 {
 __cost++;
   if (nondet_bool()) {
    if (D + F + B == A + E + C && 1 + G + F + B == A + E + C && C >= E && C >= 1 + B && 1 + B >= F && 101 + F + B >= A + E + C && 100 >= A && F >= 101) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    if (D + F + B == A + E + C && 1 + G + F + B == A + E + C && C >= E && C >= 1 + B && 1 + B >= F && 101 + F + B >= A + E + C && 100 >= A && A + E + C >= 1 + F + 2 * B) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int G_ = F;
    int F_ = D;
    int D_ = 1 + F;
    int B_ = -1 + B;
    if (D + F + B == A + E + C && 1 + G + F + B == A + E + C && C >= E && C >= 1 + B && 1 + B >= F && 101 + F + B >= A + E + C && 100 >= A && 100 >= F && 2 * B + F >= A + E + C) {
     B = B_;
     D = D_;
     F = F_;
     G = G_;
     goto loc_lbl72;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (G == H && F == A && D == E && B == C && A >= 101) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    if (G == H && F == A && D == E && B == C && E >= 1 + C) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int G_ = F;
    int F_ = D;
    int D_ = 1 + F;
    int B_ = -1 + B;
    if (G == H && F == A && D == E && B == C && 100 >= A && C >= E) {
     B = B_;
     D = D_;
     F = F_;
     G = G_;
     goto loc_lbl72;
    }
   }
  goto end;
 }
 loc_start0:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = H;
    int F_ = A;
    int D_ = E;
    int B_ = C;
    if (1 >= 0) {
     B = B_;
     D = D_;
     F = F_;
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
