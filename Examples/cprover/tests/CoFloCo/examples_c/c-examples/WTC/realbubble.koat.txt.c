int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_evalrealbubblestart;

 loc_evalrealbubblebb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    if (Y_0 >= 1 + Y_1) {
     goto loc_evalrealbubblebb2in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int D_ = C;
    if (Y_0 >= Y_1) {
     D = D_;
     goto loc_evalrealbubblebb3in;
    }
   }
  goto end;
 }
 loc_evalrealbubblebb2in:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 1;
    if (1 >= 0) {
     D = D_;
     goto loc_evalrealbubblebb3in;
    }
   }
  goto end;
 }
 loc_evalrealbubblebb3in:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = D;
    int B_ = 1 + B;
    if (1 >= 0) {
     B = B_;
     C = C_;
     goto loc_evalrealbubblebb4in;
    }
   }
  goto end;
 }
 loc_evalrealbubblebb4in:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1 + B) {
     goto loc_evalrealbubblebb1in;
    }
   }
   if (nondet_bool()) {
    if (B >= A) {
     goto loc_evalrealbubblebb5in;
    }
   }
  goto end;
 }
 loc_evalrealbubblebb5in:
 {
 __cost++;
   if (nondet_bool()) {
    if (C == 0) {
     goto loc_evalrealbubblereturnin;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + C) {
     goto loc_evalrealbubblebb6in;
    }
   }
   if (nondet_bool()) {
    if (C >= 1) {
     goto loc_evalrealbubblebb6in;
    }
   }
  goto end;
 }
 loc_evalrealbubblebb6in:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (1 >= 0) {
     A = A_;
     goto loc_evalrealbubblebb7in;
    }
   }
  goto end;
 }
 loc_evalrealbubblebb7in:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 0;
    int B_ = 0;
    if (A >= 1) {
     B = B_;
     C = C_;
     goto loc_evalrealbubblebb4in;
    }
   }
   if (nondet_bool()) {
    if (0 >= A) {
     goto loc_evalrealbubblereturnin;
    }
   }
  goto end;
 }
 loc_evalrealbubbleentryin:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (1 >= 0) {
     A = A_;
     goto loc_evalrealbubblebb7in;
    }
   }
  goto end;
 }
 loc_evalrealbubblereturnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalrealbubblestop;
    }
   }
  goto end;
 }
 loc_evalrealbubblestart:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalrealbubbleentryin;
    }
   }
  goto end;
 }
 loc_evalrealbubblestop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
