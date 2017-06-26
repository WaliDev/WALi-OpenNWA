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
    if (1 >= 0) {
     A = A_;
     goto loc_f4;
    }
   }
  goto end;
 }
 loc_f4:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (A >= 1 && 3 >= A) {
     A = A_;
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int A_ = 1;
    if (3 >= A && 0 >= A) {
     A = A_;
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    if (A >= 4) {
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
