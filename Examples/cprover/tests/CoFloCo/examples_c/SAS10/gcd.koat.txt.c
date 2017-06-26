int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_start0;

 loc_cut:
 {
 __cost++;
   if (nondet_bool()) {
    if (D == B && C >= B && B >= 1 && A >= B) {
     goto loc_stop;
    }
   }
  goto end;
 }
 loc_lbl101:
 {
 __cost++;
   if (nondet_bool()) {
    if (D == B && C >= 2 * B && B >= 1 && A >= B) {
     goto loc_cut;
    }
   }
   if (nondet_bool()) {
    int B_ = B + -D;
    if (C >= D + B && D >= 1 && B >= 1 && A >= D && B >= 1 + D) {
     B = B_;
     goto loc_lbl101;
    }
   }
   if (nondet_bool()) {
    int D_ = D + -B;
    if (C >= D + B && D >= 1 && B >= 1 && A >= D && D >= 1 + B) {
     D = D_;
     goto loc_lbl111;
    }
   }
  goto end;
 }
 loc_lbl111:
 {
 __cost++;
   if (nondet_bool()) {
    if (D == B && A >= 2 * B && B >= 1 && C >= B) {
     goto loc_cut;
    }
   }
   if (nondet_bool()) {
    int B_ = B + -D;
    if (A >= D + B && D >= 1 && B >= 1 && C >= B && B >= 1 + D) {
     B = B_;
     goto loc_lbl101;
    }
   }
   if (nondet_bool()) {
    int D_ = D + -B;
    if (A >= D + B && D >= 1 && B >= 1 && C >= B && D >= 1 + B) {
     D = D_;
     goto loc_lbl111;
    }
   }
  goto end;
 }
 loc_lbl6:
 {
 __cost++;
   if (nondet_bool()) {
    if (B == C && D == A && 0 >= C && A >= 1) {
     goto loc_stop;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (D == A && B == C && 0 >= A) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    if (D == A && B == C && 0 >= C && A >= 1) {
     goto loc_lbl6;
    }
   }
   if (nondet_bool()) {
    if (C == A && B == A && D == A && A >= 1) {
     goto loc_cut;
    }
   }
   if (nondet_bool()) {
    int B_ = B + -D;
    if (D == A && B == C && C >= 1 + A && A >= 1) {
     B = B_;
     goto loc_lbl101;
    }
   }
   if (nondet_bool()) {
    int D_ = D + -B;
    if (D == A && B == C && C >= 1 && A >= 1 + C) {
     D = D_;
     goto loc_lbl111;
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
