int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_evalrandom2dstart;

 loc_evalrandom2dLeafBlock1in:
 {
 __cost++;
   if (nondet_bool()) {
    if (D == 1) {
     goto loc_evalrandom2dbb5in;
    }
   }
   if (nondet_bool()) {
    if (0 >= D) {
     goto loc_evalrandom2dNewDefaultin;
    }
   }
   if (nondet_bool()) {
    if (D >= 2) {
     goto loc_evalrandom2dNewDefaultin;
    }
   }
  goto end;
 }
 loc_evalrandom2dLeafBlock3in:
 {
 __cost++;
   if (nondet_bool()) {
    if (D == 2) {
     goto loc_evalrandom2dbb7in;
    }
   }
   if (nondet_bool()) {
    if (1 >= D) {
     goto loc_evalrandom2dNewDefaultin;
    }
   }
   if (nondet_bool()) {
    if (D >= 3) {
     goto loc_evalrandom2dNewDefaultin;
    }
   }
  goto end;
 }
 loc_evalrandom2dLeafBlock5in:
 {
 __cost++;
   if (nondet_bool()) {
    if (D == 3) {
     goto loc_evalrandom2dbb9in;
    }
   }
   if (nondet_bool()) {
    if (2 >= D) {
     goto loc_evalrandom2dNewDefaultin;
    }
   }
   if (nondet_bool()) {
    if (D >= 4) {
     goto loc_evalrandom2dNewDefaultin;
    }
   }
  goto end;
 }
 loc_evalrandom2dLeafBlockin:
 {
 __cost++;
   if (nondet_bool()) {
    if (D == 0) {
     goto loc_evalrandom2dbb3in;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + D) {
     goto loc_evalrandom2dNewDefaultin;
    }
   }
   if (nondet_bool()) {
    if (D >= 1) {
     goto loc_evalrandom2dNewDefaultin;
    }
   }
  goto end;
 }
 loc_evalrandom2dNewDefaultin:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = C;
    if (1 >= 0) {
     A = A_;
     goto loc_evalrandom2dbb10in;
    }
   }
  goto end;
 }
 loc_evalrandom2dNodeBlock7in:
 {
 __cost++;
   if (nondet_bool()) {
    if (2 >= D) {
     goto loc_evalrandom2dLeafBlock3in;
    }
   }
   if (nondet_bool()) {
    if (D >= 3) {
     goto loc_evalrandom2dLeafBlock5in;
    }
   }
  goto end;
 }
 loc_evalrandom2dNodeBlock9in:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= D) {
     goto loc_evalrandom2dNodeBlockin;
    }
   }
   if (nondet_bool()) {
    if (D >= 2) {
     goto loc_evalrandom2dNodeBlock7in;
    }
   }
  goto end;
 }
 loc_evalrandom2dNodeBlockin:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= D) {
     goto loc_evalrandom2dLeafBlockin;
    }
   }
   if (nondet_bool()) {
    if (D >= 1) {
     goto loc_evalrandom2dLeafBlock1in;
    }
   }
  goto end;
 }
 loc_evalrandom2dbb10in:
 {
 __cost++;
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_evalrandom2dbbin;
    }
   }
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_evalrandom2dreturnin;
    }
   }
  goto end;
 }
 loc_evalrandom2dbb2in:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalrandom2dNodeBlock9in;
    }
   }
  goto end;
 }
 loc_evalrandom2dbb3in:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = C;
    if (1 >= 0) {
     A = A_;
     goto loc_evalrandom2dbb10in;
    }
   }
  goto end;
 }
 loc_evalrandom2dbb5in:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = C;
    if (1 >= 0) {
     A = A_;
     goto loc_evalrandom2dbb10in;
    }
   }
  goto end;
 }
 loc_evalrandom2dbb7in:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = C;
    if (1 >= 0) {
     A = A_;
     goto loc_evalrandom2dbb10in;
    }
   }
  goto end;
 }
 loc_evalrandom2dbb9in:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = C;
    if (1 >= 0) {
     A = A_;
     goto loc_evalrandom2dbb10in;
    }
   }
  goto end;
 }
 loc_evalrandom2dbbin:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = nondet();
    int C_ = 1 + A;
    if (3 >= D_ && D_ >= 0) {
     C = C_;
     D = D_;
     goto loc_evalrandom2dbb2in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int A_ = 1 + A;
    if (0 >= 1 + Y_0) {
     A = A_;
     goto loc_evalrandom2dbb10in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int A_ = 1 + A;
    if (Y_0 >= 4) {
     A = A_;
     goto loc_evalrandom2dbb10in;
    }
   }
  goto end;
 }
 loc_evalrandom2dentryin:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     goto loc_evalrandom2dbb10in;
    }
   }
  goto end;
 }
 loc_evalrandom2dreturnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalrandom2dstop;
    }
   }
  goto end;
 }
 loc_evalrandom2dstart:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalrandom2dentryin;
    }
   }
  goto end;
 }
 loc_evalrandom2dstop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
