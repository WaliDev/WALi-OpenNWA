int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 0;
    int B_ = 2;
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f154;
    }
   }
  goto end;
 }
 loc_f154:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    int A_ = A + C;
    if (2 >= C) {
     A = A_;
     C = C_;
     goto loc_f154;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    if (C >= 3) {
     C = C_;
     goto loc_f160;
    }
   }
  goto end;
 }
 loc_f160:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    int A_ = A + C;
    if (3 >= C) {
     A = A_;
     C = C_;
     goto loc_f160;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    if (C >= 4) {
     C = C_;
     goto loc_f166;
    }
   }
  goto end;
 }
 loc_f166:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    int A_ = A + C;
    if (2 >= C) {
     A = A_;
     C = C_;
     goto loc_f166;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    if (C >= 3) {
     C = C_;
     goto loc_f172;
    }
   }
  goto end;
 }
 loc_f172:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    int A_ = A + C;
    if (3 >= C) {
     A = A_;
     C = C_;
     goto loc_f172;
    }
   }
   if (nondet_bool()) {
    int C_ = 1;
    if (C >= 4) {
     C = C_;
     goto loc_f180;
    }
   }
  goto end;
 }
 loc_f180:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    int A_ = A + C;
    if (1 >= C) {
     A = A_;
     C = C_;
     goto loc_f180;
    }
   }
   if (nondet_bool()) {
    int C_ = 1;
    if (C >= 2) {
     C = C_;
     goto loc_f186;
    }
   }
  goto end;
 }
 loc_f186:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    int A_ = A + C;
    if (2 >= C) {
     A = A_;
     C = C_;
     goto loc_f186;
    }
   }
   if (nondet_bool()) {
    int C_ = 1;
    if (C >= 3) {
     C = C_;
     goto loc_f192;
    }
   }
  goto end;
 }
 loc_f192:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    int A_ = A + C;
    if (1 >= C) {
     A = A_;
     C = C_;
     goto loc_f192;
    }
   }
   if (nondet_bool()) {
    int C_ = 1;
    if (C >= 2) {
     C = C_;
     goto loc_f198;
    }
   }
  goto end;
 }
 loc_f198:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    int A_ = A + C;
    if (2 >= C) {
     A = A_;
     C = C_;
     goto loc_f198;
    }
   }
   if (nondet_bool()) {
    int C_ = -3;
    if (C >= 3) {
     C = C_;
     goto loc_f206;
    }
   }
  goto end;
 }
 loc_f206:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    int A_ = A + C;
    if (0 >= 3 + C) {
     A = A_;
     C = C_;
     goto loc_f206;
    }
   }
   if (nondet_bool()) {
    int C_ = -3;
    if (2 + C >= 0) {
     C = C_;
     goto loc_f212;
    }
   }
  goto end;
 }
 loc_f212:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    int A_ = A + C;
    if (0 >= 2 + C) {
     A = A_;
     C = C_;
     goto loc_f212;
    }
   }
   if (nondet_bool()) {
    int C_ = -3;
    if (1 + C >= 0) {
     C = C_;
     goto loc_f218;
    }
   }
  goto end;
 }
 loc_f218:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    int A_ = A + C;
    if (0 >= 3 + C) {
     A = A_;
     C = C_;
     goto loc_f218;
    }
   }
   if (nondet_bool()) {
    int C_ = -3;
    if (2 + C >= 0) {
     C = C_;
     goto loc_f224;
    }
   }
  goto end;
 }
 loc_f224:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    int A_ = A + C;
    if (0 >= 2 + C) {
     A = A_;
     C = C_;
     goto loc_f224;
    }
   }
   if (nondet_bool()) {
    int C_ = -4;
    if (1 + C >= 0) {
     C = C_;
     goto loc_f232;
    }
   }
  goto end;
 }
 loc_f232:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    int A_ = A + C;
    if (0 >= 2 + C) {
     A = A_;
     C = C_;
     goto loc_f232;
    }
   }
   if (nondet_bool()) {
    int C_ = -4;
    if (1 + C >= 0) {
     C = C_;
     goto loc_f238;
    }
   }
  goto end;
 }
 loc_f238:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    int A_ = A + C;
    if (0 >= 1 + C) {
     A = A_;
     C = C_;
     goto loc_f238;
    }
   }
   if (nondet_bool()) {
    int C_ = -4;
    if (C >= 0) {
     C = C_;
     goto loc_f244;
    }
   }
  goto end;
 }
 loc_f244:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    int A_ = A + C;
    if (0 >= 2 + C) {
     A = A_;
     C = C_;
     goto loc_f244;
    }
   }
   if (nondet_bool()) {
    int C_ = -4;
    if (1 + C >= 0) {
     C = C_;
     goto loc_f250;
    }
   }
  goto end;
 }
 loc_f250:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    int A_ = A + C;
    if (0 >= 1 + C) {
     A = A_;
     C = C_;
     goto loc_f250;
    }
   }
   if (nondet_bool()) {
    int C_ = -5;
    if (C >= 0) {
     C = C_;
     goto loc_f258;
    }
   }
  goto end;
 }
 loc_f258:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    int A_ = A + C;
    if (0 >= 1 + C) {
     A = A_;
     C = C_;
     goto loc_f258;
    }
   }
   if (nondet_bool()) {
    int C_ = -5;
    if (C >= 0) {
     C = C_;
     goto loc_f264;
    }
   }
  goto end;
 }
 loc_f264:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    int A_ = A + C;
    if (0 >= C) {
     A = A_;
     C = C_;
     goto loc_f264;
    }
   }
   if (nondet_bool()) {
    int C_ = -5;
    if (C >= 1) {
     C = C_;
     goto loc_f270;
    }
   }
  goto end;
 }
 loc_f270:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    int A_ = A + C;
    if (0 >= 1 + C) {
     A = A_;
     C = C_;
     goto loc_f270;
    }
   }
   if (nondet_bool()) {
    int C_ = -5;
    if (C >= 0) {
     C = C_;
     goto loc_f276;
    }
   }
  goto end;
 }
 loc_f276:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    int A_ = A + C;
    if (0 >= C) {
     A = A_;
     C = C_;
     goto loc_f276;
    }
   }
   if (nondet_bool()) {
    int C_ = -6;
    if (C >= 1) {
     C = C_;
     goto loc_f284;
    }
   }
  goto end;
 }
 loc_f284:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    int A_ = A + C;
    if (3 >= C) {
     A = A_;
     C = C_;
     goto loc_f284;
    }
   }
   if (nondet_bool()) {
    int C_ = -6;
    if (C >= 4) {
     C = C_;
     goto loc_f290;
    }
   }
  goto end;
 }
 loc_f290:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    int A_ = A + C;
    if (4 >= C) {
     A = A_;
     C = C_;
     goto loc_f290;
    }
   }
   if (nondet_bool()) {
    int C_ = -6;
    if (C >= 5) {
     C = C_;
     goto loc_f296;
    }
   }
  goto end;
 }
 loc_f296:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    int A_ = A + C;
    if (3 >= C) {
     A = A_;
     C = C_;
     goto loc_f296;
    }
   }
   if (nondet_bool()) {
    int C_ = -6;
    if (C >= 4) {
     C = C_;
     goto loc_f302;
    }
   }
  goto end;
 }
 loc_f302:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    int A_ = A + C;
    if (4 >= C) {
     A = A_;
     C = C_;
     goto loc_f302;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    if (C >= 5) {
     C = C_;
     goto loc_f310;
    }
   }
  goto end;
 }
 loc_f310:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + B;
    int A_ = A + C;
    if (2 >= C) {
     A = A_;
     C = C_;
     goto loc_f310;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    if (C >= 3) {
     C = C_;
     goto loc_f316;
    }
   }
  goto end;
 }
 loc_f316:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + B;
    int A_ = A + C;
    if (3 >= C) {
     A = A_;
     C = C_;
     goto loc_f316;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    if (C >= 4) {
     C = C_;
     goto loc_f322;
    }
   }
  goto end;
 }
 loc_f322:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + B;
    int A_ = A + C;
    if (2 >= C) {
     A = A_;
     C = C_;
     goto loc_f322;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    if (C >= 3) {
     C = C_;
     goto loc_f328;
    }
   }
  goto end;
 }
 loc_f328:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + B;
    int A_ = A + C;
    if (3 >= C) {
     A = A_;
     C = C_;
     goto loc_f328;
    }
   }
   if (nondet_bool()) {
    int C_ = 1;
    if (C >= 4) {
     C = C_;
     goto loc_f336;
    }
   }
  goto end;
 }
 loc_f336:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + B;
    int A_ = A + C;
    if (1 >= C) {
     A = A_;
     C = C_;
     goto loc_f336;
    }
   }
   if (nondet_bool()) {
    int C_ = 1;
    if (C >= 2) {
     C = C_;
     goto loc_f342;
    }
   }
  goto end;
 }
 loc_f342:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + B;
    int A_ = A + C;
    if (2 >= C) {
     A = A_;
     C = C_;
     goto loc_f342;
    }
   }
   if (nondet_bool()) {
    int C_ = 1;
    if (C >= 3) {
     C = C_;
     goto loc_f348;
    }
   }
  goto end;
 }
 loc_f348:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + B;
    int A_ = A + C;
    if (1 >= C) {
     A = A_;
     C = C_;
     goto loc_f348;
    }
   }
   if (nondet_bool()) {
    int C_ = 1;
    if (C >= 2) {
     C = C_;
     goto loc_f354;
    }
   }
  goto end;
 }
 loc_f354:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + B;
    int A_ = A + C;
    if (2 >= C) {
     A = A_;
     C = C_;
     goto loc_f354;
    }
   }
   if (nondet_bool()) {
    int C_ = -3;
    if (C >= 3) {
     C = C_;
     goto loc_f362;
    }
   }
  goto end;
 }
 loc_f362:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + B;
    int A_ = A + C;
    if (0 >= 3 + C) {
     A = A_;
     C = C_;
     goto loc_f362;
    }
   }
   if (nondet_bool()) {
    int C_ = -3;
    if (2 + C >= 0) {
     C = C_;
     goto loc_f368;
    }
   }
  goto end;
 }
 loc_f368:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + B;
    int A_ = A + C;
    if (0 >= 2 + C) {
     A = A_;
     C = C_;
     goto loc_f368;
    }
   }
   if (nondet_bool()) {
    int C_ = -3;
    if (1 + C >= 0) {
     C = C_;
     goto loc_f374;
    }
   }
  goto end;
 }
 loc_f374:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + B;
    int A_ = A + C;
    if (0 >= 3 + C) {
     A = A_;
     C = C_;
     goto loc_f374;
    }
   }
   if (nondet_bool()) {
    int C_ = -3;
    if (2 + C >= 0) {
     C = C_;
     goto loc_f380;
    }
   }
  goto end;
 }
 loc_f380:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + B;
    int A_ = A + C;
    if (0 >= 2 + C) {
     A = A_;
     C = C_;
     goto loc_f380;
    }
   }
   if (nondet_bool()) {
    int C_ = -4;
    if (1 + C >= 0) {
     C = C_;
     goto loc_f388;
    }
   }
  goto end;
 }
 loc_f388:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + B;
    int A_ = A + C;
    if (0 >= 2 + C) {
     A = A_;
     C = C_;
     goto loc_f388;
    }
   }
   if (nondet_bool()) {
    int C_ = -4;
    if (1 + C >= 0) {
     C = C_;
     goto loc_f394;
    }
   }
  goto end;
 }
 loc_f394:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + B;
    int A_ = A + C;
    if (0 >= 1 + C) {
     A = A_;
     C = C_;
     goto loc_f394;
    }
   }
   if (nondet_bool()) {
    int C_ = -4;
    if (C >= 0) {
     C = C_;
     goto loc_f400;
    }
   }
  goto end;
 }
 loc_f400:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + B;
    int A_ = A + C;
    if (0 >= 2 + C) {
     A = A_;
     C = C_;
     goto loc_f400;
    }
   }
   if (nondet_bool()) {
    int C_ = -4;
    if (1 + C >= 0) {
     C = C_;
     goto loc_f406;
    }
   }
  goto end;
 }
 loc_f406:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + B;
    int A_ = A + C;
    if (0 >= 1 + C) {
     A = A_;
     C = C_;
     goto loc_f406;
    }
   }
   if (nondet_bool()) {
    int C_ = -5;
    if (C >= 0) {
     C = C_;
     goto loc_f414;
    }
   }
  goto end;
 }
 loc_f414:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + B;
    int A_ = A + C;
    if (0 >= 1 + C) {
     A = A_;
     C = C_;
     goto loc_f414;
    }
   }
   if (nondet_bool()) {
    int C_ = -5;
    if (C >= 0) {
     C = C_;
     goto loc_f420;
    }
   }
  goto end;
 }
 loc_f420:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + B;
    int A_ = A + C;
    if (0 >= C) {
     A = A_;
     C = C_;
     goto loc_f420;
    }
   }
   if (nondet_bool()) {
    int C_ = -5;
    if (C >= 1) {
     C = C_;
     goto loc_f426;
    }
   }
  goto end;
 }
 loc_f426:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + B;
    int A_ = A + C;
    if (0 >= 1 + C) {
     A = A_;
     C = C_;
     goto loc_f426;
    }
   }
   if (nondet_bool()) {
    int C_ = -5;
    if (C >= 0) {
     C = C_;
     goto loc_f432;
    }
   }
  goto end;
 }
 loc_f432:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + B;
    int A_ = A + C;
    if (0 >= C) {
     A = A_;
     C = C_;
     goto loc_f432;
    }
   }
   if (nondet_bool()) {
    int C_ = -6;
    if (C >= 1) {
     C = C_;
     goto loc_f440;
    }
   }
  goto end;
 }
 loc_f440:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + B;
    int A_ = A + C;
    if (3 >= C) {
     A = A_;
     C = C_;
     goto loc_f440;
    }
   }
   if (nondet_bool()) {
    int C_ = -6;
    if (C >= 4) {
     C = C_;
     goto loc_f446;
    }
   }
  goto end;
 }
 loc_f446:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + B;
    int A_ = A + C;
    if (4 >= C) {
     A = A_;
     C = C_;
     goto loc_f446;
    }
   }
   if (nondet_bool()) {
    int C_ = -6;
    if (C >= 5) {
     C = C_;
     goto loc_f452;
    }
   }
  goto end;
 }
 loc_f452:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + B;
    int A_ = A + C;
    if (3 >= C) {
     A = A_;
     C = C_;
     goto loc_f452;
    }
   }
   if (nondet_bool()) {
    int C_ = -6;
    if (C >= 4) {
     C = C_;
     goto loc_f458;
    }
   }
  goto end;
 }
 loc_f458:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + B;
    int A_ = A + C;
    if (4 >= C) {
     A = A_;
     C = C_;
     goto loc_f458;
    }
   }
   if (nondet_bool()) {
    int C_ = 5;
    if (C >= 5) {
     C = C_;
     goto loc_f466;
    }
   }
  goto end;
 }
 loc_f466:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (C >= 3) {
     A = A_;
     C = C_;
     goto loc_f466;
    }
   }
   if (nondet_bool()) {
    int C_ = 5;
    if (2 >= C) {
     C = C_;
     goto loc_f472;
    }
   }
  goto end;
 }
 loc_f472:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (C >= 2) {
     A = A_;
     C = C_;
     goto loc_f472;
    }
   }
   if (nondet_bool()) {
    int C_ = 5;
    if (1 >= C) {
     C = C_;
     goto loc_f478;
    }
   }
  goto end;
 }
 loc_f478:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (C >= 3) {
     A = A_;
     C = C_;
     goto loc_f478;
    }
   }
   if (nondet_bool()) {
    int C_ = 5;
    if (2 >= C) {
     C = C_;
     goto loc_f484;
    }
   }
  goto end;
 }
 loc_f484:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (C >= 2) {
     A = A_;
     C = C_;
     goto loc_f484;
    }
   }
   if (nondet_bool()) {
    int C_ = 6;
    if (1 >= C) {
     C = C_;
     goto loc_f492;
    }
   }
  goto end;
 }
 loc_f492:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (C >= 2) {
     A = A_;
     C = C_;
     goto loc_f492;
    }
   }
   if (nondet_bool()) {
    int C_ = 6;
    if (1 >= C) {
     C = C_;
     goto loc_f498;
    }
   }
  goto end;
 }
 loc_f498:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (C >= 1) {
     A = A_;
     C = C_;
     goto loc_f498;
    }
   }
   if (nondet_bool()) {
    int C_ = 6;
    if (0 >= C) {
     C = C_;
     goto loc_f504;
    }
   }
  goto end;
 }
 loc_f504:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (C >= 2) {
     A = A_;
     C = C_;
     goto loc_f504;
    }
   }
   if (nondet_bool()) {
    int C_ = 6;
    if (1 >= C) {
     C = C_;
     goto loc_f510;
    }
   }
  goto end;
 }
 loc_f510:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (C >= 1) {
     A = A_;
     C = C_;
     goto loc_f510;
    }
   }
   if (nondet_bool()) {
    int C_ = 7;
    if (0 >= C) {
     C = C_;
     goto loc_f518;
    }
   }
  goto end;
 }
 loc_f518:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (C >= 1) {
     A = A_;
     C = C_;
     goto loc_f518;
    }
   }
   if (nondet_bool()) {
    int C_ = 7;
    if (0 >= C) {
     C = C_;
     goto loc_f524;
    }
   }
  goto end;
 }
 loc_f524:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (C >= 0) {
     A = A_;
     C = C_;
     goto loc_f524;
    }
   }
   if (nondet_bool()) {
    int C_ = 7;
    if (0 >= 1 + C) {
     C = C_;
     goto loc_f530;
    }
   }
  goto end;
 }
 loc_f530:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (C >= 1) {
     A = A_;
     C = C_;
     goto loc_f530;
    }
   }
   if (nondet_bool()) {
    int C_ = 7;
    if (0 >= C) {
     C = C_;
     goto loc_f536;
    }
   }
  goto end;
 }
 loc_f536:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (C >= 0) {
     A = A_;
     C = C_;
     goto loc_f536;
    }
   }
   if (nondet_bool()) {
    int C_ = 8;
    if (0 >= 1 + C) {
     C = C_;
     goto loc_f544;
    }
   }
  goto end;
 }
 loc_f544:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (C >= 0) {
     A = A_;
     C = C_;
     goto loc_f544;
    }
   }
   if (nondet_bool()) {
    int C_ = 8;
    if (0 >= 1 + C) {
     C = C_;
     goto loc_f550;
    }
   }
  goto end;
 }
 loc_f550:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (1 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f550;
    }
   }
   if (nondet_bool()) {
    int C_ = 8;
    if (0 >= 2 + C) {
     C = C_;
     goto loc_f556;
    }
   }
  goto end;
 }
 loc_f556:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (C >= 0) {
     A = A_;
     C = C_;
     goto loc_f556;
    }
   }
   if (nondet_bool()) {
    int C_ = 8;
    if (0 >= 1 + C) {
     C = C_;
     goto loc_f562;
    }
   }
  goto end;
 }
 loc_f562:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (1 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f562;
    }
   }
   if (nondet_bool()) {
    int C_ = 9;
    if (0 >= 2 + C) {
     C = C_;
     goto loc_f570;
    }
   }
  goto end;
 }
 loc_f570:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (1 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f570;
    }
   }
   if (nondet_bool()) {
    int C_ = 9;
    if (0 >= 2 + C) {
     C = C_;
     goto loc_f576;
    }
   }
  goto end;
 }
 loc_f576:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (2 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f576;
    }
   }
   if (nondet_bool()) {
    int C_ = 9;
    if (0 >= 3 + C) {
     C = C_;
     goto loc_f582;
    }
   }
  goto end;
 }
 loc_f582:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (1 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f582;
    }
   }
   if (nondet_bool()) {
    int C_ = 9;
    if (0 >= 2 + C) {
     C = C_;
     goto loc_f588;
    }
   }
  goto end;
 }
 loc_f588:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (2 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f588;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    if (0 >= 3 + C) {
     C = C_;
     goto loc_f596;
    }
   }
  goto end;
 }
 loc_f596:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (2 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f596;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    if (0 >= 3 + C) {
     C = C_;
     goto loc_f602;
    }
   }
  goto end;
 }
 loc_f602:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (3 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f602;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    if (0 >= 4 + C) {
     C = C_;
     goto loc_f608;
    }
   }
  goto end;
 }
 loc_f608:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (2 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f608;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    if (0 >= 3 + C) {
     C = C_;
     goto loc_f614;
    }
   }
  goto end;
 }
 loc_f614:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (3 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f614;
    }
   }
   if (nondet_bool()) {
    int C_ = -1;
    if (0 >= 4 + C) {
     C = C_;
     goto loc_f622;
    }
   }
  goto end;
 }
 loc_f622:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (4 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f622;
    }
   }
   if (nondet_bool()) {
    int C_ = -1;
    if (0 >= 5 + C) {
     C = C_;
     goto loc_f628;
    }
   }
  goto end;
 }
 loc_f628:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (5 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f628;
    }
   }
   if (nondet_bool()) {
    int C_ = -1;
    if (0 >= 6 + C) {
     C = C_;
     goto loc_f634;
    }
   }
  goto end;
 }
 loc_f634:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (4 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f634;
    }
   }
   if (nondet_bool()) {
    int C_ = -1;
    if (0 >= 5 + C) {
     C = C_;
     goto loc_f640;
    }
   }
  goto end;
 }
 loc_f640:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (5 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f640;
    }
   }
   if (nondet_bool()) {
    int C_ = -2;
    if (0 >= 6 + C) {
     C = C_;
     goto loc_f648;
    }
   }
  goto end;
 }
 loc_f648:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (6 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f648;
    }
   }
   if (nondet_bool()) {
    int C_ = -2;
    if (0 >= 7 + C) {
     C = C_;
     goto loc_f654;
    }
   }
  goto end;
 }
 loc_f654:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (7 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f654;
    }
   }
   if (nondet_bool()) {
    int C_ = -2;
    if (0 >= 8 + C) {
     C = C_;
     goto loc_f660;
    }
   }
  goto end;
 }
 loc_f660:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (6 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f660;
    }
   }
   if (nondet_bool()) {
    int C_ = -2;
    if (0 >= 7 + C) {
     C = C_;
     goto loc_f666;
    }
   }
  goto end;
 }
 loc_f666:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (7 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f666;
    }
   }
   if (nondet_bool()) {
    int C_ = 16;
    if (0 >= 8 + C) {
     C = C_;
     goto loc_f674;
    }
   }
  goto end;
 }
 loc_f674:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (7 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f674;
    }
   }
   if (nondet_bool()) {
    int C_ = 16;
    if (0 >= 8 + C) {
     C = C_;
     goto loc_f680;
    }
   }
  goto end;
 }
 loc_f680:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (8 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f680;
    }
   }
   if (nondet_bool()) {
    int C_ = 16;
    if (0 >= 9 + C) {
     C = C_;
     goto loc_f686;
    }
   }
  goto end;
 }
 loc_f686:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (7 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f686;
    }
   }
   if (nondet_bool()) {
    int C_ = 16;
    if (0 >= 8 + C) {
     C = C_;
     goto loc_f692;
    }
   }
  goto end;
 }
 loc_f692:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (8 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f692;
    }
   }
   if (nondet_bool()) {
    int C_ = 5;
    if (0 >= 9 + C) {
     C = C_;
     goto loc_f700;
    }
   }
  goto end;
 }
 loc_f700:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (C >= 3) {
     A = A_;
     C = C_;
     goto loc_f700;
    }
   }
   if (nondet_bool()) {
    int C_ = 5;
    if (2 >= C) {
     C = C_;
     goto loc_f706;
    }
   }
  goto end;
 }
 loc_f706:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (C >= 2) {
     A = A_;
     C = C_;
     goto loc_f706;
    }
   }
   if (nondet_bool()) {
    int C_ = 5;
    if (1 >= C) {
     C = C_;
     goto loc_f712;
    }
   }
  goto end;
 }
 loc_f712:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (C >= 3) {
     A = A_;
     C = C_;
     goto loc_f712;
    }
   }
   if (nondet_bool()) {
    int C_ = 5;
    if (2 >= C) {
     C = C_;
     goto loc_f718;
    }
   }
  goto end;
 }
 loc_f718:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (C >= 2) {
     A = A_;
     C = C_;
     goto loc_f718;
    }
   }
   if (nondet_bool()) {
    int C_ = 6;
    if (1 >= C) {
     C = C_;
     goto loc_f726;
    }
   }
  goto end;
 }
 loc_f726:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (C >= 2) {
     A = A_;
     C = C_;
     goto loc_f726;
    }
   }
   if (nondet_bool()) {
    int C_ = 6;
    if (1 >= C) {
     C = C_;
     goto loc_f732;
    }
   }
  goto end;
 }
 loc_f732:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (C >= 1) {
     A = A_;
     C = C_;
     goto loc_f732;
    }
   }
   if (nondet_bool()) {
    int C_ = 6;
    if (0 >= C) {
     C = C_;
     goto loc_f738;
    }
   }
  goto end;
 }
 loc_f738:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (C >= 2) {
     A = A_;
     C = C_;
     goto loc_f738;
    }
   }
   if (nondet_bool()) {
    int C_ = 6;
    if (1 >= C) {
     C = C_;
     goto loc_f744;
    }
   }
  goto end;
 }
 loc_f744:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (C >= 1) {
     A = A_;
     C = C_;
     goto loc_f744;
    }
   }
   if (nondet_bool()) {
    int C_ = 7;
    if (0 >= C) {
     C = C_;
     goto loc_f752;
    }
   }
  goto end;
 }
 loc_f752:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (C >= 1) {
     A = A_;
     C = C_;
     goto loc_f752;
    }
   }
   if (nondet_bool()) {
    int C_ = 7;
    if (0 >= C) {
     C = C_;
     goto loc_f758;
    }
   }
  goto end;
 }
 loc_f758:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (C >= 0) {
     A = A_;
     C = C_;
     goto loc_f758;
    }
   }
   if (nondet_bool()) {
    int C_ = 7;
    if (0 >= 1 + C) {
     C = C_;
     goto loc_f764;
    }
   }
  goto end;
 }
 loc_f764:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (C >= 1) {
     A = A_;
     C = C_;
     goto loc_f764;
    }
   }
   if (nondet_bool()) {
    int C_ = 7;
    if (0 >= C) {
     C = C_;
     goto loc_f770;
    }
   }
  goto end;
 }
 loc_f770:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (C >= 0) {
     A = A_;
     C = C_;
     goto loc_f770;
    }
   }
   if (nondet_bool()) {
    int C_ = 8;
    if (0 >= 1 + C) {
     C = C_;
     goto loc_f778;
    }
   }
  goto end;
 }
 loc_f778:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (C >= 0) {
     A = A_;
     C = C_;
     goto loc_f778;
    }
   }
   if (nondet_bool()) {
    int C_ = 8;
    if (0 >= 1 + C) {
     C = C_;
     goto loc_f784;
    }
   }
  goto end;
 }
 loc_f784:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (1 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f784;
    }
   }
   if (nondet_bool()) {
    int C_ = 8;
    if (0 >= 2 + C) {
     C = C_;
     goto loc_f790;
    }
   }
  goto end;
 }
 loc_f790:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (C >= 0) {
     A = A_;
     C = C_;
     goto loc_f790;
    }
   }
   if (nondet_bool()) {
    int C_ = 8;
    if (0 >= 1 + C) {
     C = C_;
     goto loc_f796;
    }
   }
  goto end;
 }
 loc_f796:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (1 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f796;
    }
   }
   if (nondet_bool()) {
    int C_ = 9;
    if (0 >= 2 + C) {
     C = C_;
     goto loc_f804;
    }
   }
  goto end;
 }
 loc_f804:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (1 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f804;
    }
   }
   if (nondet_bool()) {
    int C_ = 9;
    if (0 >= 2 + C) {
     C = C_;
     goto loc_f810;
    }
   }
  goto end;
 }
 loc_f810:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (2 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f810;
    }
   }
   if (nondet_bool()) {
    int C_ = 9;
    if (0 >= 3 + C) {
     C = C_;
     goto loc_f816;
    }
   }
  goto end;
 }
 loc_f816:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (1 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f816;
    }
   }
   if (nondet_bool()) {
    int C_ = 9;
    if (0 >= 2 + C) {
     C = C_;
     goto loc_f822;
    }
   }
  goto end;
 }
 loc_f822:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (2 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f822;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    if (0 >= 3 + C) {
     C = C_;
     goto loc_f830;
    }
   }
  goto end;
 }
 loc_f830:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (2 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f830;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    if (0 >= 3 + C) {
     C = C_;
     goto loc_f836;
    }
   }
  goto end;
 }
 loc_f836:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (3 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f836;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    if (0 >= 4 + C) {
     C = C_;
     goto loc_f842;
    }
   }
  goto end;
 }
 loc_f842:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (2 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f842;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    if (0 >= 3 + C) {
     C = C_;
     goto loc_f848;
    }
   }
  goto end;
 }
 loc_f848:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (3 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f848;
    }
   }
   if (nondet_bool()) {
    int C_ = -1;
    if (0 >= 4 + C) {
     C = C_;
     goto loc_f856;
    }
   }
  goto end;
 }
 loc_f856:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (4 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f856;
    }
   }
   if (nondet_bool()) {
    int C_ = -1;
    if (0 >= 5 + C) {
     C = C_;
     goto loc_f862;
    }
   }
  goto end;
 }
 loc_f862:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (5 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f862;
    }
   }
   if (nondet_bool()) {
    int C_ = -1;
    if (0 >= 6 + C) {
     C = C_;
     goto loc_f868;
    }
   }
  goto end;
 }
 loc_f868:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (4 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f868;
    }
   }
   if (nondet_bool()) {
    int C_ = -1;
    if (0 >= 5 + C) {
     C = C_;
     goto loc_f874;
    }
   }
  goto end;
 }
 loc_f874:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (5 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f874;
    }
   }
   if (nondet_bool()) {
    int C_ = -2;
    if (0 >= 6 + C) {
     C = C_;
     goto loc_f882;
    }
   }
  goto end;
 }
 loc_f882:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (6 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f882;
    }
   }
   if (nondet_bool()) {
    int C_ = -2;
    if (0 >= 7 + C) {
     C = C_;
     goto loc_f888;
    }
   }
  goto end;
 }
 loc_f888:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (7 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f888;
    }
   }
   if (nondet_bool()) {
    int C_ = -2;
    if (0 >= 8 + C) {
     C = C_;
     goto loc_f894;
    }
   }
  goto end;
 }
 loc_f894:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (6 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f894;
    }
   }
   if (nondet_bool()) {
    int C_ = -2;
    if (0 >= 7 + C) {
     C = C_;
     goto loc_f900;
    }
   }
  goto end;
 }
 loc_f900:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (7 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f900;
    }
   }
   if (nondet_bool()) {
    int C_ = 16;
    if (0 >= 8 + C) {
     C = C_;
     goto loc_f908;
    }
   }
  goto end;
 }
 loc_f908:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (7 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f908;
    }
   }
   if (nondet_bool()) {
    int C_ = 16;
    if (0 >= 8 + C) {
     C = C_;
     goto loc_f914;
    }
   }
  goto end;
 }
 loc_f914:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (8 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f914;
    }
   }
   if (nondet_bool()) {
    int C_ = 16;
    if (0 >= 9 + C) {
     C = C_;
     goto loc_f920;
    }
   }
  goto end;
 }
 loc_f920:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (7 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f920;
    }
   }
   if (nondet_bool()) {
    int C_ = 16;
    if (0 >= 8 + C) {
     C = C_;
     goto loc_f926;
    }
   }
  goto end;
 }
 loc_f926:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = C + -B;
    int A_ = A + C;
    if (8 + C >= 0) {
     A = A_;
     C = C_;
     goto loc_f926;
    }
   }
   if (nondet_bool()) {
    if (0 >= 9 + C) {
     goto loc_f934;
    }
   }
  goto end;
 }
 loc_f934:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
