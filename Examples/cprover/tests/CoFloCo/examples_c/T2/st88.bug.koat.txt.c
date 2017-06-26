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
 loc_f12:
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
 loc_f20:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (A >= 3) {
     A = A_;
     goto loc_f20;
    }
   }
   if (nondet_bool()) {
    if (2 >= A) {
     goto loc_f9;
    }
   }
  goto end;
 }
 loc_f26:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f2;
    }
   }
  goto end;
 }
 loc_f28:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f30;
    }
   }
  goto end;
 }
 loc_f9:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = nondet();
    if (0 >= 1 + B_ && 5 >= A) {
     B = B_;
     goto loc_f12;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    if (B_ >= 1 && 5 >= A) {
     B = B_;
     goto loc_f12;
    }
   }
   if (nondet_bool()) {
    int B_ = 0;
    if (5 >= A) {
     B = B_;
     goto loc_f20;
    }
   }
   if (nondet_bool()) {
    if (A >= 6) {
     goto loc_f20;
    }
   }
  goto end;
 }
 loc_f30:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
