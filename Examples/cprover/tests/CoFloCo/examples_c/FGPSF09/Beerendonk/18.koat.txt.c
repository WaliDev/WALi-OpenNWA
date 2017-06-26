int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_start;

 loc_eval:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (A >= 1) {
     A = A_;
     goto loc_eval;
    }
   }
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (A >= 1 && B >= 1) {
     A = A_;
     goto loc_eval;
    }
   }
   if (nondet_bool()) {
    int B_ = -1 + B;
    if (B >= 1 && 0 >= A && A >= 1) {
     B = B_;
     goto loc_eval;
    }
   }
   if (nondet_bool()) {
    int B_ = -1 + B;
    if (0 >= A && B >= 1) {
     B = B_;
     goto loc_eval;
    }
   }
   if (nondet_bool()) {
    if (0 >= B && 0 >= A && A >= 1) {
     goto loc_eval;
    }
   }
   if (nondet_bool()) {
    if (0 >= B && 0 >= A && B >= 1) {
     goto loc_eval;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_eval;
    }
   }
  goto end;
 }
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
