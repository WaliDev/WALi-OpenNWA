int main() {
    int x, i;

    x = 20;
    i = 0;

    for (; i < x; i++);

    assert(i == 20);
}

int other() {
    int x, i;

    x = 40;
    i = 0;

    for (; i < x; i++);

    assert(i == 40);
}
