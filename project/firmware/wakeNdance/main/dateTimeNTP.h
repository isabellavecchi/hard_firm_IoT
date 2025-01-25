#ifndef __DATE_TIME_NTP_LIB__
#define __DATE_TIME_NTP_LIB__


/**************************
**		DEFINITIONS		 **
**************************/

#define TCP_SOCKET  SOCK_STREAM
#define UDP_SOCKET  SOCK_DGRAM
#define NTP_PORT    123

#define NTP_TIMESTAMP_DELTA 2208988800UL // Seconds from 1900 to 1970
#define NTP_SERVER "pool.ntp.org" // or any other NTP server

#define DATE_LEN    11
#define TIME_LEN    6



/**************************
**		STRUCTURES		 **
**************************/

typedef struct
{
    uint8_t li_vn_mode;      // Eight bits. li, vn, and mode.
    uint8_t stratum;         // Eight bits. Stratum level of the local clock.
    uint8_t poll;            // Eight bits. Maximum interval between successive messages.
    uint8_t precision;       // Eight bits. Precision of the local clock.

    uint32_t rootDelay;      // 32 bits. Total round trip delay time.
    uint32_t rootDispersion; // 32 bits. Max error aloud from primary clock source.
    uint32_t refId;          // 32 bits. Reference clock identifier.

    uint32_t refTm_s;        // 32 bits. Reference time-stamp seconds.
    uint32_t refTm_f;        // 32 bits. Reference time-stamp fraction of a second.

    uint32_t origTm_s;       // 32 bits. Originate time-stamp seconds.
    uint32_t origTm_f;       // 32 bits. Originate time-stamp fraction of a second.

    uint32_t rxTm_s;         // 32 bits. Received time-stamp seconds.
    uint32_t rxTm_f;         // 32 bits. Received time-stamp fraction of a second.

    uint32_t txTm_s;         // 32 bits. Transmit time-stamp seconds.
    uint32_t txTm_f;         // 32 bits. Transmit time-stamp fraction of a second.

} ntp_packet;



/**************************
**		FUNCTIONS		 **
**************************/

void dateTimeNTP_setup(void);
char* dateTimeNTP_getTime(void);
char* dateTimeNTP_getData(void);

#endif //__DATE_TIME_NTP_LIB__