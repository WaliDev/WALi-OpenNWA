int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_start0;

 loc_lbl32:
 {
 __cost++;
   if (nondet_bool()) {
    if (B == C && D == 1 && A >= 2) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int D_ = -1 + D;
    if (B == C && A >= 1 + D && D >= 1 && D >= 2) {
     D = D_;
     goto loc_lbl32;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (D == A && B == C && 1 >= A) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int D_ = -1 + D;
    if (D == A && B == C && A >= 2) {
     D = D_;
     goto loc_lbl32;
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
