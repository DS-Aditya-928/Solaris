#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "stdlib.h"
#include "api_hal_i2c.h"
#include <string.h>

#include <time.h>
#include "api_os.h"
#include "api_debug.h"
#include "api_event.h"
#include "api_call.h"
#include "api_audio.h"
#include "api_fs.h"
#include "api_charset.h"
#include "api_hal_gpio.h"
#include "api_hal_pm.h"
#include "api_hal_uart.h"
#include <api_socket.h>
#include <api_network.h>
#include <api_debug.h>
#include "api_ssl.h"

#include "TFTMaster.h"
#include "colours.h"
#include "certificates.h"

#include "drawVars.h"
#include "ComVars.h"
#include "keyboard.h"
#include "netWork.h"
#include "CallHandler.h"
#include "contacts.h"
#include "keypad.h"
#include "DB9_main.h"

#define MAIN_TASK_STACK_SIZE (2048 * 2)
#define MAIN_TASK_PRIORITY 0
#define MAIN_TASK_NAME "Start Task"

#define SECOND_TASK_STACK_SIZE (2048 * 2)
#define SECOND_TASK_PRIORITY 0
#define SECOND_TASK_NAME "Main Task"

#define THIRD_TASK_STACK_SIZE (2048 * 2)
#define THIRD_TASK_PRIORITY 1
#define THIRD_TASK_NAME "Disp Loop"

#define CALL_TASK_STACK_SIZE (2048 * 2)
#define CALL_TASK_PRIORITY 1
#define CALL_TASK_NAME "Call Task"

static HANDLE mainTaskHandle = NULL;
static HANDLE secondTaskHandle = NULL;
static HANDLE thirdTaskHandle = NULL;
static HANDLE callTaskHandle = NULL;

void EventDispatch(API_Event_t *pEvent)
{
    switch (pEvent->id)
    {

    case API_EVENT_ID_NETWORK_REGISTERED_HOME:
    case API_EVENT_ID_NETWORK_REGISTERED_ROAMING:
    {
        netRegister = true;

        uint8_t status;

        Network_GetAttachStatus(&status);

        if (!status)
        {
            Network_StartAttach();
        }

        else
        {
            Network_PDP_Context_t context = {
                .apn = "airtelgprs.com", 
                .userName = "",
                .userPasswd = ""};
            Network_StartActive(context);
        }
        break;
    }

    case API_EVENT_ID_NETWORK_ATTACHED:
    {      
        //print("AYyy");
        Network_PDP_Context_t context = {
            .apn = "airtelgprs.com", 
            .userName = "",
            .userPasswd = ""};
        Network_StartActive(context);
        netAct = true;
        break;
    }

    case API_EVENT_ID_CALL_ANSWER:
        break;
    case API_EVENT_ID_CALL_HANGUP:
    {
        if (incoming)
        {
            OS_StopTask(callTaskHandle);
            fillScreen(BLACK);
            OS_Sleep(1000);

            if (turnOff)
            {
                draw = false;
                turnOff = false;
            }
            inCall = false;
            *(curDrawPtr) = true;
            incoming = false;
        }
        inCall = false;
        break;
    }
    case API_EVENT_ID_CALL_INCOMING:
    {
        inCall = true;
        add = pEvent->pParam1;
        // fillScreen(GREEN);
        // while (*(curDrawPtr));

        if (!draw)
        {
            turnOff = true;
            OS_Sleep(2750);
        }

        incoming = true;
        // fillScreen(RED);
        // OS_Sleep(500);
        callTaskHandle = OS_CreateTask(CallIn, NULL, NULL, CALL_TASK_STACK_SIZE, CALL_TASK_PRIORITY, 0, 0, CALL_TASK_NAME);
        // fillScreen(ORANGE);
        /*
        CallIn();

        *(curDrawPtr) = true;
        //end barcket*/
        break;
    }

    default:
        break;
    }
}

void displayHandler()
{
    while (true)
    {

        // OS_Sleep(100);
        // print("Work proof");
    }
}

void MainTask(void *pData)
{
    API_Event_t *event = NULL;

    TIME_SetIsAutoUpdateRtcTime(true);

    GPIO_config_t config;
    config.pin = GPIO_PIN26; // Button Pin
    config.mode = GPIO_MODE_INPUT;
    // config.defaultLevel = GPIO_LEVEL_HIGH;
    GPIO_Init(config);

    config.pin = GPIO_PIN3; // RST Pin
    config.mode = GPIO_MODE_OUTPUT;
    GPIO_Init(config);

    curTime.minute = 70;

    GPIO_Set(GPIO_PIN3, 0);
    OS_Sleep(250);
    GPIO_Set(GPIO_PIN3, 1);

    TFTInit();

    secondTaskHandle = OS_CreateTask(MainThread, NULL, NULL, SECOND_TASK_STACK_SIZE, SECOND_TASK_PRIORITY, 0, 0, SECOND_TASK_NAME);

    while (1)
    {
        if (OS_WaitEvent(mainTaskHandle, (void **)&event, OS_TIME_OUT_WAIT_FOREVER))
        {
            EventDispatch(event);
            OS_Free(event->pParam1);
            OS_Free(event->pParam2);
            OS_Free(event);
        }

        /*
        GPIO_Set(GPIO_PIN27, draw);//inbuilt led

        GPIO_Set(GPIO_PIN3, draw);
        GPIO_LEVEL s;
        GPIO_Get(GPIO_PIN26, &s);

        if (!draw && (!s || inCall))
        {
            GPIO_Set(GPIO_PIN3, 1);
            OS_Sleep(2500);
            setBrightness(255);
            fillScreen(BLACK);
            setTextColour(WHITE, BLACK);
            draw = true;
            *(curDrawPtr) = true;
        }
        */
    }
}

void DB9_Main(void)
{
    mainTaskHandle = OS_CreateTask(MainTask,
                                   NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle);
}
