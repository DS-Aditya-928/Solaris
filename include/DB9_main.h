#include "MainMenu.h"

void MainThread()
{
  draw = true;
  OS_Sleep(3000);
  // setBrightness(255);
  fillScreen(BLACK);
  setCursor(48, 100);
  setTextColour(WHITE, BLACK);
  setTextSize(2);
  print("Deskboy Mk.9"); // 12 px EACH
  setCursor(81, 170);//6 px
  setTextSize(1);
  print("By Aditya.D.S");
  fillRect(20, 200, 200, 40, WHITE);
  
  //deleteAll();
  retrieveContacts();
  
  fillScreen(BLACK);

  while (true)
  {
    if (draw)
    {
      mainMenu();
    }

    OS_Sleep(1);
  }
}