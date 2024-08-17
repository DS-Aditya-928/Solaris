// uint8_t wikiBuffer[10192 * 2];

/*
void test()
{

    // Get ip from DNS server.
    memset(ip, 0, sizeof(ip));
    if (DNS_GetHostByName2("en.wikipedia.org", ip) != 0)
    {
        print("Fail!");
    }

    else
    {
        print(ip);
        error = SSL_Init(&config);
        if (error != SSL_ERROR_NONE)
        {
            print("Fail 2!");
        }

        else
        {
            print("Success 2!");
        }

        error = SSL_Connect(&config, "en.wikipedia.org", "443");
        if (error != SSL_ERROR_NONE)
        {
            print("Fail 3!");
        }

        else
        {
            print("Success 3");
        }

        char *pData = "GET https://en.wikipedia.org/w/api.php?action=query&format=rawfm&prop=revisions%7Ccategoryinfo&titles=Pet_door&rvprop=content&rvslots=* HTTP/1.1\r\n\r\n\r\n\r\n\r\n";

        int err = SSL_Write(&config, pData, strlen(pData), 5000);

        if (err <= 0)
        {
            print("Fail 4");
        }

        else
        {
            print("OK 4");
            char buf;
            // print("Fail 5");
            print(itoa(err, &buf, 10));
        }

        memset(buffer, 0, sizeof(buffer));
        err = SSL_Read(&config, buffer, 10192 * 2, 2000);
        if (err <= 0)
        {
            char buf;
            print("Fail 5");
            print(itoa(err, &buf, 10));
        }

        else
        {
            char buf;
            // print("Fail 5");
            print(itoa(err, &buf, 10));
            for (int i = 1; i <= 64; i++)
            {
                print(buffer + (255 * i));
                OS_Sleep(1000);
            }
        }
        SSL_Close(&config);
    }
}
*/
static uint8_t mode = 0;
/*
0 = search menu
1 = search results
2 = contents
3 = specific text body display
*/

char searchQuery[250];

char *contents[32];

char *searchResults[16];
char *searchURLs[16];

char *splashText[3] = {"The Free Encyclopedia", "Knowledge is Power", "Knowledge is Freedom"};

void wikipedia()
{
    time_t preventTimeout = clock();
    // uint8_t ip[16];
    uint8_t base = 0;

    SSL_Error_t error;

    SSL_Config_t config = {
        .caCert = ca_cert,
        .caCrl = NULL,
        .clientCert = NULL,
        .clientKey = NULL,
        .clientKeyPasswd = NULL,
        .hostName = "en.wikipedia.org",
        .minVersion = SSL_VERSION_SSLv3,
        .maxVersion = SSL_VERSION_TLSv1_2,
        .verifyMode = SSL_VERIFY_MODE_OPTIONAL,
        .entropyCustom = "GPRS"};

    fillScreen(BLACK);
    setCursor(48, 100);
    setTextSize(2);
    print("Loading Wiki.");

    memset(searchQuery, 0, 250);

    while (!netAct)
    {
        OS_Sleep(1);
    }

    OS_Sleep(100);

    // DNS_GetHostByName2("en.wikipedia.org", ip);

    setCursor(0, 0);

    error = SSL_Init(&config);
    if (error != SSL_ERROR_NONE)
    {
        print("Fail 2!");
        return;
    }

    error = SSL_Connect(&config, "en.wikipedia.org", "443");
    if (error != SSL_ERROR_NONE)
    {
        print("Fail 3!");
        return;
    }

    mode = 0;
wkDraw:
    curDrawPtr = &wikiDraw;

    fillScreen(BLACK);

    switch (mode)
    {
    case 0:
    {
        setTextSize(4);
        setCursor(15, 50);
        print("Wikipedia");
        setTextSize(1);
        uint8_t splashRand = (((float)(rand() % 10)) / 10.f) * 3.f;
        setCursor(120 - (strlen(splashText[splashRand]) * 3), 100);
        print(splashText[splashRand]);

        drawRect(20, 150, 200, 40, WHITE);

        setCursor(25, 160);
        setTextSize(2);
        char tPSQ[13]; // 12 char + null
        memset(tPSQ, 0, 13);
        memcpy(tPSQ, searchQuery, 12);
        if (strlen(searchQuery) > 12)
        {
            memcpy(tPSQ + 9, "...", 4);
        }
        print(tPSQ);
        drawRect(180, 150, 40, 40, WHITE);

        setTextSize(3);
        setCursor(192, 160);
        print("Q");

        drawRect(0, 0, 40, 40, WHITE);
        break;
    }

    case 1:
    {
        setTextSize(2);

        for (int i = 0; i < 7; i++)
        {
            if ((base + i) > 15)
            {
                break;
            }
            drawRect(0, i * 40, 240, 40, WHITE);
            setCursor(15, (i * 40) + 12);
            char buf[19]; // 18 chars + null term
            memset(buf, 0, 19);
            memcpy(buf, searchResults[base + i], 18);
            if (strlen(searchResults[base + i]) > 18)
            {
                memcpy(buf + 15, "...", 4);
            }
            print(buf);
        }

        drawRect(0, 280, 80, 40, WHITE);
        drawRect(80, 280, 80, 40, WHITE);
        drawRect(160, 280, 80, 40, WHITE);

        setCursor(30, 295);
        print("E");
        setCursor(110, 295);
        print("<");
        setCursor(190, 295);
        print(">");

        break;
    }
    }

    wikiDraw = false;

    while (true)
    {
        if (inCall)
        {
            continue;
        }

        if (wikiDraw)
        {
            goto wkDraw;
        }

        if ((clock() - preventTimeout) >= (16384 * 60))
        {
            // keeps it alive
            // print("prevent");
            char tP[] = "GET https://en.wikipedia.org/w/api.php?action=opensearch&format=none&search=h&limit=1 HTTP/1.1\r\n\r\n\r\n";
            SSL_Write(&config, tP, strlen(tP), 5000);
            char buf[1450];
            memset(buf, 0, 1450);
            SSL_Read(&config, buf, 1450, 5000);
            // print("prevented");

            /*
            for (int i = 1; i <= 7; i++)
            {
                print(buf + (254 * i));
                OS_Sleep(1000);
            }
            */

            preventTimeout = clock();
        }

        tp = getPoint();

        if (tp.z >= MINP && tp.z <= MAXP)
        {
            switch (mode)
            {
            case 0:
            {
                if (tp.x >= 0 && tp.x <= 40 && tp.y >= 0 && tp.y <= 40)
                {
                    SSL_Close(&config);
                    SSL_Destroy(&config);
                    return;
                }

                else if (tp.x >= 20 && tp.x <= 180 && tp.y >= 150 && tp.y <= 190)
                {
                    char *buf = keyBoard(true);
                    memset(searchQuery, 0, 250);
                    memcpy(searchQuery, buf, 249);
                    wikiDraw = true;
                }

                else if (tp.x >= 180 && tp.x <= 220 && tp.y >= 150 && tp.y <= 190 && searchQuery[0])
                {
                    setCursor(87, 200); // 6 px
                    setTextSize(1);
                    print("Querying...");
                    // SAMPLE
                    //"https://en.wikipedia.org/w/api.php?action=opensearch&format=json&search=test&limit=32"

                    char searchString[256] = "GET https://en.wikipedia.org/w/api.php?action=opensearch&format=json&search=";
                    strcat(searchString, searchQuery);
                    strcat(searchString, "&limit=16 HTTP/1.1\r\n\r\n\r\n");
                    setTextSize(1);
                    setCursor(0, 0);

                    char *searchBuf = OS_Malloc(4500);
                    memset(searchBuf, 0, 4500);
                    int num = SSL_Write(&config, searchString, strlen(searchString), 5000);
                    /*
                    print("write done");
                    char buf[10];
                    print("write code");
                    print(itoa(num, buf, 10));
                    */
                    num = SSL_Read(&config, searchBuf, 4500, 5000);
                    /*
                    print("readDone\nreadCode");
                    print(itoa(num, buf, 10));
                    */
                    char lookStr[256] = "\"";
                    strcat(lookStr, searchQuery);
                    strcat(lookStr, "\",");
                    char *begin = strstr(searchBuf, lookStr); // begin is where actual data starts
                    begin += strlen(lookStr);
                    char *beg2 = begin;
                    *strstr(begin + 1, "],") = 0;

                    for (int i = 0; i < 16; i++)
                    {
                        intptr_t loc1 = strstr(begin, "\"") + 1; // char right after comma.
                        if (!(loc1 - 1))
                        {
                            break;
                        }
                        begin = loc1;
                        intptr_t loc2 = strstr(begin, "\"");
                        if (!loc2)
                        {
                            break;
                        }
                        begin = loc2 + 1;
                        searchResults[i] = OS_Malloc(loc2 - loc1 + 1);
                        memset(searchResults[i], 0, loc2 - loc1 + 1);
                        memcpy(searchResults[i], loc1, loc2 - loc1);

                        /*
                        fillScreen(BLACK);
                        setCursor(0, 0);
                        print(searchResults[i]);
                        OS_Sleep(1000);
                        */
                    }

                    /*
                    for (int i = 0; i <= 5; i++)
                    {
                        print(beg2 + (254 * i));
                        OS_Sleep(1000);
                    }
                    */
                    OS_Free(searchBuf);
                    wikiDraw = true;
                    mode = 1;
                }
                break;
            }

            case 1:
            {
                if (tp.y >= 0 && tp.y <= 280) // numbox has been hit
                {
                    short num = tp.y / 40;

                    if (!*searchResults[base + num])
                    {
                        // fancy display stuff to do later
                        // goto startCm;
                    }
                }

                if (tp.y >= 280 && tp.y <= 320)
                {
                    if (tp.x >= 0 && tp.x < 80)
                    {
                        mode = 0;
                        wikiDraw = true;
                    }

                    else if (tp.x >= 80 && tp.x < 160 && ((base - 7) >= 0))
                    {
                        base -= 7;
                        wikiDraw = true;
                        // Serial.println("BUTOON PRESS");
                    }

                    else if (tp.x >= 160 && tp.x <= 240)
                    {
                        //setCursor(0, 0);
                        //print("p1");
                        //print(searchResults[base + 7]);
                        // check if first entry of the new list of 7 is empty or not.
                        if ((base + 7) < 16)
                        {
                            if (strlen(searchResults[base + 7]) > 0)
                            {
                                print("p2");
                                base += 7;
                                wikiDraw = true;
                            }
                        }
                    }
                }
                break;
            }
            }
        }
    }
}