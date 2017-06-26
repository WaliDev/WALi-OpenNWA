int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_f3;

 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (A >= 1 + B) {
     B = B_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (B >= A && B >= 2 + C) {
     C = C_;
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f3:
 {
 __cost++;
   if (nondet_bool()) {
    if (B >= 1 + C) {
     goto loc_f1;
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
