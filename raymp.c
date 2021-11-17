//
// Whitley, 8 February 2019
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//
#define M 1920
#define N 1080
#define DEPTHLIMIT 5
//
typedef struct
{
   double x ;
   double y ;
   double z ;
   //
} triple ;
//
typedef struct
{
   int r ;
   int g ;
   int b ;
   //
} color ;
//
typedef struct
{
  triple c;
  double r;
  color h;
  //
} sphere ;

//
sphere a[4];
triple e = { 0.50 , 0.50 , -1.00 } ; // the eye
triple g = { 0.00 , 1.25 , -0.50 } ; // the light
//
double dotp( triple t , triple u )
{
   return t.x * u.x + t.y * u.y + t.z * u.z ;
}
//
void diff( triple* t , triple u , triple v ) // t = u - v
{
   t->x = u.x - v.x ;
   t->y = u.y - v.y ;
   t->z = u.z - v.z ;
}
//
void sum( triple* t , triple u , triple v ) // t = u + v
{
   t->x = u.x + v.x ;
   t->y = u.y + v.y ;
   t->z = u.z + v.z ;
}
//
double min( double a, double b){
  if( a < b) return a;
  else return b;
}
//
void scalar( triple* t , triple u , double s ) // t = u * s
{
   t->x = u.x * s ;
   t->y = u.y * s ;
   t->z = u.z * s ;
}
//
double mag( triple r )
{
   return sqrt((r.x * r.x) + (r.y * r.y) + (r.z * r.z));
}
//
void normalize( triple* t , triple u ) // t = u - v
{
   double uMag = mag(u);
   t->x = u.x / uMag;
   t->y = u.y / uMag;
   t->z = u.z / uMag;
}
//
color recursion(triple p, triple r, int depth)
{

  color objectC, reflectionC ;
  int s , sphereHit , shadowBool, red, green, blue ;
  double b , c , t , t1 , t2 , rmag, smag , minT, gradient, nmag ;
  triple intersect , normS , gVect, reflection, d, sVect ;
  //
  sphereHit = -1 ;
  shadowBool = 0 ;
  minT = 99999999999999999999999999999999999.9999999999;
  rmag = mag(r) ; // normalize r
  r.x = r.x / rmag;
  r.y = r.y / rmag;
  r.z = r.z / rmag;
  for( s = 0; s < 4; s++) // loop through spheres
  {
    diff(&d , p, a[s].c);
    //
    b = 2.0 * dotp(d , r) ;
    c = (d.x * d.x) + (d.y * d.y) + (d.z * d.z) - (a[s].r * a[s].r) ;
    //
    t1 = ( (b * -1) - sqrt((b * b) - (4 * c)) ) / 2.0 ;
    t2 = ( (b * -1) + sqrt((b * b) - (4 * c)) ) / 2.0 ;
    t = min(t1, t2);
    //
    if( ((b * b) - 4 * c) >= 0 && t < minT && t >= 0){
      sphereHit = s;
      minT = t;
    }
  }
  //
  if( sphereHit < 0 ){
      objectC.r = -1 ;
      objectC.g = -1 ;
      objectC.b = -1 ;
      return objectC ;
   }
   // shadows
   scalar(&intersect , r , minT) ;
   sum(&intersect , p, intersect);
   //
   diff(&sVect , g , intersect);
   smag = mag(sVect) ; // normalize r
   sVect.x = sVect.x / smag ;
   sVect.y = sVect.y / smag ;
   sVect.z = sVect.z / smag ;
   //
   if (sphereHit == 0){
      if ( ( (int)((intersect.x+1) / 0.1) + (int)((intersect.z+1) / 0.1) ) % 2 == 0 ){
        red = a[sphereHit].h.r ;
        green = a[sphereHit].h.g ;
        blue = a[sphereHit].h.b ;
      }
      else{
        red = 120 ;
        green = 81  ;
        blue = 169 ;
      }
   }
   //
   else{
      red = a[sphereHit].h.r ;
      green = a[sphereHit].h.g ;
      blue = a[sphereHit].h.b ;
   }
   //
   for( s = 0; s < 4; s++) // loop through spheres
   {
      diff(&d , intersect , a[s].c);
      //
      b = 2.0 * dotp(d , sVect) ;
      c = (d.x * d.x) + (d.y * d.y) + (d.z * d.z) - (a[s].r * a[s].r) ;
      //
      t1 = ( (b * -1) - sqrt((b * b) - (4 * c)) ) / 2.0 ;
      t2 = ( (b * -1) + sqrt((b * b) - (4 * c)) ) / 2.0 ;
      t = min(t1, t2);
      //
      if( ((b * b) - 4 * c) && t >= 0.001){
        shadowBool = 1;
        break;
      }
   }
   //
   diff(&gVect, g , intersect) ;
   diff(&normS, intersect , a[sphereHit].c ) ;
   nmag = mag(normS) ; // normalize normS
   normS.x = normS.x / nmag;
   normS.y = normS.y / nmag;
   normS.z = normS.z / nmag;
   gradient = dotp(normS,gVect) / ( mag(gVect) * mag(normS) );
   if(shadowBool){
      red = red / 2 ;
      green = green / 2 ;
      blue = blue / 2 ;
   }
   //
   else{
      if ( gradient < 0 ) gradient = 0 ;
      red = red * 0.5 + (red * 0.5 * gradient ) ;
      green = green * 0.5 + (green * 0.5 * gradient ) ;
      blue = blue * 0.5 + (blue * 0.5 * gradient ) ;
  }
  if( depth == DEPTHLIMIT ){
  }
  else{
    /*intersect.x += normS.x * 0.00001 ;
    intersect.y += normS.y * 0.00001 ;
    intersect.z += normS.z * 0.00001 ; */
    scalar(&reflection, normS, -2 * dotp(r , normS) ) ;
    sum(&reflection, reflection , r ) ;
    reflectionC = recursion(intersect, reflection, depth + 1) ;
    if( reflectionC.r != -1 ){
      red = red * 0.5 + (reflectionC.r * 0.5 ) ;
      green = green * 0.5 + (reflectionC.g * 0.5) ;
      blue = blue * 0.5 + (reflectionC.b * 0.5 ) ;
    }
  }
  //
  objectC.r = red ;
  objectC.g = green ;
  objectC.b = blue ;
  return objectC ;
}
//
void init()
{
   a[0].c.x =      0.50 ;
   a[0].c.y = -20000.00 ; // the floor
   a[0].c.z =      0.50 ;
   a[0].r   =  20000.25 ;
   a[0].h.r =    205    ; // color is Peru
   a[0].h.g =    133    ;
   a[0].h.b =     63    ;
   //
   a[1].c.x =      0.50 ;
   a[1].c.y =      0.75 ;
   a[1].c.z =      0.50 ;
   a[1].r   =      0.25 ;
   a[1].h.r =      0    ; // color is Blue
   a[1].h.g =      0    ;
   a[1].h.b =    255    ;
   //
   a[2].c.x =      1.25 ;
   a[2].c.y =      0.65 ;
   a[2].c.z =      1.00 ;
   a[2].r   =      0.40 ;
   a[2].h.r =      0    ; // color is Green
   a[2].h.g =    255    ;
   a[2].h.b =      0    ;
   //
   a[3].c.x =      -.30 ;
   a[3].c.y =      0.65 ;
   a[3].c.z =      1.25 ;
   a[3].r   =      0.40 ;
   a[3].h.r =    255    ; // color is Red
   a[3].h.g =      0    ;
   a[3].h.b =      0    ;
}
//
int main(void)
{
   int*** rgb = (int***)malloc(N * sizeof(int**)); // red-green-blue for each pixel
   for (int i = 0; i < N; i++)
   {
    rgb[i] = (int**)malloc(M * sizeof(int*));
    for (int j = 0; j < M; j++)
    {
      rgb[i][j] = (int*)malloc(3 * sizeof(int));
    }
   }
   //
   int y , x ;
   triple p , r , v;
   color c;
   double rmag ;
   //
   FILE* fout ;
   //
   init();
   //
   for( y = 0 ; y < N ; y++ )
   {
      #pragma omp parallel for private(x,p,v,r,c)
      for( x = 0 ; x < M ; x++)
      {
         p.x = (x + 0.5)/ N - 1.0/6 ;
         p.y = 1.0 - (y + 0.5) / N ;
         p.z = 0.0 ;
         v.z = 0 ;
         //
         for( double row = -0.0005; row <= 0.0005; row+= 0.0005 ){
          for( double col = -0.0005; col <= 0.0005; col+= 0.0005 ){
            v.x = p.x + row ;
            v.y = p.y + col ;
            diff(&r, v, e) ;
            c = recursion(e, r, 0);
            if(c.r == -1){
              rgb[y][x][0] += 178 ;
              rgb[y][x][1] += 255 ;
              rgb[y][x][2] += 255 ;
            }
            else{
              rgb[y][x][0] += c.r ;
              rgb[y][x][1] += c.g ;
              rgb[y][x][2] += c.b ;
            }
          }
        }
        //
        rgb[y][x][0] = rgb[y][x][0] / 9 ;
        rgb[y][x][1] = rgb[y][x][1] / 9 ;
        rgb[y][x][2] = rgb[y][x][2] / 9 ;
     }
   }
   //
   //
   //
   fout = fopen( "image.ppm" , "w" ) ;
   //
   fprintf( fout , "P3\n" ) ;
   fprintf( fout , "%d %d\n" , M , N ) ;
   fprintf( fout , "255\n" ) ;
   //
   for( y = 0 ; y < N ; y++ )
   {
      for( x = 0 ; x < M ; x++)
      {
         fprintf( fout , "%d %d %d\n" ,
          rgb[y][x][0] , rgb[y][x][1] , rgb[y][x][2] ) ;
      }
   }
   fclose( fout ) ;
   //
   return 0 ;
}
//
// end of file
//
