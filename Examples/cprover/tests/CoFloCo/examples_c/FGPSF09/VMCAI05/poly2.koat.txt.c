int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_start;

 loc_eval:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    int B_ = -2 + B;
    int A_ = A + B;
    if (A >= 0) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_eval;
    }
   }
   if (nondet_bool()) {
    int C_ = -2 + C;
    int A_ = A + C;
    if (A >= 0) {
     A = A_;
     C = C_;
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
  foo(nondet(), nondet(), nondet());
}
