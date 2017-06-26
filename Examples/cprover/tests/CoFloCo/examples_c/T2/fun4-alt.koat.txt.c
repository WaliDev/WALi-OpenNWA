int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 2;
    if (B >= 0 && 3 >= B && 3 >= A && A >= 0) {
     C = C_;
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (C + A >= 1 + 2 * B) {
     B = B_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int B_ = -1 + B;
    if (2 * B >= 2 + C + A) {
     B = B_;
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
