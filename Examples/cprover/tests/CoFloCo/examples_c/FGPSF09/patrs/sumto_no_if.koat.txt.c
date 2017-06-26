int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_start;

 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_sumto;
    }
   }
  goto end;
 }
 loc_sumto:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1 + B) {
     goto loc_end;
    }
   }
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (B >= A) {
     A = A_;
     goto loc_sumto;
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
