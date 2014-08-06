/* Copyright (c) Ishak BELAHMAR <newishak@gmail.com>
 * 
 * This small tool is released under the WTFPL license 
 * See http://www.wtfpl.net/ for more details
 *
 */

#include <termios.h>
#include <stdio.h>
#include <unistd.h>

static struct termios old, new;

/* Initialize new terminal i/o settings */
void initTermios(int echo) 
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  new = old; /* make new settings same as old settings */
  new.c_lflag &= ~ICANON; /* disable buffered i/o */
  new.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
  tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char getch(void) 
{
  return getch_(0);
}

/* Read 1 character with echo */
char getche(void) 
{
  return getch_(1);
}

int main(void) {

  // Set unbuffered mode: IMPORTANT
  // Otherwise defaut behavior is waiting for a certain
  // amount of packets before releasing the pipe
  //
  setvbuf(stdout, NULL, _IONBF, 0);

  unsigned char c;
  while (1) {
    c = getch();
    // If EOF or EOT or NULL, leaving
    if (!c || c == 4 || c == 255)
      return 0;
    // If \r => print \n
    if ( c== '\r'){
       c = '\n';
    }
    printf("%c", c);
  }
  return 0;
} 
