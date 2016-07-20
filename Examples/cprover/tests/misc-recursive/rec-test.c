int i = 0;

void loop() {
    if (i >= 50)
        return;
    else {
       i++;
       loop();
    }
}

void main() {
   loop();
   assert(i==50);
}
