int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E) {
 goto loc_f2;

 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = nondet();
    if (1 >= A) {
     B = B_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    if (C >= 2 && A >= 2) {
     goto loc_f300;
    }
   }
  goto end;
 }
 loc_f300:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = nondet();
    if (1 + D >= 0) {
     B = B_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int E_ = 1 + E;
    int D_ = 1 + D;
    if (0 >= 2 + D && E >= 0) {
     D = D_;
     E = E_;
     goto loc_f300;
    }
   }
   if (nondet_bool()) {
    int E_ = 1 + E;
    int D_ = 1 + D;
    if (0 >= 2 + D && 0 >= 2 + E) {
     D = D_;
     E = E_;
     goto loc_f300;
    }
   }
  goto end;
 }
 loc_f1:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet());
}
