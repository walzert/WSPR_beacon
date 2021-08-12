// LED PIN
#define LED_PIN 13

// CALL
char call[] = "N0CALL";



// LOCATOR
char* locator = "AA00";
char* locator_full = "AA00AA";

// DEBUGGING
//#define DEBUG
//#define DEBUG_GPS

// Power Measurment
bool POWERTEST = false;


// Global variables
#define WSPR_160m_FREQ 1836600UL //160m 
#define WSPR_80m_FREQ  3594000ULL //80m 
#define WSPR_80m_FREQ2 3568600ULL //80m 
#define WSPR_60m_FREQ  5287200UL //60m 
#define WSPR_40m_FREQ  7038600UL //40m 
#define WSPR_30m_FREQ  10138700UL //30m 
#define WSPR_20m_FREQ  14095600UL //20m 
#define WSPR_17m_FREQ  18104600UL //17m 
#define WSPR_15m_FREQ  21094600UL //15m 
#define WSPR_12m_FREQ  24924600UL //12m 
#define WSPR_10m_FREQ  28001300UL //10m
#define WSPR_6m_FREQ   50293000UL //6m
#define WSPR_4m_FREQ   70091000UL //4m
#define WSPR_2m_FREQ  144504000UL //2m

#define WSPR_DELAY 683        // Delay value for WSPR
#define WSPR_TONE_SPACING 146 // ~1.46 Hz

// Powerlevel

const uint8_t dbm = 29;

bool send_160m = false;
bool send_80m = true;
bool send_60m = false;
bool send_40m = false;
bool send_30m = false;
bool send_20m = false;
bool send_17m = false;
bool send_15m = false;
bool send_12m = false;
bool send_10m = false;
bool send_6m = false;
bool send_4m = false;  
bool send_2m = false;

#define RELAY_PIN0 7
#define RELAY_PIN1 9
#define RELAY_PIN2 10


// Relays
#define RELAY_160m RELAY_PIN2  //160m 
#define RELAY_80m RELAY_PIN0  //80m
#define RELAY_80m2 RELAY_PIN2  //80m  
#define RELAY_80m3 RELAY_PIN3  //80m  
#define RELAY_60m RELAY_PIN2  //60m 
#define RELAY_40m RELAY_PIN2  //40m 
#define RELAY_30m RELAY_PIN2  //30m 
#define RELAY_20m RELAY_PIN2  //20m 
#define RELAY_17m RELAY_PIN2  //17m 
#define RELAY_15m RELAY_PIN2  //15m 
#define RELAY_12m RELAY_PIN2  //12m 
#define RELAY_10m RELAY_PIN1  //10m 
#define RELAY_6m RELAY_PIN2  //6m 
#define RELAY_4m RELAY_PIN2  //4m 
#define RELAY_2m RELAY_PIN2  //2m 


//Output
#define OUTPUT_160m SI5351_CLK0  //160m 
#define OUTPUT_80m SI5351_CLK0  //80m 
#define OUTPUT_80m2 SI5351_CLK1  //80m 
#define OUTPUT_80m3 SI5351_CLK2  //80m 
#define OUTPUT_60m SI5351_CLK0  //60m 
#define OUTPUT_40m SI5351_CLK0  //40m 
#define OUTPUT_30m SI5351_CLK0  //30m 
#define OUTPUT_20m SI5351_CLK0  //20m 
#define OUTPUT_17m SI5351_CLK0  //17m 
#define OUTPUT_15m SI5351_CLK0  //15m 
#define OUTPUT_12m SI5351_CLK0  //12m 
#define OUTPUT_10m SI5351_CLK0  //10m 
#define OUTPUT_6m SI5351_CLK0  //6m 
#define OUTPUT_4m SI5351_CLK0  //4m 
#define OUTPUT_2m SI5351_CLK0  //2m 
