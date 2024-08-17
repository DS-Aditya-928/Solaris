#ifndef TFTMasterLIBDEF
#define TFTMasterLIBDEF

#define DELAYI2C 30
#define MINP 30
#define MAXP 500

uint16_t slaveAdd;

struct TFTSPoint
{
  int16_t x, y, z;
};

struct TFTSPoint tR;

bool suc = false;
static bool recieved = false;

bool fillScreen(uint16_t colour)
{
  OS_Sleep(DELAYI2C);
  uint8_t tS[3] = {0x1, colour & 0xFF, (colour >> 8) & 0xFF};

  return (UART_Write(2, &tS, 3));
}

bool setCursor(uint16_t x, uint16_t y)
{
  OS_Sleep(DELAYI2C);
  uint8_t tS[5] = {0x2, x & 0xFF, (x >> 8) & 0xFF, y & 0xFF, (y >> 8) & 0xFF};

  return (UART_Write(2, &tS, 5));
}

bool setBrightness(uint8_t b)
{
  OS_Sleep(DELAYI2C);
  uint8_t tS[2] = {0x3, b};

  return (UART_Write(2, &tS, 2));
}

bool setTextColour(uint16_t fore, uint16_t back)
{
  OS_Sleep(DELAYI2C);
  uint8_t tS[5] = {0x4, fore & 0xFF, (fore >> 8) & 0xFF, back & 0xFF, (back >> 8) & 0xFF};

  return (UART_Write(2, &tS, 5));
}

bool setTextSize(uint8_t s)
{
  OS_Sleep(DELAYI2C);
  uint8_t tS[2] = {0x5, s};

  return (UART_Write(2, &tS, 2));
}

bool print(char tP[])
{
  //OS_Sleep(DELAYI2C);//none of these in callback funcs or interrupts.
  uint8_t tS[256];
  memset(&tS[0], (uint8_t)0, sizeof(tS));
  tS[0] = 0x6;

  int i = 0;
  for(i = 0; i < 254; i++)//only copy 254 characters
  {
    if(tP[i])
    {
      tS[i + 1] = tP[i];
    }

    else
    {
      tS[i + 1] = 0;
      break;
    }    
  }
  //memcpy(&tS[1], &tP[0], 30);

  return (UART_Write(2, &tS, i + 2));
}

bool printChar(char tP)
{
  //OS_Sleep(DELAYI2C);//none of these in callback funcs or interrupts.
  uint8_t tS[3];

  tS[0] = 0x6;
  tS[1] = tP;
  tS[2] = 0;

  //memcpy(&tS[1], &tP[0], 30);

  return (UART_Write(2, &tS, 3));
}

bool drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t colour)
{
  OS_Sleep(DELAYI2C);
  uint8_t tS[11] = {0x7, x & 0xFF, (x >> 8) & 0xFF, y & 0xFF, (y >> 8) & 0xFF, w & 0xFF,
                    (w >> 8) & 0xFF, h & 0xFF, (h >> 8) & 0xFF, colour & 0xFF, (colour >> 8) & 0xFF};

  return (UART_Write(2, &tS, 11));
}

bool fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t colour)
{
  OS_Sleep(DELAYI2C);
  uint8_t tS[11] = {0x8, x & 0xFF, (x >> 8) & 0xFF, y & 0xFF, (y >> 8) & 0xFF, w & 0xFF,
                    (w >> 8) & 0xFF, h & 0xFF, (h >> 8) & 0xFF, colour & 0xFF, (colour >> 8) & 0xFF};

  return (UART_Write(2, &tS, 11));
}

static void func(UART_Callback_Param_t p)
{
  //suc = true;
  //char outData[10];
  //print("rec");
  recieved = true;
  memcpy(&tR, p.buf, p.length);
  //itoa(tR.y, outData, 10);
  //print(outData);
  //Trace(1, "Rec");
}

static void cbfunc(UART_Error_t error)
{

}

bool TFTInit()
{
  UART_Config_t configUART =
      {
          .baudRate = UART_BAUD_RATE_9600,
          .dataBits = UART_DATA_BITS_8,
          .stopBits = UART_STOP_BITS_2,
          .parity = UART_PARITY_NONE,
          .rxCallback = func,
          .errorCallback = cbfunc,
          .useEvent = false,
      };
  return (UART_Init(2, configUART));
}


struct TFTSPoint getPoint()
{
  //print("GP");
  OS_Sleep(DELAYI2C);
  uint8_t tS = 0x9;
  recieved = false;
  UART_Write(2, &tS, 1);
  
  while(!recieved)
  {
    OS_Sleep(1);
  }
  //OS_Sleep(100);

  return (tR);
}

/*
class TFTMaster
{
  uint8_t id;


public:

  void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t col)
  {
    Wire.beginTransmission(id);
    Wire.write("DR");
    Wire.write(strPrep(x, 3).c_str());
    Wire.write(strPrep(y, 3).c_str());
    Wire.write(strPrep(w, 3).c_str());
    Wire.write(strPrep(h, 3).c_str());
    Wire.write(strPrep(col, 5).c_str());
    Wire.endTransmission();
  }

  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t col)
  {
    Wire.beginTransmission(id);
    Wire.write("FR");
    Wire.write(strPrep(x, 3).c_str());
    Wire.write(strPrep(y, 3).c_str());
    Wire.write(strPrep(w, 3).c_str());
    Wire.write(strPrep(h, 3).c_str());
    Wire.write(strPrep(col, 5).c_str());
    Wire.endTransmission();
  }

  void windowScroll(int16_t x, int16_t y, int16_t w, int16_t h, int16_t dx, int16_t dy)
  {
    Wire.beginTransmission(id);
    Wire.write("SR");
    Wire.write(strPrep(x, 3).c_str());
    Wire.write(strPrep(y, 3).c_str());
    Wire.write(strPrep(w, 3).c_str());
    Wire.write(strPrep(h, 3).c_str());
    Wire.write(strPrep(dx, 3).c_str());
    Wire.write(strPrep(dy, 3).c_str());

    Wire.endTransmission();
  }

  /*
  void drawBitmap(uint16_t x, uint16_t y, const uint8_t bm[], int16_t width, int16_t height, uint16_t colour) //split it up. 32 bytes is the most we can send.
  {
    int nC = width / 16;
    int nR = height / 16;

    for (int i = 0; i < nC; i++)
    {
      for (int j = 0; j < nR; j++)
      {
        //first transmission is 'metadata'

        Serial.println("Starting bitmap");
        Wire.beginTransmission(id);
        Wire.write("LL");
        Wire.write((strPrep(x + (i * 16), 3) + strPrep(y + (j * 16), 3) + strPrep(16, 3) + strPrep(16, 3) + strPrep(colour, 5)).c_str());
        Wire.endTransmission();

        Wire.beginTransmission(id);
        for (int i = 0; i < 32; i++)
        {
          Wire.write(bm[i]);
          //Serial.println(i);
        }
        Wire.endTransmission();

        delay(DELAYI2C);

        Wire.beginTransmission(id);
        Wire.write("ENDBMP");
        //Wire.endTransmission();
        Serial.println(Wire.endTransmission());
        Serial.println("Done");
      }
    }
  }


  void setBacklight(bool md)
  {
    Wire.beginTransmission(id);
    Wire.write("BL");
    Wire.write(String(md).c_str());
    Wire.endTransmission();
  }

  TFTSPoint getPoint()
  {
    TFTSPoint tR;
    String proc;
    Wire.flush();

    Wire.requestFrom(0x18, 32);

    while (Wire.available())
    {
      char c = (char)Wire.read();
      if (c == '#')
      {
        break;
      }
      proc += c;
    }

    int bufX = proc.substring(4, 7).toInt();
    int bufY = proc.substring(0, 3).toInt();
    tR.z = proc.substring(8).toInt();

    tR.x =  ((bufX - 130)/770.0f) * 240;
    tR.y = 320 - (((bufY - 150)/780.0f) * 320);

    if(tR.x > 240)
    {
      if(tR.x <= 500)
      {
        tR.x = 240;
      }

      else
      {
        tR.x = 0;
      }
    }

    if(tR.y > 320)
    {
      if(tR.y <= 500)
      {
        tR.y = 320;
      }

      else
      {
        tR.y = 0;
      }
    }


    return (tR);
  }
};
*/

#endif