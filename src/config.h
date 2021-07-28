// CALL
char call[] = "N0CALL";

// LOCATOR
char* locator = "AA00";
char* locator_full = "AA00AA";

// DEBUGGING
#define DEBUG
//#define DEBUG_GPS

// Power Measurment
bool POWERTEST = false;


enum mode
{
  MODE_WSPR,
  MODE_FT8
};
// Global variables
#define WSPR_DEFAULT_FREQ 3586600ULL //80m
#define WSPR_160m_FREQ 1836600UL //160m 
#define WSPR_80m_FREQ 3594000UL //80m 
#define WSPR_60m_FREQ 5287200UL //60m 
#define WSPR_40m_FREQ 7038600UL //40m 
#define WSPR_30m_FREQ 10138700UL //30m 
#define WSPR_20m_FREQ 14095600UL //20m 
#define WSPR_17m_FREQ 18104600UL //17m 
#define WSPR_15m_FREQ 21094600UL //15m 
#define WSPR_12m_FREQ 24924600UL //12m 
#define WSPR_10m_FREQ 28125900UL //10m
#define WSPR_60m_FREQ 50293000UL //6m
#define WSPR_40m_FREQ 70091000UL //4m
#define WSPR_2m_FREQ 144504000UL //2m

#define WSPR_DELAY 683        // Delay value for WSPR
#define WSPR_TONE_SPACING 146 // ~1.46 Hz

// Powerlevel

uint8_t dbm = 29;

bool send_160m = false;
bool send_80m = true;
bool send_60m = false;
bool send_40m = false;
bool send_30m = false;
bool send_20m = false;
bool send_17m = false;
bool send_15m = false;
bool send_12m = false;
bool send_10m = true;
bool send_6m = false;
bool send_4m = false;
bool send_2m = false;