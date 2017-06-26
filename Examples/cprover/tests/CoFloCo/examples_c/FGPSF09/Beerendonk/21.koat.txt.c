int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_start;

 loc_eval1:
 {
 __cost++;
   if (nondet_bool()) {
    if (C == A && A >= 1 + B) {
     goto loc_eval2;
    }
   }
  goto end;
 }
 loc_eval2:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = -1 + A;
    if (A >= 1 + B) {
     A = A_;
     C = C_;
     goto loc_eval2;
    }
   }
   if (nondet_bool()) {
    if (B >= A) {
     goto loc_eval1;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_eval1;
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
