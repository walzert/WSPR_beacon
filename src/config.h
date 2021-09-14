// LED PIN
#define LED_PIN 13

// CALL
// Enter here your CALL you are not allowed to use this sketch if you are not an licensed radio amateur
char call[] = "N0CALL";

// LOCATOR
char* locator = "AA00";
char* locator_full = "AA00AA";

// DEBUGGING
#define DEBUG  // Enable Serial Commands for DEBUGING
#define DEBUG_GPS  // Enable Serial Commands for DEBUGING GPS messages
#define DISPLAY_USE // Enable DISPLAY OUTPUT 
// Power Measurment
bool POWERTEST = false; // Enable POWERTEST for measureing dbm's  
int calibration = -4269000;  // Use Etherkit si5351_calibration.ino to get calibration factor

// Global variables
#define WSPR_160m_FREQ 1838060UL //160m 
#define WSPR_80m_FREQ  3568656UL //80m 
#define WSPR_60m_FREQ  5364779UL //60m  
#define WSPR_40m_FREQ  7040156UL //40m 
#define WSPR_30m_FREQ  10140158UL //30m 
#define WSPR_20m_FREQ  14095658UL //20m 
#define WSPR_17m_FREQ  18104693UL //17m 
#define WSPR_15m_FREQ  21094622UL //15m 
#define WSPR_12m_FREQ  24926172UL //12m 
#define WSPR_10m_FREQ  28126088UL //10m 
#define WSPR_6m_FREQ   50293073UL //6m 
#define WSPR_4m_FREQ   70092427UL //4m 
#define WSPR_2m_FREQ  144890496UL //2m 
                       


#define WSPR_DELAY 683        // Delay value for WSPR
#define WSPR_TONE_SPACING 146 // ~1.46 Hz

// Powerlevel
uint8_t dbm = 27;


// Enable sending on bands
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


// Define PINs for the relays sending on bands
#define RELAY_PIN0 7
#define RELAY_PIN1 9
#define RELAY_PIN2 10


// Set relays for the different bands
#define RELAY_160m RELAY_PIN0  //160m 
#define RELAY_80m RELAY_PIN0  //80m
#define RELAY_60m RELAY_PIN0  //60m 
#define RELAY_40m RELAY_PIN0  //40m 
#define RELAY_30m RELAY_PIN0  //30m 
#define RELAY_20m RELAY_PIN0  //20m 
#define RELAY_17m RELAY_PIN0  //17m 
#define RELAY_15m RELAY_PIN0  //15m 
#define RELAY_12m RELAY_PIN0  //12m 
#define RELAY_10m RELAY_PIN0  //10m 
#define RELAY_6m RELAY_PIN0  //6m 
#define RELAY_4m RELAY_PIN0  //4m 
#define RELAY_2m RELAY_PIN0  //2m 


//// Set Output on SI5351 for the different bands
#define OUTPUT_160m SI5351_CLK0  //160m 
#define OUTPUT_80m SI5351_CLK0  //80m 
#define OUTPUT_80m2 SI5351_CLK0  //80m 
#define OUTPUT_80m3 SI5351_CLK0  //80m 
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
