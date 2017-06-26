int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_start;

 loc_div:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= A) {
     goto loc_end;
    }
   }
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_end;
    }
   }
   if (nondet_bool()) {
    int B_ = B + -A;
    if (A >= 1 && B >= 1 + A) {
     B = B_;
     goto loc_div;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_div;
    }
   }
  goto end;
 }
 loc_end:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
