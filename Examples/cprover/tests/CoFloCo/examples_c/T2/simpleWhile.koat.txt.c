int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 0;
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_f6;
    }
   }
  goto end;
 }
 loc_f15:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 1;
    int A_ = 1 + C;
    if (A == 1 + C) {
     A = A_;
     D = D_;
     goto loc_f19;
    }
   }
   if (nondet_bool()) {
    int D_ = 0;
    if (C >= A) {
     D = D_;
     goto loc_f19;
    }
   }
   if (nondet_bool()) {
    int D_ = 0;
    if (A >= 2 + C) {
     D = D_;
     goto loc_f19;
    }
   }
  goto end;
 }
 loc_f6:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (C >= 1 + B) {
     B = B_;
     goto loc_f6;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    int A_ = 2 + A;
    if (C >= 1 + B) {
     A = A_;
     B = B_;
     goto loc_f6;
    }
   }
   if (nondet_bool()) {
    if (C >= 1 + A && B >= C) {
     goto loc_f15;
    }
   }
   if (nondet_bool()) {
    if (B >= C && A >= 1 + C) {
     goto loc_f15;
    }
   }
   if (nondet_bool()) {
    int D_ = 1;
    int C_ = A;
    if (A == C && B >= C) {
     C = C_;
     D = D_;
     goto loc_f19;
    }
   }
  goto end;
 }
 loc_f19:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
