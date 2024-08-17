short boxNum = 0;
short touchBoxNum = 0;

bool validToAdd = true;
char buttonKPText[] = {'E', '<', 'M', 'S'};
static char tRKeyBoard[250]; // 249 char + null terminator

void printTr(bool bigKey)
{
    if (bigKey)
    {
        setTextSize(1);
        setCursor(0, 0);
    }

    else
    {
        setTextSize(2);
        setCursor(120 - (strlen(tRKeyBoard) * 6), 50);
    }

    print(tRKeyBoard);

    if(bigKey)
    {
        setTextSize(2);
    }
}

char *keyBoard(bool bigKey)
{
    OS_Sleep(100);
    short base = 65;
    int maxNameLength;

    memset(tRKeyBoard, 0, 250);

    curDrawPtr = &kbDraw;
startKB:
    fillScreen(BLACK);
    setTextSize(2);
    for (uint8_t i = 0; i < 6; i++)
    {
        drawRect(i * 40, 80, 40, 40, WHITE);
        setCursor((i * 40) + 14, 95);
        printChar((char)(base + i));
    }

    for (uint8_t i = 0; i < 4; i++)
    {
        for (uint8_t j = 0; j < 5; j++)
        {
            drawRect(j * 48, 120 + (i * 40), 48, 40, WHITE);

            setCursor((j * 48) + 18, 135 + (i * 40)); // width = 12px;

            boxNum = ((i * 5) + j); // i is the y, j is the x

            printChar((char)(base + 6 + boxNum));
        }
    }

    for (int i = 0; i < 4; i++)
    {
        drawRect(i * 60, 280, 60, 40, WHITE);
        setCursor((i * 60) + 24, 295);
        printChar(buttonKPText[i]);
    }

    printTr(bigKey);

    kbDraw = false;

    while (true)
    {
        if (inCall)
        {
            OS_Sleep(1);
            continue;
        }

        if (kbDraw)
        {
            goto startKB;
        }

        if (strlen(tRKeyBoard) > maxNameLength)
        {
            maxNameLength = strlen(tRKeyBoard);
            printTr(bigKey);
        }

        if (strlen(tRKeyBoard) < maxNameLength)
        {
            fillRect(0, 0, 240, 80, BLACK); // lets not clear thing repeatedly.
            maxNameLength = strlen(tRKeyBoard);
            printTr(bigKey);
        }

        tp = getPoint();

        if (tp.z >= MINP && tp.z <= MAXP)
        {
            short x;
            short y;

            if (tp.y <= 320 && tp.y >= 80) // valid touch location
            {
                if (tp.y >= 80 && tp.y <= 120)
                {
                    y = 0;
                    x = ((tp.x) / 40);
                    // Serial.println("X: " + String(x));
                }

                else if (tp.y >= 120 && tp.y <= 280)
                {
                    y = ((tp.y - 120) / 40) + 1;
                    x = (tp.x / 48) + 1;
                    // Serial.println(String(y) + " " + String(x));
                }

                else if (tp.y >= 280 && tp.y <= 320)
                {
                    y = 5;
                    x = ((tp.x) / 60) + 1;
                }

                touchBoxNum = ((y * 5) + x); // accurate!

                validToAdd = true;

                if (validToAdd)
                {
                    if (touchBoxNum >= 0 && touchBoxNum < 26)
                    {
                        if (strlen(tRKeyBoard) <= (bigKey?249:14))
                        {
                            tRKeyBoard[strlen(tRKeyBoard)] = (char)(touchBoxNum + base);
                            tRKeyBoard[strlen(tRKeyBoard) + 1] = 0;
                        }
                    }

                    else if (touchBoxNum == 26)
                    {
                        fillScreen(BLACK);
                        return (tRKeyBoard);
                    }

                    else if (touchBoxNum == 27)
                    {
                        tRKeyBoard[strlen(tRKeyBoard) - 1] = 0;
                    }

                    else if (touchBoxNum == 28)
                    {
                        if (base == 97)
                        {
                            base = 33;
                        }

                        else if (base == 65)
                        {
                            base = 97;
                        }

                        else if (base == 33)
                        {
                            base = 65;
                        }

                        // base = (base == 97) ? 65 : 97;
                        goto startKB;
                    }

                    else if (touchBoxNum == 29)
                    {
                        if(strlen(tRKeyBoard) <= bigKey?249:14)
                        {
                            tRKeyBoard[strlen(tRKeyBoard)] = 32;
                        }                        
                    }

                    OS_Sleep(100);
                }
            }
        }
    }
}