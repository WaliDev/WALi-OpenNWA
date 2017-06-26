int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B) {
 goto loc_evalsipmabubblestart;

 loc_evalsipmabubblebb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    if (Y_0 >= 1 + Y_1) {
     goto loc_evalsipmabubblebb2in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    if (Y_0 >= Y_1) {
     goto loc_evalsipmabubblebb3in;
    }
   }
  goto end;
 }
 loc_evalsipmabubblebb2in:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalsipmabubblebb3in;
    }
   }
  goto end;
 }
 loc_evalsipmabubblebb3in:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (1 >= 0) {
     B = B_;
     goto loc_evalsipmabubblebb4in;
    }
   }
  goto end;
 }
 loc_evalsipmabubblebb4in:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1 + B) {
     goto loc_evalsipmabubblebb1in;
    }
   }
   if (nondet_bool()) {
    if (B >= A) {
     goto loc_evalsipmabubblebb5in;
    }
   }
  goto end;
 }
 loc_evalsipmabubblebb5in:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (1 >= 0) {
     A = A_;
     goto loc_evalsipmabubblebb6in;
    }
   }
  goto end;
 }
 loc_evalsipmabubblebb6in:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 0;
    if (A >= 0) {
     B = B_;
     goto loc_evalsipmabubblebb4in;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + A) {
     goto loc_evalsipmabubblereturnin;
    }
   }
  goto end;
 }
 loc_evalsipmabubbleentryin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalsipmabubblebb6in;
    }
   }
  goto end;
 }
 loc_evalsipmabubblereturnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalsipmabubblestop;
    }
   }
  goto end;
 }
 loc_evalsipmabubblestart:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalsipmabubbleentryin;
    }
   }
  goto end;
 }
 loc_evalsipmabubblestop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet());
}
