int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = nondet();
    int B_ = 1;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_f7;
    }
   }
  goto end;
 }
 loc_f13:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f13;
    }
   }
  goto end;
 }
 loc_f15:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f17;
    }
   }
  goto end;
 }
 loc_f7:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = nondet();
    if (0 >= 1 + A) {
     A = A_;
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    int A_ = nondet();
    if (A >= 1) {
     A = A_;
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    int B_ = 1;
    int A_ = 0;
    if (A == 0) {
     A = A_;
     B = B_;
     goto loc_f13;
    }
   }
  goto end;
 }
 loc_f17:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
