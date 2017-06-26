int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = nondet();
    if (A_ >= 1) {
     A = A_;
     goto loc_f5;
    }
   }
   if (nondet_bool()) {
    int A_ = nondet();
    if (0 >= A_) {
     A = A_;
     goto loc_f12;
    }
   }
  goto end;
 }
 loc_f5:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (19 >= A) {
     A = A_;
     goto loc_f5;
    }
   }
   if (nondet_bool()) {
    if (A >= 20) {
     goto loc_f12;
    }
   }
  goto end;
 }
 loc_f12:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet());
}
