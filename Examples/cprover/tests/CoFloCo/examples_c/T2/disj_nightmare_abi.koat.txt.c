int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_start;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = A;
    if (B == 0 && 0 >= 1 + A) {
     B = B_;
     goto loc_f0;
    }
   }
   if (nondet_bool()) {
    int B_ = A;
    if (B == 0 && A >= 1) {
     B = B_;
     goto loc_f0;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f0;
    }
   }
  goto end;
 }
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
