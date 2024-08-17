short maxNumberLength;

char buttonText[] = {'+', '0', '<'};
char button4Text[] = {'E', 'C', 'S', 'M'};

clock_t lastAddtime;

void keyPad()
{
kpd:
    curDrawPtr = &kpDraw;

    fillScreen(BLACK);
    setTextSize(2);

    setCursor(120 - (strlen(number) * 6), 50);
    print(number);

    OS_Sleep(10);
    char* tP = "";
    //numCont = 2;
    itoa(numCont, tP, 10);
    memcpy(tP + strlen(tP) , "/256", 5);

    setCursor(0, 105);
    print(strcat(itoa(numCont, tP, 10), "/256"));

    for (uint8_t i = 0; i < 4; i++)
    {
        for (uint8_t j = 0; j < 3; j++)
        {
            drawRect(j * 80, 120 + (i * 40), 80, 40, WHITE);

            setCursor((j * 80) + 34, 135 + (i * 40)); // width = 12px;

            boxNum = ((i * 3) + 1 + j); // i is the y, j is the x

            if (boxNum <= 9)
            {
                char tp[] = "0";
                tp[0] += boxNum;
                print(tp);
            }

            else
            {
                char tp[] = "0";
                tp[0] = buttonText[boxNum - 10];
                print(tp);
            }
        }
    }

    for (uint8_t i = 0; i < 4; i++)
    {
        drawRect(i * 60, 280, 60, 40, WHITE);
        setCursor(26 + (i * 60), 295);
        char tp[] = "0";
        tp[0] = button4Text[i];
        print(tp);
    }

    kpDraw = false;

    while (true)
    {
        if(inCall)
        {
            OS_Sleep(1);
            continue;
        }

        if (kpDraw)
        {
            goto kpd;
        }
        // Serial.println("KP HAPPENIGN");
        if (strlen(number) > maxNumberLength)
        {
            maxNumberLength = strlen(number);
            setCursor(120 - (strlen(number) * 6), 50);
            print(number);
        }

        if (strlen(number) < maxNumberLength)
        {
            fillRect(0, 45, 240, 25, BLACK); // lets not clear thing repeatedly.
            maxNumberLength = strlen(number);
            setCursor(120 - (strlen(number) * 6), 50);
            print(number);
        }

        tp = getPoint();

        if (tp.z >= MINP && tp.z <= MAXP)
        {
            // Serial.println(String(tp.x) + " " + String(tp.y));

            if (tp.y >= 120 && tp.y <= 280)
            {
                short x = (tp.x) / 80;
                short y = (tp.y - 120) / 40;

                touchBoxNum = ((y * 3) + 1 + x); // accurate!

                validToAdd = (((clock() - lastAddtime) / CLOCKS_PER_MSEC) >= 200);

                if (validToAdd) // governs all keypresses
                {
                    char toAdd = 0;
                    if (touchBoxNum <= 9 && touchBoxNum >= 0 && (strlen(number) < 15))
                    {
                        toAdd = '0' + touchBoxNum;
                    }

                    else if (touchBoxNum == 10 && !strlen(number))
                    {
                        toAdd = '+';
                    }

                    else if (touchBoxNum == 11 && (strlen(number) < 15))
                    {
                        toAdd = '0';
                    }

                    else if (touchBoxNum == 12)
                    {
                        number[strlen(number) - 1] = 0;
                    }

                    strncat(number, &toAdd, 1);
                    lastAddtime = clock();
                }
            }

            else if (tp.y >= 280 && tp.y <= 320)
            {
                // 210 - 920

                switch ((tp.x) / 60)
                {
                case 0:
                {
                    fillScreen(BLACK);
                    return;
                }

                case 1:
                {
                    if (strlen(number))
                    {                        
                        CallOut();
                        kpDraw = true;
                    }
                    break;
                }

                case 2:
                {
                    if (true)
                    {
                        //setCursor(0, 0);
                        // numSave();
                        contact_t c;
                        //c.num = 9945359359;
                        char* bufName = keyBoard(false);//fine

                        memset(c.name, 0, 16);
                        memcpy(c.name, bufName, 15);

                        memset(c.num, 0, 16);
                        memcpy(c.num, number, 15);
                        addContact(c);

                        //print();
                        //OS_Sleep(2000);
                        kpDraw = true;
                    }
                    break;
                }

                case 3:
                {
                    contact_t buf = contactMenu(true);
                    
                    memset(number, 0, 16);
                    memcpy(number, buf.num, 15);
                    //number = buf.num;
                    kpDraw = true;
                    break;
                }
                }
            }
        }
        //end:        
    }

    return;
}