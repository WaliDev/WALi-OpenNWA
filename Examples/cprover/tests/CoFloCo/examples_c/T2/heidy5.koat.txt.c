int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1;
    if (A >= 1) {
     C = C_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    if (0 >= A) {
     C = C_;
     goto loc_f4;
    }
   }
  goto end;
 }
 loc_f4:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1;
    if (0 >= B && 0 >= A) {
     C = C_;
     goto loc_f5;
    }
   }
   if (nondet_bool()) {
    int B_ = -1 + B;
    if (B >= 1) {
     B = B_;
     goto loc_f4;
    }
   }
  goto end;
 }
 loc_f5:
 loc_f2:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
