int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_evalrealheapsortstart;

 loc_evalrealheapsortbb10in:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalrealheapsortbb11in;
    }
   }
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalrealheapsortbb12in;
    }
   }
  goto end;
 }
 loc_evalrealheapsortbb11in:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 1 + 2 * C;
    if (1 >= 0) {
     D = D_;
     goto loc_evalrealheapsortbb13in;
    }
   }
  goto end;
 }
 loc_evalrealheapsortbb12in:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 2 + 2 * C;
    if (1 >= 0) {
     D = D_;
     goto loc_evalrealheapsortbb13in;
    }
   }
  goto end;
 }
 loc_evalrealheapsortbb13in:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalrealheapsortbb14in;
    }
   }
   if (nondet_bool()) {
    int C_ = A;
    if (1 >= 0) {
     C = C_;
     goto loc_evalrealheapsortbb16in;
    }
   }
  goto end;
 }
 loc_evalrealheapsortbb14in:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = D;
    if (1 >= 0) {
     C = C_;
     goto loc_evalrealheapsortbb16in;
    }
   }
  goto end;
 }
 loc_evalrealheapsortbb16in:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 3 + B + 2 * C) {
     goto loc_evalrealheapsortbb9in;
    }
   }
   if (nondet_bool()) {
    if (2 + 2 * C + B >= A) {
     goto loc_evalrealheapsortbb17in;
    }
   }
  goto end;
 }
 loc_evalrealheapsortbb17in:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (1 >= 0) {
     B = B_;
     goto loc_evalrealheapsortbb18in;
    }
   }
  goto end;
 }
 loc_evalrealheapsortbb18in:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 2 + B) {
     goto loc_evalrealheapsortbb8in;
    }
   }
   if (nondet_bool()) {
    if (1 + B >= A) {
     goto loc_evalrealheapsortreturnin;
    }
   }
  goto end;
 }
 loc_evalrealheapsortbb2in:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1;
    if (1 + C == 0) {
     C = C_;
     goto loc_evalrealheapsortbb3in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int C_ = -1 + Y_0;
    if (1 + C == 0 && 1 + 2 * Y_0 >= 0 && 0 >= 2 * Y_0 && Y_0 >= 0 && 0 >= 1) {
     C = C_;
     goto loc_evalrealheapsortbb3in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int C_ = -1 + Y_0;
    if (2 + C >= 2 * Y_0 && 2 * Y_0 >= 1 + C && 1 + C == 0 && 0 >= Y_0 && 0 >= 1) {
     C = C_;
     goto loc_evalrealheapsortbb3in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int C_ = -1;
    if (1 + C == 0 && 1 + 2 * Y_0 >= 0 && 0 >= 2 * Y_0 && Y_0 >= 0 && 0 >= 1) {
     C = C_;
     goto loc_evalrealheapsortbb3in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int C_ = -1 + Y_0;
    if (1 + C == 0 && 1 + 2 * Y_0 >= 0 && 0 >= 2 * Y_0 && Y_0 >= 0 && 1 + 2 * Y_1 >= 0 && 0 >= 2 * Y_1 && Y_1 >= 0 && 0 >= 1) {
     C = C_;
     goto loc_evalrealheapsortbb3in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int C_ = -1 + Y_0;
    if (2 + C >= 2 * Y_0 && 2 * Y_0 >= 1 + C && 1 + C == 0 && 0 >= Y_0 && 1 + 2 * Y_1 >= 0 && 0 >= 2 * Y_1 && Y_1 >= 0 && 0 >= 1) {
     C = C_;
     goto loc_evalrealheapsortbb3in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int C_ = -1;
    if (2 + C >= 2 * Y_0 && 2 * Y_0 >= 1 + C && 1 + C == 0 && 0 >= Y_0 && 0 >= 1) {
     C = C_;
     goto loc_evalrealheapsortbb3in;
    }
   }
   if (nondet_bool()) {
    int Y_1 = nondet();
    int Y_0 = nondet();
    int C_ = -1 + Y_1;
    if (2 + C >= 2 * Y_0 && 2 * Y_0 >= 1 + C && 1 + C == 0 && 1 + 2 * Y_1 >= 0 && 0 >= 2 * Y_1 && Y_1 >= 0 && 0 >= Y_0 && 0 >= 1) {
     C = C_;
     goto loc_evalrealheapsortbb3in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int C_ = -1 + Y_0;
    if (2 + C >= 2 * Y_0 && 2 * Y_0 >= 1 + C && 2 + C >= 2 * Y_1 && 2 * Y_1 >= 1 + C && 1 + C == 0 && 0 >= Y_0 && 0 >= Y_1 && 0 >= 1) {
     C = C_;
     goto loc_evalrealheapsortbb3in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int C_ = -1;
    if (1 + C == 0 && 1 + 2 * Y_0 >= 0 && 0 >= 2 * Y_0 && Y_0 >= 0 && 0 >= 1) {
     C = C_;
     goto loc_evalrealheapsortbb3in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int C_ = -1 + Y_0;
    if (1 + C == 0 && 1 + 2 * Y_0 >= 0 && 0 >= 2 * Y_0 && Y_0 >= 0 && 1 + 2 * Y_1 >= 0 && 0 >= 2 * Y_1 && Y_1 >= 0 && 0 >= 1) {
     C = C_;
     goto loc_evalrealheapsortbb3in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int C_ = -1 + Y_0;
    if (2 + C >= 2 * Y_0 && 2 * Y_0 >= 1 + C && 1 + C == 0 && 0 >= Y_0 && 1 + 2 * Y_1 >= 0 && 0 >= 2 * Y_1 && Y_1 >= 0 && 0 >= 1) {
     C = C_;
     goto loc_evalrealheapsortbb3in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int C_ = -1;
    if (1 + C == 0 && 1 + 2 * Y_0 >= 0 && 0 >= 2 * Y_0 && Y_0 >= 0 && 1 + 2 * Y_1 >= 0 && 0 >= 2 * Y_1 && Y_1 >= 0 && 0 >= 1) {
     C = C_;
     goto loc_evalrealheapsortbb3in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Y_2 = nondet();
    int C_ = -1 + Y_0;
    if (2 * Y_0 >= C && 1 + C >= 2 * Y_0 && Y_0 >= 0 && 2 * Y_1 >= C && 1 + C >= 2 * Y_1 && Y_1 >= 0 && 2 * Y_2 >= C && 1 + C >= 2 * Y_2 && Y_2 >= 0 && C >= 0) {
     C = C_;
     goto loc_evalrealheapsortbb3in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Y_2 = nondet();
    int C_ = -1 + Y_0;
    if (2 + C >= 2 * Y_0 && 2 * Y_0 >= 1 + C && 0 >= Y_0 && 0 >= 2 + C && 2 * Y_1 >= C && 1 + C >= 2 * Y_1 && Y_1 >= 0 && 2 * Y_2 >= C && 1 + C >= 2 * Y_2 && Y_2 >= 0 && C >= 0) {
     C = C_;
     goto loc_evalrealheapsortbb3in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int C_ = -1;
    if (2 + C >= 2 * Y_0 && 2 * Y_0 >= 1 + C && 1 + C == 0 && 0 >= Y_0 && 1 + 2 * Y_1 >= 0 && 0 >= 2 * Y_1 && Y_1 >= 0 && 0 >= 1) {
     C = C_;
     goto loc_evalrealheapsortbb3in;
    }
   }
   if (nondet_bool()) {
    int Y_1 = nondet();
    int Y_0 = nondet();
    int Y_2 = nondet();
    int C_ = -1 + Y_1;
    if (2 + C >= 2 * Y_0 && 2 * Y_0 >= 1 + C && 2 * Y_1 >= C && 1 + C >= 2 * Y_1 && Y_1 >= 0 && 0 >= Y_0 && 0 >= 2 + C && 2 * Y_2 >= C && 1 + C >= 2 * Y_2 && Y_2 >= 0 && C >= 0) {
     C = C_;
     goto loc_evalrealheapsortbb3in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Y_2 = nondet();
    int C_ = -1 + Y_0;
    if (2 + C >= 2 * Y_0 && 2 * Y_0 >= 1 + C && 2 + C >= 2 * Y_1 && 2 * Y_1 >= 1 + C && 0 >= Y_0 && 0 >= Y_1 && 0 >= 2 + C && 2 * Y_2 >= C && 1 + C >= 2 * Y_2 && Y_2 >= 0 && C >= 0) {
     C = C_;
     goto loc_evalrealheapsortbb3in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int C_ = -1;
    if (2 + C >= 2 * Y_0 && 2 * Y_0 >= 1 + C && 1 + C == 0 && 0 >= Y_0 && 0 >= 1) {
     C = C_;
     goto loc_evalrealheapsortbb3in;
    }
   }
   if (nondet_bool()) {
    int Y_1 = nondet();
    int Y_0 = nondet();
    int C_ = -1 + Y_1;
    if (2 + C >= 2 * Y_0 && 2 * Y_0 >= 1 + C && 1 + C == 0 && 1 + 2 * Y_1 >= 0 && 0 >= 2 * Y_1 && Y_1 >= 0 && 0 >= Y_0 && 0 >= 1) {
     C = C_;
     goto loc_evalrealheapsortbb3in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int C_ = -1 + Y_0;
    if (2 + C >= 2 * Y_0 && 2 * Y_0 >= 1 + C && 2 + C >= 2 * Y_1 && 2 * Y_1 >= 1 + C && 1 + C == 0 && 0 >= Y_0 && 0 >= Y_1 && 0 >= 1) {
     C = C_;
     goto loc_evalrealheapsortbb3in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int C_ = -1;
    if (2 + C >= 2 * Y_0 && 2 * Y_0 >= 1 + C && 1 + C == 0 && 1 + 2 * Y_1 >= 0 && 0 >= 2 * Y_1 && Y_1 >= 0 && 0 >= Y_0 && 0 >= 1) {
     C = C_;
     goto loc_evalrealheapsortbb3in;
    }
   }
   if (nondet_bool()) {
    int Y_1 = nondet();
    int Y_0 = nondet();
    int Y_2 = nondet();
    int C_ = -1 + Y_1;
    if (2 + C >= 2 * Y_0 && 2 * Y_0 >= 1 + C && 2 * Y_1 >= C && 1 + C >= 2 * Y_1 && Y_1 >= 0 && 2 * Y_2 >= C && 1 + C >= 2 * Y_2 && Y_2 >= 0 && C >= 0 && 0 >= Y_0 && 0 >= 2 + C) {
     C = C_;
     goto loc_evalrealheapsortbb3in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Y_2 = nondet();
    int C_ = -1 + Y_0;
    if (2 + C >= 2 * Y_0 && 2 * Y_0 >= 1 + C && 2 + C >= 2 * Y_1 && 2 * Y_1 >= 1 + C && 0 >= Y_0 && 2 * Y_2 >= C && 1 + C >= 2 * Y_2 && Y_2 >= 0 && C >= 0 && 0 >= Y_1 && 0 >= 2 + C) {
     C = C_;
     goto loc_evalrealheapsortbb3in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int C_ = -1;
    if (2 + C >= 2 * Y_0 && 2 * Y_0 >= 1 + C && 2 + C >= 2 * Y_1 && 2 * Y_1 >= 1 + C && 1 + C == 0 && 0 >= Y_0 && 0 >= Y_1 && 0 >= 1) {
     C = C_;
     goto loc_evalrealheapsortbb3in;
    }
   }
   if (nondet_bool()) {
    int Y_2 = nondet();
    int Y_0 = nondet();
    int Y_1 = nondet();
    int C_ = -1 + Y_2;
    if (2 + C >= 2 * Y_0 && 2 * Y_0 >= 1 + C && 2 + C >= 2 * Y_1 && 2 * Y_1 >= 1 + C && 2 * Y_2 >= C && 1 + C >= 2 * Y_2 && Y_2 >= 0 && C >= 0 && 0 >= Y_0 && 0 >= Y_1 && 0 >= 2 + C) {
     C = C_;
     goto loc_evalrealheapsortbb3in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Y_2 = nondet();
    int C_ = -1 + Y_0;
    if (2 + C >= 2 * Y_0 && 2 * Y_0 >= 1 + C && 2 + C >= 2 * Y_1 && 2 * Y_1 >= 1 + C && 2 + C >= 2 * Y_2 && 2 * Y_2 >= 1 + C && 0 >= Y_0 && 0 >= Y_1 && 0 >= Y_2 && 0 >= 2 + C) {
     C = C_;
     goto loc_evalrealheapsortbb3in;
    }
   }
  goto end;
 }
 loc_evalrealheapsortbb3in:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= C) {
     goto loc_evalrealheapsortbb5in;
    }
   }
   if (nondet_bool()) {
    if (C >= 1) {
     goto loc_evalrealheapsortbb4in;
    }
   }
  goto end;
 }
 loc_evalrealheapsortbb4in:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 + C == 0) {
     goto loc_evalrealheapsortbb2in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (2 * Y_0 >= C && 1 + C >= 2 * Y_0 && Y_0 >= 0 && C >= 0) {
     goto loc_evalrealheapsortbb2in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (2 + C >= 2 * Y_0 && 2 * Y_0 >= 1 + C && 0 >= Y_0 && 0 >= 2 + C) {
     goto loc_evalrealheapsortbb2in;
    }
   }
   if (nondet_bool()) {
    if (1 + C == 0) {
     goto loc_evalrealheapsortbb5in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (2 * Y_0 >= C && 1 + C >= 2 * Y_0 && Y_0 >= 0 && C >= 0) {
     goto loc_evalrealheapsortbb5in;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (2 + C >= 2 * Y_0 && 2 * Y_0 >= 1 + C && 0 >= Y_0 && 0 >= 2 + C) {
     goto loc_evalrealheapsortbb5in;
    }
   }
  goto end;
 }
 loc_evalrealheapsortbb5in:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (1 >= 0) {
     B = B_;
     goto loc_evalrealheapsortbb6in;
    }
   }
  goto end;
 }
 loc_evalrealheapsortbb6in:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = B;
    if (A >= 1 + B) {
     C = C_;
     goto loc_evalrealheapsortbb3in;
    }
   }
   if (nondet_bool()) {
    if (B >= A) {
     goto loc_evalrealheapsortbb7in;
    }
   }
  goto end;
 }
 loc_evalrealheapsortbb7in:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 0;
    if (1 >= 0) {
     B = B_;
     goto loc_evalrealheapsortbb18in;
    }
   }
  goto end;
 }
 loc_evalrealheapsortbb8in:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 0;
    if (1 >= 0) {
     C = C_;
     goto loc_evalrealheapsortbb16in;
    }
   }
  goto end;
 }
 loc_evalrealheapsortbb9in:
 {
 __cost++;
   if (nondet_bool()) {
    if (A == 3 + 2 * C + B) {
     goto loc_evalrealheapsortbb11in;
    }
   }
   if (nondet_bool()) {
    if (A >= 4 + B + 2 * C) {
     goto loc_evalrealheapsortbb10in;
    }
   }
   if (nondet_bool()) {
    if (2 + 2 * C + B >= A) {
     goto loc_evalrealheapsortbb10in;
    }
   }
  goto end;
 }
 loc_evalrealheapsortentryin:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1;
    if (A >= 3) {
     B = B_;
     goto loc_evalrealheapsortbb6in;
    }
   }
   if (nondet_bool()) {
    if (2 >= A) {
     goto loc_evalrealheapsortreturnin;
    }
   }
  goto end;
 }
 loc_evalrealheapsortreturnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalrealheapsortstop;
    }
   }
  goto end;
 }
 loc_evalrealheapsortstart:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalrealheapsortentryin;
    }
   }
  goto end;
 }
 loc_evalrealheapsortstop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
