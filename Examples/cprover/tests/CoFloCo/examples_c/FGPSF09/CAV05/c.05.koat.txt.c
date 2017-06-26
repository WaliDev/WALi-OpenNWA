int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_start;

 loc_eval:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = A + -B;
    if (B >= 1 && A >= 1 && A >= 1 + B) {
     A = A_;
     goto loc_eval;
    }
   }
   if (nondet_bool()) {
    int A_ = A + -B;
    if (A >= 1 + B && B >= 1 && A >= 1 && B >= 1 + A) {
     A = A_;
     goto loc_eval;
    }
   }
   if (nondet_bool()) {
    int B_ = B + -A;
    if (B >= A && B >= 1 && A >= 1 && A >= 1 + B) {
     B = B_;
     goto loc_eval;
    }
   }
   if (nondet_bool()) {
    int B_ = B + -A;
    if (B >= A && B >= 1 && A >= 1 && B >= 1 + A) {
     B = B_;
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
