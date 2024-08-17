
void appList()
{
startAd:
    curDrawPtr = &adDraw;

    fillScreen(BLACK);
    setTextSize(2);
    setCursor(10, 12);
    print("Contacts");
    drawRect(0, 0, 240, 40, WHITE);

    setCursor(10, 52);
    print("Messages");
    drawRect(0, 40, 240, 40, WHITE);

    setCursor(10, 92);
    print("Wikipedia");
    drawRect(0, 80, 240, 40, WHITE);

    setCursor(10, 252);
    print("Settings");
    drawRect(0, 240, 240, 40, WHITE);

    drawRect(0, 280, 240, 40, WHITE);

    adDraw = false;

    OS_Sleep(100);//wait for fresh tp data

    while (true)
    {
        if (inCall)
        {
            OS_Sleep(1);
            continue;
        }

        if (adDraw)
        {
            goto startAd;
        }

        tp = getPoint();

        if (tp.z >= MINP && tp.z <= MAXP)
        {
            // Serial.println(String(tp.x) + " " + String(tp.y));

            if (tp.x >= 0 && tp.x <= 240)
            {
                if (tp.y >= 0 && tp.y <= 40)
                {
                    contactMenu(MAIN);
                    adDraw = true;
                }

                else if (tp.y >= 80 && tp.y <= 120)
                {
                    wikipedia();
                    adDraw = true;
                }

                else if (tp.y >= 240 && tp.y <= 280)
                {
                    // settings();
                    adDraw = true;
                }

                else if (tp.y >= 280 && tp.y <= 320)
                {
                    fillScreen(BLACK);
                    return;
                }
            }
        }
    }
}