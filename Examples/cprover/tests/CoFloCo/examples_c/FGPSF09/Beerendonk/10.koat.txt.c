int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A) {
 goto loc_start;

 loc_eval:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int A_ = 0;
    if (A == 1 && 0 >= 2 * Y_0 && 2 * Y_0 >= 0) {
     A = A_;
     goto loc_eval;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int A_ = 2 * Y_0;
    if (A == 1 + 2 * Y_0 && 2 + 2 * Y_0 >= 0 && 2 * Y_0 >= 0) {
     A = A_;
     goto loc_eval;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Y_2 = nondet();
    int Y_3 = nondet();
    int A_ = nondet();
    if (1 >= 2 * Y_3 && 2 * Y_3 >= 0 && 2 * Y_2 >= 1 && 1 >= 2 * Y_2 && 1 >= 2 * Y_1 && 3 * Y_1 >= 2 && A_ >= Y_1 && 1 >= 2 * Y_0 && 3 * Y_0 >= 2 && Y_0 >= A_ && A == 1) {
     A = A_;
     goto loc_eval;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Y_2 = nondet();
    int Y_3 = nondet();
    int A_ = nondet();
    if (2 * Y_0 >= 1 && 1 + 2 * Y_0 >= 0 && 2 * Y_0 >= 2 * Y_3 && 1 + 2 * Y_3 >= 2 * Y_0 && 2 * Y_0 >= 2 * Y_2 && 3 * Y_2 >= 1 + 2 * Y_0 && A_ >= Y_2 && 2 * Y_0 >= 2 * Y_1 && 3 * Y_1 >= 1 + 2 * Y_0 && Y_1 >= A_ && A == 2 * Y_0) {
     A = A_;
     goto loc_eval;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_eval;
    }
   }
  goto end;
 }
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet());
}
