bool netRegister = false;
bool homeNet = false;
bool inCall = false;
bool incoming = false;
bool turnOff = false;
bool netAct = false;

struct TFTSPoint tp;

char number[16] = "";

RTC_Time_t curTime;

/*
API_FS_INFO fsInfo;

  /*
  print("Querying");
  API_FS_GetFSInfo(FS_DEVICE_NAME_FLASH, &fsInfo);
  print("queried");
  char tp[10];
  itoa(fsInfo.totalSize, &tp, 10);
  print(tp);
  
  
  OS_Sleep(300);
  print("Ok");

  OS_Sleep(300);
  int32_t contFile1 = API_FS_Open("/d.txt", FS_O_CREAT | FS_O_RDWR, 0);
  print("Opened");
  //char contData[1000];
  uint16_t num = 55000;

  API_FS_Write(contFile1, &num, 2);
  print("Written");
  API_FS_Close(contFile1);
  print("1");
  OS_Sleep(100);  

  contFile1 = API_FS_Open("/d.txt", FS_O_CREAT | FS_O_RDONLY, 0);
  uint16_t contData = 0;;
  API_FS_Read(contFile1, &contData, 2);
  API_FS_Close(contFile1);

  char tp[10];
  itoa(contData, &tp, 10);
  print(tp);
  OS_Sleep(100);
*/