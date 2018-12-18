
// nagenda

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h> 
#include <ncurses.h>
#include <time.h>



int cols, rows;
int tyear , tmonth , tday ; 
int usersel_month, usersel_day ;
char app_statusbar[PATH_MAX];
int txtday = 0;
int foundit, found_day, found_month ; 




/////////////////////////////////
/////////////////////////////////
/////////////////////////////////
int fexist(char *a_option)
{
  char dir1[PATH_MAX]; 
  char *dir2;
  DIR *dip;
  strncpy( dir1 , "",  PATH_MAX  );
  strncpy( dir1 , a_option,  PATH_MAX  );

  struct stat st_buf; 
  int status; 
  int fileordir = 0 ; 

  status = stat ( dir1 , &st_buf);
  if (status != 0) {
    fileordir = 0;
  }

  // this is compatible to check if a file exists
  FILE *fp2check = fopen( dir1  ,"r");
  if( fp2check ) {
  // exists
  fileordir = 1; 
  fclose(fp2check);
  } 

  if (S_ISDIR (st_buf.st_mode)) {
    fileordir = 2; 
  }
return fileordir;
/////////////////////////////
}




////////////////////////////////////////////////////////////////////
char *strrlf(char *str) 
{     // copyleft, C function made by Spartrekus 
      char ptr[strlen(str)+1];
      int i,j=0;
      for(i=0; str[i]!='\0'; i++)
      {
        if (str[i] != '\n' && str[i] != '\n') 
        ptr[j++]=str[i];
      } 
      ptr[j]='\0';
      size_t siz = sizeof ptr ; 
      char *r = malloc( sizeof ptr );
      return r ? memcpy(r, ptr, siz ) : NULL;
}
////////////////////////////////////////////////////////////////////
char *strcut( char *str , int myposstart, int myposend )
{     // copyleft, C function made by Spartrekus 
      char ptr[strlen(str)+1];
      int i,j=0;
      for(i=0; str[i]!='\0'; i++)
      {
        if ( ( str[i] != '\0' ) && ( str[i] != '\0') )
         if ( ( i >=  myposstart-1 ) && (  i <= myposend-1 ) )
           ptr[j++]=str[i];
      } 
      ptr[j]='\0';
      size_t siz = sizeof ptr ; 
      char *r = malloc( sizeof ptr );
      return r ? memcpy(r, ptr, siz ) : NULL;
}





///////////////////////////////
///////////////////////////////
///////////////////////////////
char *strtimestamp()
{
      long t;
      struct tm *ltime;
      time(&t);
      ltime=localtime(&t);
      char charo[50];  int fooi ; 
      fooi = snprintf( charo, 50 , "%04d%02d%02d%02d%02d%02d",
	1900 + ltime->tm_year, ltime->tm_mon +1 , ltime->tm_mday, 
	ltime->tm_hour, ltime->tm_min, ltime->tm_sec 
	);
    size_t siz = sizeof charo ; 
    char *r = malloc( sizeof charo );
    return r ? memcpy(r, charo, siz ) : NULL;
}



//////////////////
int now_year()
{
      long t;
      struct tm *ltime;
      time(&t);
      ltime=localtime(&t);
      return ltime->tm_year + 1900;
}

//////////////////
int now_day()
{
      long t;
      struct tm *ltime;
      time(&t);
      ltime=localtime(&t);
      return ltime->tm_mday;
}
int now_month()
{
      long t;
      struct tm *ltime;
      time(&t);
      ltime=localtime(&t);
      return ltime->tm_mon+1;
}



/* signum function */
int sgn(int x)
{
  return (x > 0) ? 1 : (x < 0) ? -1 : 0;
}

void setpixel( int x , int y, char chprinttmp )
{ 
    if ( chprinttmp == '-' ) 
       mvaddch( y , x , ACS_HLINE );
    else if ( chprinttmp == '|' ) 
       mvaddch( y , x , ACS_VLINE );
    else
       mvaddch( y , x , chprinttmp);
}
void nlinebes(int ystart,int xstart,int yend,int xend, char chprint)
/*--------------------------------------------------------------
 * Bresenham-Algorithmus: Linien auf Rastergeraten zeichnen
 *
 * Eingabeparameter:
 *    int xstart, ystart        = Koordinaten des Startpunkts
 *    int xend, yend            = Koordinaten des Endpunkts
 *
 * Ausgabe:
 *    void setpixel(int x, int y) setze ein Pixel in der Grafik
 *         (wird in dieser oder aehnlicher Form vorausgesetzt)
 *---------------------------------------------------------------
 */
{
    int x, y, t, dx, dy, incx, incy, pdx, pdy, ddx, ddy, es, el, err;
 
/* Entfernung in beiden Dimensionen berechnen */
   dx = xend - xstart;
   dy = yend - ystart;
 
/* Vorzeichen des Inkrements bestimmen */
   incx = sgn(dx);
   incy = sgn(dy);
   if(dx<0) dx = -dx;
   if(dy<0) dy = -dy;
 
/* feststellen, welche Entfernung grââ¢âÆer ist */
   if (dx>dy)
   {
      /* x ist schnelle Richtung */
      pdx=incx; pdy=0;    /* pd. ist Parallelschritt */
      ddx=incx; ddy=incy; /* dd. ist Diagonalschritt */
      es =dy;   el =dx;   /* Fehlerschritte schnell, langsam */
   } else
   {
      /* y ist schnelle Richtung */
      pdx=0;    pdy=incy; /* pd. ist Parallelschritt */
      ddx=incx; ddy=incy; /* dd. ist Diagonalschritt */
      es =dx;   el =dy;   /* Fehlerschritte schnell, langsam */
   }
 
/* Initialisierungen vor Schleifenbeginn */
   x = xstart;
   y = ystart;
   err = el/2;
   setpixel(x,y, chprint);
 
/* Pixel berechnen */
   for(t=0; t<el; ++t) /* t zaehlt die Pixel, el ist auch Anzahl */
   {
      /* Aktualisierung Fehlerterm */
      err -= es;
      if(err<0)
      {
          /* Fehlerterm wieder positiv (>=0) machen */
          err += el;
          /* Schritt in langsame Richtung, Diagonalschritt */
          x += ddx;
          y += ddy;
      } else
      {
          /* Schritt in schnelle Richtung, Parallelschritt */
          x += pdx;
          y += pdy;
      }
       setpixel(x,y, chprint);
   }
}




const char *string_get_weekday(int year, int month, int day) 
{
  static const char *weekdayname[] = {
     "MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};
  size_t JND =                                                     \
          day                                                      \
        + ((153 * (month + 12 * ((14 - month) / 12) - 3) + 2) / 5) \
        + (365 * (year + 4800 - ((14 - month) / 12)))              \
        + ((year + 4800 - ((14 - month) / 12)) / 4)                \
        - ((year + 4800 - ((14 - month) / 12)) / 100)              \
        + ((year + 4800 - ((14 - month) / 12)) / 400)              \
        - 32045;
         return weekdayname[JND % 7];
}






void drawit()
{
  /////////////////////////////////////////////////
  FILE *fp6; int fetchi; 
  char fetchlinetmp[PATH_MAX];
  char charo[PATH_MAX]; int curmonth, curday;
  char fetchline[PATH_MAX]; int posy;  
  foundit = 0;
  int i ;  
  char textstring[PATH_MAX];
  int jo; 
  getmaxyx( stdscr, rows, cols);

    erase();
    nlinebes( 0  , cols/8 , rows-1,     cols/8 , '|'); 
    nlinebes( 0  , 2*cols/8 , rows-1, 2*cols/8 , '|'); 
    nlinebes( 0  , 3*cols/8 , rows-1, 3*cols/8 , '|'); 
    nlinebes( 0  , 4*cols/8 , rows-1, 4*cols/8 , '|'); 
    nlinebes( 0  , 5*cols/8 , rows-1, 5*cols/8 , '|'); 
    nlinebes( 0  , 6*cols/8 , rows-1, 6*cols/8 , '|'); 
    nlinebes( 0  , 7*cols/8 , rows-1, 7*cols/8 , '|'); 
    nlinebes( 0  , 8*cols/8 , rows-1, 8*cols/8 , '|'); 
    

    i = 0;

    if ( tmonth >= 13 ) 
    {  tmonth = 1; tyear++; }
    else if ( tmonth <= 0 ) 
    {  tmonth = 12; tyear--; }




      i = 7-1;
      if ( tday + i  <= 0 ) 
      {  tday = 27; tmonth--;}
      else if ( tday + i >= 32 ) 
      {  tday = 1; tmonth++;}

      i = 0;
      if ( strcmp( string_get_weekday( tyear, tmonth, tday+i ),      "TUE" ) == 0 )
        tday = tday-1;
      else if ( strcmp( string_get_weekday( tyear, tmonth, tday+i ), "WED" ) == 0 )
        tday = tday-2;
      else if ( strcmp( string_get_weekday( tyear, tmonth, tday+i ), "THU" ) == 0 )
        tday = tday-3;
      else if ( strcmp( string_get_weekday( tyear, tmonth, tday+i ), "FRI" ) == 0 )
        tday = tday-4;
      else if ( strcmp( string_get_weekday( tyear, tmonth, tday+i ), "SAT" ) == 0 )
        tday = tday-5;
      else if ( strcmp( string_get_weekday( tyear, tmonth, tday+i ), "SUN" ) == 0 )
        tday = tday-6;


    txtday = 0;
    for ( i = 0 ; i<= 7-1 ; i++ )
    {
      attroff( A_REVERSE );
      if ( tmonth == usersel_month )  
       if ( tday+i == usersel_day )  
       {   attron( A_REVERSE );   
           foundit = 1;
       }

    posy = 2;
    if ( tmonth >= 1 )
    if ( tmonth <= 12 )
    if ( tday+i >= 1 )
    if ( tday+i <= 31 )
    {
       mvprintw( 0  , 1+i*cols/8 , "%s %d/%d", string_get_weekday( tyear, tmonth, tday+i ), tmonth,  tday+i );
       
       if ( txtday == 0 ) 
       {
          txtday = tday +i;
          found_day =   tday + i;
          found_month = tmonth;
       }
    }


  //////////////////////////////////////
  attroff( A_REVERSE );
  if ( fexist( "nagenda.agn" ) == 1 )
  {
    fp6 = fopen( "nagenda.agn" , "rb");
    while( !feof(fp6) ) 
    {
          fgets(fetchline, PATH_MAX, fp6); 

          //fgets(fetchlinetmp, PATH_MAX, fp6); 
          //strncpy( fetchline, "" , PATH_MAX );
          //for( fetchi = 0 ; ( fetchi <= strlen( fetchlinetmp ) ); fetchi++ )
          //  if ( fetchlinetmp[ fetchi ] != '\n' )
          //    fetchline[fetchi]=fetchlinetmp[fetchi];

         if ( !feof(fp6) )
         {
              charo[0] = fetchline[24-2] ;
              charo[1] = fetchline[25-2] ;
              charo[2] = '\0';
              curmonth = atoi( charo );
              charo[0] = fetchline[24] ;
              charo[1] = fetchline[25] ;
              charo[2] = '\0';
              curday = atoi( charo );

              if ( curmonth ==   tmonth )
              if (  curday ==  tday+i )
              {
                mvprintw( posy, 1 + i*cols/8   , "%s" , charo  );

                strncpy( textstring, "" , PATH_MAX );
                jo = 0;
                for( fetchi = 28-1 ; ( fetchi <= strlen( fetchline )-1 ); fetchi++ )
                 if ( fetchline[ fetchi ] != '\n' )
                  if ( jo <= cols/8 -1-2 ) 
                     textstring[jo++]=fetchline[fetchi];

                mvprintw( posy++, 1 + i*cols/8 , "-%s" , textstring  );

              }



	 }
     }
     fclose( fp6 );
     }


    }
    attroff( A_REVERSE );

    
    attroff( A_REVERSE );
    mvprintw( rows-1, 0, "[%d/%d/%d]", tyear, usersel_month, usersel_day );

    mvprintw( rows-1, 0, "%s", app_statusbar );
    strncpy( app_statusbar , "", PATH_MAX );
}



////////////////////////
char *strninput( char *myinitstring )
{
   int strninput_gameover = 0; 
   char strmsg[PATH_MAX];
   char charo[PATH_MAX];
   strncpy( strmsg, myinitstring , PATH_MAX );
   int ch ;  int foo ; 
   int fooj; 
   while ( strninput_gameover == 0 )
   {
                  getmaxyx( stdscr, rows, cols);
                  attroff( A_REVERSE );
                  for ( fooj = 0 ; fooj <= cols-1;  fooj++)
                  {
                    mvaddch( rows-1, fooj , ' ' );
                  }
                  mvprintw( rows-1, 0, ":> %s" , strrlf( strmsg ) );
                  attron( A_REVERSE );
                  printw( " " );
                  attroff( A_REVERSE );
                  refresh() ; 
                  ch = getch();

		  /* if ( ch == 127  ) 
		  {
			   strncpy( strmsg, strcut( strmsg, 1 , strlen( strmsg ) -1 )  ,  PATH_MAX );
		  } */


		  if ( ch == 263 ) 
	  	   strncpy( strmsg, strcut( strmsg, 1 , strlen( strmsg ) -1 )  ,  PATH_MAX );

		  else if ( ch == KEY_BACKSPACE ) 
			   strncpy( strmsg, strcut( strmsg, 1 , strlen( strmsg ) -1 )  ,  PATH_MAX );

		  else if ( ch == 127 ) 
			   strncpy( strmsg, strcut( strmsg, 1 , strlen( strmsg ) -1 )  ,  PATH_MAX );

		  else if ( ch == 4  ) 
			 strncpy( strmsg, strtimestamp() ,  PATH_MAX );

                  /// BSD !! ctrl+b (over ssh)
		  else if ( ch == 2  ) 
			 strncpy( strmsg, "" ,  PATH_MAX );

                  /// BSD !! ctrl+b (over ssh)
		  if ( ch == 8 ) 
			   strncpy( strmsg, strcut( strmsg, 1 , strlen( strmsg ) -1 )  ,  PATH_MAX );

		  else if ( ch == 27  ) 
	{		 strncpy( strmsg, ""  ,  PATH_MAX );
                        strninput_gameover = 1;
           }
		  

		  else if ( ch == KEY_F(10)  ) 
    	          {		 
                        strncpy( strmsg, ""  ,  PATH_MAX );
                        strninput_gameover = 1;
                  }

	          else if (
			(( ch >= 'a' ) && ( ch <= 'z' ) ) 
		        || (( ch >= 'A' ) && ( ch <= 'Z' ) ) 
		        || (( ch >= '1' ) && ( ch <= '9' ) ) 
		        || (( ch == '0' ) ) 
		        || (( ch == '~' ) ) 
		        || (( ch == '!' ) ) 
		        || (( ch == '&' ) ) 
		        || (( ch == '=' ) ) 
		        || (( ch == ':' ) ) 
		        || (( ch == ';' ) ) 
		        || (( ch == '<' ) ) 
		        || (( ch == '>' ) ) 
		        || (( ch == ' ' ) ) 
		        || (( ch == '|' ) ) 
		        || (( ch == '#' ) ) 
		        || (( ch == '?' ) ) 
		        || (( ch == '+' ) ) 
		        || (( ch == '/' ) ) 
		        || (( ch == '\\' ) ) 
		        || (( ch == '.' ) ) 
		        || (( ch == '$' ) ) 
		        || (( ch == '%' ) ) 
		        || (( ch == '-' ) ) 
		        || (( ch == '{' ) ) 
		        || (( ch == '}' ) ) 
		        || (( ch == '(' ) ) 
		        || (( ch == ')' ) ) 
		        || (( ch == ']' ) ) 
		        || (( ch == '[' ) ) 
		        || (( ch == '*' ) ) 
		        || (( ch == '"' ) ) 
		        || (( ch == '@' ) ) 
		        || (( ch == '-' ) ) 
		        || (( ch == '_' ) ) 
		        || (( ch == '^' ) ) 
		        || (( ch == '\'' ) ) 
	             ) 
		  {
                           foo = snprintf( charo, PATH_MAX , "%s%c",  strmsg, ch );
			   strncpy( strmsg,  charo ,  PATH_MAX );
		  }
		  else if ( ch == 10 ) 
		  {
                        strninput_gameover = 1;
		  }
     }
     char ptr[PATH_MAX];
     strncpy( ptr, strmsg, PATH_MAX );
     size_t siz = sizeof ptr ; 
     char *r = malloc( sizeof ptr );
     return r ? memcpy(r, ptr, siz ) : NULL;
}




void file_append( char *strput )
{
   FILE *fp; char charo[PATH_MAX];
   fp = fopen( "nagenda.agn" , "ab+" );
   fputs(  "id-" , fp );
   fputs(  strtimestamp() , fp );
   fputs(  ";" , fp );
   snprintf( charo, 50 , "%04d%02d%02d", 2018 , usersel_month  , usersel_day );
   fputs(  charo , fp );
   fputs(  ";" , fp );
   fputs(  strput , fp );
   fputs(  "\n" , fp );
   fclose( fp );
   strncpy( app_statusbar , "|SAVED|", PATH_MAX );
}




///////////////////////////////////////////////////////////
int main( int argc, char *argv[])
{
   initscr();			
   curs_set( 0 );
   start_color();
   init_pair(1, COLOR_RED, COLOR_BLACK);
   init_pair(2, COLOR_GREEN, COLOR_BLACK);
   init_pair(3, COLOR_BLUE, COLOR_BLACK);
   init_pair(4, COLOR_CYAN, COLOR_BLACK);
   color_set( 2, NULL );

   int ch ; 
   int rows, cols;  
   getmaxyx( stdscr, rows, cols);
   int gameover_nsc = 0;

   tyear = now_year();
   tmonth = now_month() ; tday = now_day();
   usersel_month = tmonth; 
   usersel_day =   tday; 

   char userstring[PATH_MAX];
   strncpy( app_statusbar , "", PATH_MAX );
   while ( gameover_nsc == 0)
   {
           getmaxyx( stdscr, rows, cols);
           drawit();
           if ( foundit == 0 )
           {
               usersel_day   = found_day;
               usersel_month = found_month;
               drawit();
           }
	   refresh();			
           mvprintw( rows-1 , cols-4, "%d", ch );
           ch = getch();

           if ( ch == 'q' )      gameover_nsc =1 ;
           else if ( ch == 'k' ) { tday -= 7; usersel_day--;  }
           else if ( ch == 'j' ) { tday += 7; usersel_day++;  }
           else if ( ch == 'd' ) { usersel_month++; tmonth = usersel_month ; }
           else if ( ch == 'n' ) { usersel_month++; tmonth = usersel_month ; }
           else if ( ch == 'u' ) { usersel_month--; tmonth = usersel_month ; }
           else if ( ch == 'l' ) usersel_day++;
           else if ( ch == 'h' ) usersel_day--;
           else if ( ch == '>' ) tday++;
           else if ( ch == '<' ) tday--;
           else if ( ch == 'g' ) 
           { 
              tday = txtday ; 
              usersel_day = txtday ;
              usersel_month = tmonth;
           }
           else if ( ch == 27 )
           {
                 tmonth = now_month() ; tday = now_day();
                 tyear = now_year();
                 usersel_month = tmonth; 
                 usersel_day =   tday; 
           }
           else if ( ch == 'c' ) 
           {
               strncpy( userstring, strninput( "" ), PATH_MAX );
               if ( strcmp( userstring , "" ) != 0 ) 
                 file_append( userstring );
           }

    }

    curs_set( 1 );
    endwin();			/* End curses mode		  */
    return 0;
}




