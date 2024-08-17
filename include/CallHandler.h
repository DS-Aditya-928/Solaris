
void CallOut()
{
    OS_Sleep(100);

    inCall = true;
    fillScreen(BLACK);
    AUDIO_MicOpen();
    AUDIO_SpeakerOpen();
    AUDIO_MicSetMute(false);
    AUDIO_SpeakerSetVolume(15);

    CALL_Dial(number);

    drawRect(0, 280, 240, 40, WHITE);
    setCursor(120 - (strlen(number) * 6), 50);
    print(number);

    while (inCall)
    {
        tp = getPoint();

        if (tp.z >= MINP && tp.z <= MAXP )
        {
            if (tp.x >= 0 && tp.x <= 240 && tp.y >= 280 && tp.y <= 320)
            {
                inCall = false;
            }
        }
    }

    CALL_HangUp();
}

char *add;
uint8_t ringTone[10] = {'3', '3', '*', '7', '7', '6', '6', '5', '6', '7'};

void CallIn()
{
    OS_Sleep(1000);
    // inCall = true;
    fillScreen(BLACK);
    setTextSize(2);

    setCursor(120 - (strlen(add) * 6), 50);
    print(add);

    setCursor(120 - (7 * 6), 240);
    print("Accept?");    

    drawRect(0, 280, 120, 40, WHITE);
    setCursor(60 - (7), 290);
    print("Y");
    drawRect(120, 280, 120, 40, WHITE);
    setCursor(180 - (7), 290);
    print("N");

    bool pickedUp = false;

    clock_t ringStart = clock();

    int curSec;
    int lastSec = 100;

    while (inCall)
    {
        tp = getPoint();

        if (tp.z >= MINP && tp.z <= MAXP)
        {
            if (tp.x > (pickedUp?0:120) && tp.x <= 240 && tp.y >= 280 && tp.y <= 320)
            {
                CALL_HangUp();                
                tp.z = 0;
                AUDIO_SpeakerSetVolume(15);
                // OS_Sleep(1000);
                break;
            }

            if (tp.x >= 0 && tp.x <= 120 && tp.y >= 280 && tp.y <= 320 && !pickedUp)
            {
                fillRect(0, 280, 240, 40, BLACK);
                drawRect(0, 280, 240, 40, WHITE);
                CALL_Answer();
                setCursor(120 - (9 * 6), 240);
                print("End Call?");
                ringStart = clock();
                AUDIO_SpeakerSetVolume(5);
                pickedUp = true;
                // inCall = false;
            }
        }

        curSec = (int)((clock() - ringStart) / (CLOCKS_PER_SEC/4));//actually current quarter second.

        if (curSec != lastSec) // quarter seconds
        {
            if (pickedUp)
            {
                setCursor(0, 0);
                char ts[3];
                itoa(curSec/4, ts, 10);

                print(ts);
            }

            else
            {
                CALL_DTMF(ringTone[curSec % 10], 0, 6, 15, false);
            }
        }

        lastSec = curSec;
    }

    while(true)
    {
        //prevens crashes.
    }
}