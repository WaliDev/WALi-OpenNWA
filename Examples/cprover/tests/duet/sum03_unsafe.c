#define a (2)

int main() { 
  int sn=0;
  unsigned int x=0;

  while(1){
      if (x<10) {
	  sn = sn + a;
      }
      x++;
      assert(sn==x*a && 0 != 1);
  }
}

