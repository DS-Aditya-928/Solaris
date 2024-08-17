#include "ctype.h"
#define maxContacts 256
#define fileName "/contacts.txt"

uint8_t numCont;
//#define test

typedef struct contact
{
    char num[16];//15 char + nullterm
    char name[16]; // 15 characters + null terminator

} contact_t;

contact_t contacts[maxContacts]; // 5.6kb total

static char tRAC[16];

char *allCaps(char *str)
{
    memset(tRAC, 0, 16);
    memcpy(tRAC, str, 16);

    for (int i = 0; i < 16; i++) // til null terminator
    {
        if (!tRAC[i])
        {
            break;
        }
        tRAC[i] = (uint8_t)toupper((unsigned char)tRAC[i]);
    }

    return (tRAC);
}

static void reSort()
{
    for (int i = 0; i < maxContacts - 1; i++)
    {
        for (int j = i + 1; j < maxContacts; j++)
        {
            char buf1[16], buf2[16];

            memcpy(buf1, allCaps(contacts[i].name), 16);
            memcpy(buf2, allCaps(contacts[j].name), 16);

            if (!buf1[0])
            {
                memset(buf1, '~', 16);
            }

            if (!buf2[0])
            {
                memset(buf2, '~', 16);
            }

            if (strcmp(buf1, buf2) > 0)
            {
                contact_t buf;
                buf = contacts[i];
                contacts[i] = contacts[j];
                contacts[j] = buf;
            }
        }
    }
}

void retrieveContacts();

void addContact(contact_t c)
{
    int32_t contFile = API_FS_Open(fileName, FS_O_CREAT | FS_O_RDWR, 0); // trunc clears all, append
    for (int i = 0; i < maxContacts; i++)
    {
        if (!contacts[i].num[0]) // first empty address
        {
            contacts[i] = c;
            break;
        }
    }
    API_FS_Close(contFile);
    contFile = API_FS_Open(fileName, FS_O_CREAT | FS_O_RDWR | FS_O_TRUNC, 0);
    API_FS_Write(contFile, &contacts, sizeof(contact_t) * maxContacts);
    API_FS_Close(contFile);
    OS_Sleep(100);
    retrieveContacts(); // updates local info.
}

void deleteContact(contact_t c) // numbers are unique. names arent.
{
    contact_t contactDel[maxContacts];
    int32_t contFile = API_FS_Open(fileName, FS_O_CREAT | FS_O_RDWR, 0);

    // read array as in file.
    API_FS_Read(contFile, &contactDel, sizeof(contact_t) * maxContacts);
    API_FS_Close(contFile);
    // CHECK FOR NUMBER AND AREA CODE
    for (int i = 0; i < maxContacts; i++)
    {
        if (!strcmp(contactDel[i].num, c.num))
        {
            // print("found it!");
            memset(contactDel[i].num, 0, 16);
            memset(contactDel[i].name, 0, 16);
            break;
        }
    }

    contFile = API_FS_Open(fileName, FS_O_CREAT | FS_O_RDWR | FS_O_TRUNC, 0);

    API_FS_Write(contFile, &contactDel, sizeof(contact_t) * maxContacts);
    API_FS_Close(contFile); // write with deleted member removed.

    retrieveContacts(); // updates localInfo.
}

void deleteAll()
{
    int32_t contFile = API_FS_Open(fileName, FS_O_CREAT | FS_O_RDWR | FS_O_TRUNC, 0); // trunc clears all, append
    // API_FS_Write(contFile, &c, sizeof(contact_t));
    API_FS_Close(contFile);
    memset(contacts, 0, sizeof(contact_t) * maxContacts);
}

void retrieveContacts()
{
    memset(&contacts, 0, sizeof(contact_t) * maxContacts);
    int32_t contFile = API_FS_Open(fileName, FS_O_CREAT | FS_O_RDWR, 0);

    API_FS_Read(contFile, &contacts, sizeof(contact_t) * maxContacts);
    API_FS_Close(contFile);

    reSort(); // levaes file untouched. only messes with local array

    numCont = 0;

    for (int i = 0; i < maxContacts; i++)
    {
        if (contacts[i].num[0])
        {
            // print(getNumAS(contacts[i]));
            numCont++;
//#define test
#ifdef test
            print(contacts[i].num);

            OS_Sleep(1000);
#endif
        }
    }
}

#define AUX true
#define MAIN false

uint8_t base = 0;

contact_t contactMenu(bool isAux)
{
    static contact_t tR;
startCm:
    curDrawPtr = &conDraw;

    fillScreen(BLACK);
    setTextSize(2);

    for (int i = 0; i < 7; i++)
    {
        drawRect(0, i * 40, 240, 40, WHITE);
        setCursor(15, (i * 40) + 12);
        print(contacts[base + i].name);
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

    conDraw = false;

    while (true)
    {
        if (inCall)
        {
            OS_Sleep(1);
            continue;
        }

        if (conDraw)
        {
            goto startCm;
        }

        tp = getPoint();

        if (tp.z >= MINP && tp.z <= MAXP)
        {
            if (tp.y >= 0 && tp.y <= 280) // numbox has been hit
            {
                short num = tp.y / 40;

                if (contacts[base + num].name != 0)
                {
                    if(isAux)
                    {
                        tR = contacts[base + num];
                        return(tR);
                    }

                    else
                    {
                        deleteContact(contacts[base + num]);
                    }
                    goto startCm;
                }
            }

            if (tp.y >= 280 && tp.y <= 320)
            {
                if (tp.x >= 0 && tp.x < 80)
                {
                    return tR;
                }

                else if (tp.x >= 80 && tp.x < 160 && ((base - 7) >= 0))
                {
                    base -= 7;
                    conDraw = true;
                    // Serial.println("BUTOON PRESS");
                }

                else if (tp.x >= 160 && tp.x <= 240)
                {
                    // check if first entry of the new list of 7 is empty or not.
                    if (*(contacts[base + 7].name) != 0)
                    {
                        base += 7;
                        conDraw = true;
                    }
                }
            }
        }
    }
}