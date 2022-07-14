
#define WIN32_LEAN_AND_MEAN
#include "Engine.h"
#include <stdlib.h>
#include <memory.h>

#include <math.h>
#include <windows.h>
#include <wchar.h>
#include <stdio.h>

#include <string> 

//
//  You are free to modify this file
//

//  is_key_pressed(int button_vk_code) - check if a key is pressed,
//                                       use keycodes (VK_SPACE, VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN, 'A', 'B')
//
//  get_cursor_x(), get_cursor_y() - get mouse cursor position
//  is_mouse_button_pressed(int button) - check if mouse button is pressed (0 - left button, 1 - right button)
//  clear_buffer() - set all pixels in buffer to 'black'
//  is_window_active() - returns true if window is active
//  schedule_quit_game() - quit game after act()

# define M_PI           3.14159265358979323846

using namespace std;

double aPlayer = 0;

int pEnemy = 20;
int xEnemy = 0;
int yEnemy = 0;
static int colorEnemy = 9450800;

int pFriend = 20;
int xFriend = 0;
int yFriend = 0;
static int colorFriend = 4892940;

int rPlayer = 15;
int xPlayer = 0;
int yPlayer = 0;
int xPlayer2 = 0;
int yPlayer2 = 0;
static int colorPers = 6000000;


double thex = 0;
double they = 0;
double thex2 = 0;
double they2 = 0;

static int rCircle = 150;
int xCircle = 0;
int yCircle = 0;
static int colorCircle = 2000;

static int thetic = 0;

static int score = 0;
static int chances = 3;

static bool flagPlayer = 1;
static bool dirPlayer = 1;

static int flagScore = 0;
static int flagChances = 0;

static bool flagEnemy = 1;
static int dirEnemy = 0;

static bool flagFriend = 1;
static int dirFriend = 0;


void drawPlayer(int rPlayer, int xPlayer, int yPlayer, int colorPers, int thex, int they, int xEnemy, int yEnemy, int xFriend, int yFriend) {

    for (int j = 1; j <= rPlayer; j++) {
        for (int i = 0; i < 360; i++) {
            double a = i * M_PI / 180;

            int x = j * cos(a);
            int y = j * sin(a);

            xPlayer = thex + (x + SCREEN_HEIGHT / 2);
            yPlayer = they + (y + SCREEN_WIDTH / 2);

            buffer[xPlayer][yPlayer] = colorPers;

            if (xPlayer == xEnemy && yPlayer == yEnemy) {
                flagChances++;
                flagEnemy = 1;
            }
            if (xPlayer == xFriend && yPlayer == yFriend) {
                flagScore++;
                flagFriend = 1;
            }
        }

    }

    
}

void drawEnemyOrFriend(int p, int x, int y, int color, int xp1, int yp1, int xp2, int yp2) {

    for (int j = 0; j < p; j++) {
        buffer[x++][y] = color;
    }
    for (int j = 0; j < p; j++) {
        buffer[x][y++] = color;
    }
    for (int j = 0; j < p; j++) {
        buffer[x--][y] = color;
    }
    for (int j = 0; j < p; j++) {
        buffer[x][y--] = color;
    }

}

void initialize()
{
   
}

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt)
{
  if (is_key_pressed(VK_ESCAPE))
    schedule_quit_game(); 

  HWND hwnd = GetActiveWindow();
  HDC hdc = GetDC(hwnd);

  string scorestr = "Score: " + to_string(score);
  LPCSTR scorel = scorestr.c_str();
  TextOutA(hdc, 50, 50, scorel, strlen(scorestr.c_str()));

  string chancesstr = "Chances: " + to_string(chances);
  LPCSTR chancesl = chancesstr.c_str();
  TextOutA(hdc, 50, 80, chancesl, strlen(chancesstr.c_str()));

  if (flagScore > 0) {
      score += 5;
      flagScore = 0;
  }

  if (flagChances > 0) {
      chances--;
      flagChances = 0;
  }

  if (chances <= 0) {
      score = 0;
      finalize();
      chances = 3;
      flagFriend = 1;
      flagEnemy = 1;
  }

}

// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw()
{
  memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));
  
  for (int j = rCircle; j >= rCircle-5; j--) {
      for (int i = 0; i < 360; i++) {

          double a = i * M_PI / 180;
          int x = j * cos(a);
          int y = j * sin(a);
          buffer[x + SCREEN_HEIGHT / 2][y + SCREEN_WIDTH / 2] = colorCircle;

      }
  }


  aPlayer = thetic * 0.5 * M_PI / 180;

  if (is_key_pressed(VK_SPACE)) {
      if (flagPlayer == 1) {
          if (dirPlayer == 1) {
              dirPlayer = 0;
          }
          else {
              dirPlayer = 1;
          }
          flagPlayer = 0;
      }
  }
  else {
      flagPlayer = 1;
  }

  switch (dirPlayer) {

  case 1:
      thetic++;
      break;
  case 0:
      thetic--;
      break;
  }

  thex = rCircle * cos(aPlayer);
  they = rCircle * sin(aPlayer);
  thex2 = -1 * rCircle * cos(aPlayer);
  they2 = -1 * rCircle * sin(aPlayer);

  drawPlayer(rPlayer, xPlayer, yPlayer, colorPers, thex, they, xEnemy, yEnemy, xFriend, yFriend);

  drawPlayer(rPlayer, xPlayer2, yPlayer2, colorPers, thex2, they2, xEnemy, yEnemy, xFriend, yFriend);


  if (flagEnemy == 1) {
      xEnemy = SCREEN_HEIGHT - 21;
      yEnemy = 21 + rand() % SCREEN_WIDTH - 21;

      if (yEnemy <= SCREEN_WIDTH / 2 - rCircle) {
          dirEnemy = 0;
      }
      else if (yEnemy > SCREEN_WIDTH / 2 - rCircle && yEnemy < SCREEN_WIDTH / 2 + rCircle) {
          dirEnemy = 1;
      }
      else if (yEnemy >= SCREEN_WIDTH / 2 + rCircle) {
          dirEnemy = 2;
      }

      flagEnemy = 0;
  }

  if (xEnemy <= SCREEN_HEIGHT && xEnemy >= 0 && yEnemy <= SCREEN_WIDTH && yEnemy >= 0) {

      switch (dirEnemy)
      {
      case 0:
          xEnemy--;
          yEnemy++;
          break;
      case 1:
          xEnemy--;
          break;
      case 2:
          xEnemy--;
          yEnemy--;
          break;
      }
  }
  else flagEnemy = 1;

  drawEnemyOrFriend(pEnemy, xEnemy, yEnemy, colorEnemy, xPlayer, yPlayer, xPlayer2, yPlayer2);

  if (flagFriend == 1) {
      xFriend = SCREEN_HEIGHT - 21;
      yFriend = 21 + rand() % SCREEN_WIDTH - 21;

      if (yFriend <= SCREEN_WIDTH / 2 - rCircle) {
          dirFriend = 0;
      }
      else if (yFriend > SCREEN_WIDTH / 2 - rCircle && yFriend < SCREEN_WIDTH / 2 + rCircle) {
          dirFriend = 1;
      }
      else if (yFriend >= SCREEN_WIDTH / 2 + rCircle) {
          dirFriend = 2;
      }

      flagFriend = 0;
  }

  if (xFriend <= SCREEN_HEIGHT && xFriend >= 0 && yFriend <= SCREEN_WIDTH && yFriend >= 0) {

      switch (dirFriend)
      {
      case 0:
          xFriend--;
          yFriend++;
          break;
      case 1:
          xFriend--;
          break;
      case 2:
          xFriend--;
          yFriend--;
          break;
      }
  }
  else flagFriend = 1;

  drawEnemyOrFriend(pFriend, xFriend, yFriend, colorFriend, xPlayer, yPlayer, xPlayer2, yPlayer2);
  
}

// free game data in this function
void finalize()
{
    clear_buffer();
    Sleep(1000);
}

