
/* from the Diploma Thesis of Michael Petter, available at http://www2.cs.tum.edu/~petter/ */

geo (var k)
{
  var x,y;

  x = 0;
  y = 0;

  while( y != k )
     {
     x = x + y^5;
     y = y + 1;
     }
}
