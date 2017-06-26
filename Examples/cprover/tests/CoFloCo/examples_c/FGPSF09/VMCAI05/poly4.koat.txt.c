int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E) {
 goto loc_start;

 loc_eval:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 1 + E;
    int B_ = 1 + B;
    if (C >= 1 + D && A >= 1 + B) {
     B = B_;
     E = E_;
     goto loc_eval;
    }
   }
   if (nondet_bool()) {
    int E_ = 1 + E;
    int D_ = 1 + D;
    if (C >= 1 + D && A >= 1 + B) {
     D = D_;
     E = E_;
     goto loc_eval;
    }
   }
   if (nondet_bool()) {
    int E_ = 1 + E;
    int D_ = 1 + D;
    if (C >= 1 + D && B >= A) {
     D = D_;
     E = E_;
     goto loc_eval;
    }
   }
   if (nondet_bool()) {
    int E_ = 1 + E;
    int B_ = 1 + B;
    if (D >= C && A >= 1 + B) {
     B = B_;
     E = E_;
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
  foo(nondet(), nondet(), nondet(), nondet(), nondet());
}
