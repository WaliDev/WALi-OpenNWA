int x;

void p(int v) {

  if (v <= 0) return; 
  
  x = x + 1;
  p(v-1);
  x = x - 1;
  
  x = x + 1;
  p(v-2);
  x = x - 1;

}

int main() {
  x = 5;
  p(9);
}
