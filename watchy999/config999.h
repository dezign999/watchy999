//Set this flag to true if you want to monitor Serial logs
RTC_DATA_ATTR bool debugger = false;

//NTP Syncing - updates your time to fix any drifting
//Enter your gmtOffset below, or leave it at "0" and the offset will be determined via http://ip-api.com automatically
RTC_DATA_ATTR int  gmtOffset = 0;
const char* ntpServer = "pool.ntp.org";

uint16_t ambientOffset = 7; //This varies for every RTC. Mine runs 7° C hotter than outside the watch.

//weather api - Update these to match your city/country/api key
//get your free api key from: https://openweathermap.org/appid
#define APIKEY "f058fe1cad2afe8e2ddc5d063a64cecb" //use your own API key (this is SQFMI'S) :)
#define URL "http://api.openweathermap.org/data/2.5/weather"

//Country abbreviation for OpenWeatherMap
#define COUNTRY "US"

// Put your Access Point SSIDs and correpsonding passwords below. Put your most used accesspoint first. Access Points MUST HAVE
// an accompanying Timeout in Seconds. Some APs take longer to respond than others, this rectifies these instances.
//**YOU MUST** HAVE ENTRIES FOR: apPasswords, apTimeouts, cityNames, cityAbbv that match the number of Access Points defined.
//A mismatch of credentials will result in a Wifi Credential Error and limit you to RTC weather and no NTP syncing.
const char* accessPoints[] = {"AP1", "AP2"};
const char* apPasswords[] = {"AP1-PASS", "AP2-PASS"};
int apTimeouts[] = { 5, 10 }; //How long in Seconds to wait for each AP to connect

// Place your city names below that correspond with the access point you,re connected to.
// This will show the weather for that access point you're connected to.
// If your city name isn't very unique, you can use its ID instead.
// When you look up a city on openweathermap.org, it'll take you to a page like
// "https://openweathermap.org/city/4574324". Copy those numbers (ex 4574324) on the end
// and put them between quotes where you'd normally put a cit name.
// If your city name has a space in it, use a + (plus sign) to join them, ex "WADING+RIVER".
// The plus sign is required so the web query url remains intact.
const char* cityNames[] = {"5143022", "GREENPORT"};

// This is the abbreviated name that will appear above your weather icon to identify which access point you're connected to.
// Maximum of 4 letters.
const char* cityAbbv[] = {"HOME", "GPT"};

//Count and Sanity Check Access Points and required matching criteria
const int accessPointCount = sizeof(accessPoints) / sizeof(accessPoints[0]); // number of known networks
const int apTimeoutsCount = sizeof(apTimeouts) / sizeof(apTimeouts[0]);
const int apPasswordsCount = sizeof(apPasswords) / sizeof(apPasswords[0]);
const int cityNamesCount = sizeof(cityNames) / sizeof(cityNames[0]);
const int cityAbbvCount = sizeof(cityAbbv) / sizeof(cityAbbv[0]);
