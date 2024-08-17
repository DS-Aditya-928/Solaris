uint8_t lastMin = 70;
struct TFTSPoint tp;
#include "AppDrawer.h"

void mainMenu()
{
  curDrawPtr = &menuDraw;

  if (!inCall)
  {
    if (menuDraw)
    {
      fillScreen(BLACK);
      setTextColour(WHITE, BLACK);
    }

    if (netRegister)
    {
      TIME_GetRtcTime(&curTime);

      curTime.hour += curTime.timeZone;
      curTime.minute += curTime.timeZoneMinutes;

      if (curTime.minute >= 60)
      {
        curTime.minute -= 60;
        curTime.hour++;
      }

      if (curTime.hour >= 24)
      {
        curTime.hour -= 24;
      }

      if (menuDraw || (lastMin != curTime.minute)) 
      {
        fillRect(60, 0, 120, 10, BLACK);
        setTextSize(7);

        setCursor(78, 50);

        char bufT[] = "00";
        bufT[0] += curTime.hour / 10;
        bufT[1] += curTime.hour % 10;
        print(bufT);

        bufT[0] = '0';
        bufT[1] = '0';
        bufT[0] += curTime.minute / 10;
        bufT[1] += curTime.minute % 10;

        setCursor(78, 110);
        print(bufT);

        setTextSize(2);

        setCursor(108, 10);

        setCursor(90, 190);
      }

      lastMin = curTime.minute;
    }

    if (menuDraw)
    {
      setTextSize(1);

      for (int i = 0; i < 3; i++)
      {
        for (int j = 0; j < 3; j++)
        {
          drawRect(0 + (20 * i), 260 + (20 * j), 20, 20, WHITE); // bottom
          setCursor(7 + (20 * i), 268 + (20 * j));          
         
          printChar((char)((j * 3) + (i + 1) + 48));
          // Trace(1, num);
        }
      }

      drawRect(0, 0, 60, 60, WHITE);

      drawRect(180, 260, 60, 60, WHITE);

      if (!netRegister)
      {
        setCursor(72, 0); // 6 px
        setTextSize(1);
        print("Disconnected....");
      }

      menuDraw = false;
    }

    //tp = getPoint();

    if (tp.z >= MINP && tp.z <= MAXP)
    {
      if ((tp.x >= 0 && tp.x <= 60) && (tp.y >= 260 && tp.y <= 320))
      {
        // caller button
        fillScreen(BLACK);
        keyPad();
        menuDraw = true;
        OS_Sleep(100);
      }

      if ((tp.x >= 180 && tp.x <= 320) && (tp.y >= 260 && tp.y <= 320))
      {
        fillScreen(BLACK);
        appList();
        menuDraw = true;
        OS_Sleep(100);
      }

      if ((tp.x >= 0 && tp.x <= 60) && (tp.y >= 0 && tp.y <= 60) && draw)
      {        
        //test();
        //draw = false;
      }
    }
  }
}