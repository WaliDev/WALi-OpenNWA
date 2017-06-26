int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H) {
 goto loc_start0;

 loc_cut:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = -1 + G;
    int E_ = -1 + E;
    if (D == A && A >= G + E && A >= 1 + G && E >= 0 && G >= 1 && E >= 2 && G >= 2) {
     E = E_;
     G = G_;
     goto loc_cut;
    }
   }
   if (nondet_bool()) {
    int G_ = -1 + G;
    int E_ = -1 + E;
    if (D == A && G == 1 && A >= 1 + E && A >= 2 && E >= 0 && E >= 2) {
     E = E_;
     G = G_;
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int G_ = -1 + G;
    int E_ = 0;
    if (D == A && A >= G + E && A >= 1 + G && E >= 0 && G >= 1 && 1 >= E && G >= 2) {
     E = E_;
     G = G_;
     goto loc_cut;
    }
   }
   if (nondet_bool()) {
    int G_ = -1 + G;
    int E_ = 0;
    if (D == A && G == 1 && A >= 1 + E && A >= 2 && E >= 0 && 1 >= E) {
     E = E_;
     G = G_;
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int G_ = -1 + G;
    int E_ = 1 + E;
    int B_ = E;
    if (D == A && A >= G + E && A >= 1 + G && E >= 0 && G >= 1 && A >= 2 + E && G >= 2) {
     B = B_;
     E = E_;
     G = G_;
     goto loc_cut;
    }
   }
   if (nondet_bool()) {
    int G_ = -1 + G;
    int E_ = 1 + E;
    int B_ = E;
    if (D == A && G == 1 && A >= 1 + E && A >= 2 && E >= 0 && A >= 2 + E) {
     B = B_;
     E = E_;
     G = G_;
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int G_ = -1 + G;
    int E_ = 0;
    int B_ = E;
    if (D == A && A >= G + E && A >= 1 + G && E >= 0 && G >= 1 && 1 + E >= A && G >= 2) {
     B = B_;
     E = E_;
     G = G_;
     goto loc_cut;
    }
   }
   if (nondet_bool()) {
    int G_ = -1 + G;
    int E_ = 0;
    int B_ = E;
    if (D == A && 1 + E == A && G == 1 && A >= 2 && A >= 1) {
     B = B_;
     E = E_;
     G = G_;
     goto loc_stop;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = D;
    int E_ = 0;
    if (G == H && E == F && D == A && B == C && 0 >= A) {
     E = E_;
     G = G_;
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int G_ = -1 + D;
    int E_ = 0;
    if (G == H && E == F && D == A && B == C && A >= 2) {
     E = E_;
     G = G_;
     goto loc_cut;
    }
   }
   if (nondet_bool()) {
    int G_ = -1 + D;
    int E_ = 0;
    if (G == H && E == F && A == 1 && B == C && D == 1) {
     E = E_;
     G = G_;
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int G_ = -1 + D;
    int E_ = 1;
    int B_ = 0;
    if (G == H && E == F && D == A && B == C && A >= 2) {
     B = B_;
     E = E_;
     G = G_;
     goto loc_cut;
    }
   }
   if (nondet_bool()) {
    int G_ = -1 + D;
    int E_ = 0;
    int B_ = 0;
    if (G == H && E == F && A == 1 && B == C && D == 1) {
     B = B_;
     E = E_;
     G = G_;
     goto loc_stop;
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
