int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_start;

 loc_eval1:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1 + B) {
     goto loc_eval2;
    }
   }
  goto end;
 }
 loc_eval2:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (A >= 1 + C) {
     B = B_;
     goto loc_eval1;
    }
   }
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (A >= 1 + C) {
     C = C_;
     goto loc_eval1;
    }
   }
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (C >= A) {
     A = A_;
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
