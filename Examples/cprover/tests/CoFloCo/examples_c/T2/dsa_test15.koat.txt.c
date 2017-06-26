int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = nondet();
    int B_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_f8;
    }
   }
  goto end;
 }
 loc_f19:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (9 >= C) {
     C = C_;
     goto loc_f19;
    }
   }
   if (nondet_bool()) {
    if (C >= 10) {
     goto loc_f29;
    }
   }
  goto end;
 }
 loc_f8:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (9 >= B) {
     B = B_;
     goto loc_f8;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    if (B >= 10) {
     C = C_;
     goto loc_f19;
    }
   }
  goto end;
 }
 loc_f29:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
