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
    if (1 >= 0) {
     A = A_;
     goto loc_f9;
    }
   }
  goto end;
 }
 loc_f10:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (A >= 6) {
     A = A_;
     goto loc_f9;
    }
   }
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (5 >= A) {
     A = A_;
     goto loc_f9;
    }
   }
  goto end;
 }
 loc_f19:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (A >= 3) {
     A = A_;
     goto loc_f19;
    }
   }
   if (nondet_bool()) {
    if (2 >= A) {
     goto loc_f9;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f2;
    }
   }
  goto end;
 }
 loc_f25:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f2;
    }
   }
  goto end;
 }
 loc_f27:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f29;
    }
   }
  goto end;
 }
 loc_f9:
 {
 __cost++;
   if (nondet_bool()) {
    if (5 >= A) {
     goto loc_f10;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    if (0 >= 1 + B_ && A >= 6) {
     B = B_;
     goto loc_f10;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    if (B_ >= 1 && A >= 6) {
     B = B_;
     goto loc_f10;
    }
   }
   if (nondet_bool()) {
    int B_ = 0;
    if (A >= 6) {
     B = B_;
     goto loc_f19;
    }
   }
  goto end;
 }
 loc_f29:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
