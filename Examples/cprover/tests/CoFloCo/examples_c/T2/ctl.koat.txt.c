int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f10001;
    }
   }
   if (nondet_bool()) {
    int B_ = 2;
    if (1 >= 0) {
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = 1;
    if (1 >= 0) {
     B = B_;
     goto loc_f10001;
    }
   }
   if (nondet_bool()) {
    int B_ = 1;
    int A_ = 1;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_f110;
    }
   }
   if (nondet_bool()) {
    int B_ = 2;
    int A_ = B;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_f12;
    }
   }
   if (nondet_bool()) {
    int D_ = 1;
    int A_ = B;
    if (1 >= 0) {
     A = A_;
     D = D_;
     goto loc_f10001;
    }
   }
   if (nondet_bool()) {
    int D_ = 1;
    int B_ = 1;
    int A_ = B;
    if (1 >= 0) {
     A = A_;
     B = B_;
     D = D_;
     goto loc_f10001;
    }
   }
  goto end;
 }
 loc_f100:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1;
    if (1 >= 0) {
     B = B_;
     goto loc_f110;
    }
   }
   if (nondet_bool()) {
    int D_ = 1;
    if (1 >= 0) {
     D = D_;
     goto loc_f10001;
    }
   }
  goto end;
 }
 loc_f1000:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 2;
    if (1 >= 0) {
     B = B_;
     goto loc_f1200;
    }
   }
   if (nondet_bool()) {
    int D_ = 1;
    if (1 >= 0) {
     D = D_;
     goto loc_f10001;
    }
   }
   if (nondet_bool()) {
    int D_ = 1;
    int B_ = 1;
    if (1 >= 0) {
     B = B_;
     D = D_;
     goto loc_f10001;
    }
   }
  goto end;
 }
 loc_f110:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 2;
    if (1 >= 0) {
     B = B_;
     goto loc_f120;
    }
   }
   if (nondet_bool()) {
    int D_ = 1;
    if (1 >= 0) {
     D = D_;
     goto loc_f10001;
    }
   }
  goto end;
 }
 loc_f12:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f12;
    }
   }
   if (nondet_bool()) {
    int D_ = 1;
    if (1 >= 0) {
     D = D_;
     goto loc_f10001;
    }
   }
  goto end;
 }
 loc_f120:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f120;
    }
   }
   if (nondet_bool()) {
    int D_ = 1;
    if (1 >= 0) {
     D = D_;
     goto loc_f10001;
    }
   }
  goto end;
 }
 loc_f1200:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f1200;
    }
   }
   if (nondet_bool()) {
    int D_ = 1;
    if (1 >= 0) {
     D = D_;
     goto loc_f10001;
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
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f10001;
    }
   }
   if (nondet_bool()) {
    int A_ = B;
    if (1 >= 0) {
     A = A_;
     goto loc_f1200;
    }
   }
  goto end;
 }
 loc_f2200:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 0;
    if (C == 0) {
     C = C_;
     goto loc_f10000;
    }
   }
  goto end;
 }
 loc_f10001:
 loc_f10000:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
