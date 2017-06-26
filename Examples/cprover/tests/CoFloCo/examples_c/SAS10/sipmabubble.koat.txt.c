int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H) {
 goto loc_start0;

 loc_lbl131:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = -1 + G;
    if (D == A && E == G && A >= G && G >= 1) {
     G = G_;
     goto loc_lbl142;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    if (D == A && A >= G && E >= 1 && G >= E && G >= 1 + E) {
     B = B_;
     goto loc_lbl91;
    }
   }
   if (nondet_bool()) {
    int E_ = 1 + E;
    if (D == A && A >= G && E >= 1 && G >= E && G >= 1 + E) {
     E = E_;
     goto loc_lbl131;
    }
   }
  goto end;
 }
 loc_lbl142:
 {
 __cost++;
   if (nondet_bool()) {
    if (D == A && E == 0 && 1 + G == 0 && A >= 0) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int G_ = -1 + G;
    int E_ = 0;
    if (D == A && E == 1 && G == 0 && A >= 1) {
     E = E_;
     G = G_;
     goto loc_lbl142;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int E_ = 0;
    if (D == A && 1 + G == E && A >= E && E >= 0 && E >= 2) {
     B = B_;
     E = E_;
     goto loc_lbl91;
    }
   }
   if (nondet_bool()) {
    int E_ = 1;
    if (D == A && 1 + G == E && A >= E && E >= 0 && E >= 2) {
     E = E_;
     goto loc_lbl131;
    }
   }
  goto end;
 }
 loc_lbl91:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 1 + E;
    if (D == A && A >= G && G >= 1 + E && E >= 0) {
     E = E_;
     goto loc_lbl131;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = D;
    if (G == H && E == F && D == A && B == C && 0 >= 1 + A) {
     G = G_;
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int G_ = -1 + D;
    int E_ = 0;
    if (G == H && E == F && A == 0 && B == C && D == 0) {
     E = E_;
     G = G_;
     goto loc_lbl142;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int G_ = D;
    int E_ = 0;
    if (G == H && E == F && D == A && B == C && A >= 1) {
     B = B_;
     E = E_;
     G = G_;
     goto loc_lbl91;
    }
   }
   if (nondet_bool()) {
    int G_ = D;
    int E_ = 1;
    if (G == H && E == F && D == A && B == C && A >= 1) {
     E = E_;
     G = G_;
     goto loc_lbl131;
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
