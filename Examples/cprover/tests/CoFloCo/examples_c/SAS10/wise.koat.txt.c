int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_start0;

 loc_lbl81:
 {
 __cost++;
   if (nondet_bool()) {
    if (B == C && 2 + D == C && A >= 0 && C >= 3 + A) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (B == C && C >= 2 + D && A >= 0 && D >= 1 + A && C >= 3 + D) {
     D = D_;
     goto loc_lbl81;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (B == C && C >= 2 + D && A >= 0 && D >= 1 + A && D >= 3 + C) {
     B = B_;
     goto loc_lbl91;
    }
   }
  goto end;
 }
 loc_lbl91:
 {
 __cost++;
   if (nondet_bool()) {
    if (D == A && 2 + B == A && C >= 0 && A >= 3 + C) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (D == A && C >= 0 && B >= 1 + C && A >= 2 + B && B >= 3 + A) {
     D = D_;
     goto loc_lbl81;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (D == A && C >= 0 && B >= 1 + C && A >= 2 + B && A >= 3 + B) {
     B = B_;
     goto loc_lbl91;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (D == A && B == C && 0 >= 1 + A) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    if (D == A && B == C && 0 >= 1 + C) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    if (D == A && B == C && 2 + C >= A && C >= 0 && 2 + A >= C && A >= 0) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (D == A && B == C && C >= 3 + A && A >= 0) {
     D = D_;
     goto loc_lbl81;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (D == A && B == C && C >= 0 && A >= 3 + C) {
     B = B_;
     goto loc_lbl91;
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
