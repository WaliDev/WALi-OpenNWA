int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_f1;

 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + B;
    if (B >= 1 && A >= 1 && A >= B) {
     C = C_;
     goto loc_f2;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    if (B >= 1 + C) {
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    if (C >= 1 + B) {
     goto loc_f3;
    }
   }
  goto end;
 }
 loc_f3:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 0;
    if (C >= 1 + A && C >= 1 && 1 + A >= 0) {
     C = C_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (1 + C >= 0 && A >= C) {
     C = C_;
     goto loc_f2;
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
