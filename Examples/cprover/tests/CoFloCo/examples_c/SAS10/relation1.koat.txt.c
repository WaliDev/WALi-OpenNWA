int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_start0;

 loc_lbl41:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = A;
    if (C == D) {
     C = C_;
     goto loc_stop;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = nondet();
    if (C == D && A == B) {
     A = A_;
     goto loc_lbl41;
    }
   }
  goto end;
 }
 loc_start0:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = D;
    int A_ = B;
    if (1 >= 0) {
     A = A_;
     C = C_;
     goto loc_start;
    }
   }
  goto end;
 }
 loc_stop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
