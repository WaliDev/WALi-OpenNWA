int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 4;
    int A_ = 2;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_f5;
    }
   }
  goto end;
 }
 loc_f5:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 4 + A;
    int A_ = 2 + A;
    if (19 >= A) {
     A = A_;
     B = B_;
     goto loc_f5;
    }
   }
   if (nondet_bool()) {
    if (A >= 20) {
     goto loc_f8;
    }
   }
  goto end;
 }
 loc_f8:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
