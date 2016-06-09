void main()
{
    int x=rand();
    int y=rand();
    int z=rand();

    while(x<100 && 100<z) {
	int tmp=rand();
	if (tmp) {
	    x++;
	} else {
	    x--;
	    z--;
	}
    }                 
    
    assert(0);    
}


