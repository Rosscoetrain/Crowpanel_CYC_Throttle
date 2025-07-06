/*

 *   By Ross Scanlon iam@rosscoe.com

*/

#ifndef DEFINES_H
#define DEFINES_H


#define EEPROM_SIZE 128
#define EEPROM_ADDR_WIFI_FLAG 0
#define EEPROM_ADDR_WIFI_CREDENTIAL 4

#define FORMAT_LITTLEFS_IF_FAILED true

#define THROTTLE_COUNT 4
#define NUM_LOCO_SLOTS 10
#define NUM_FUNC_SLOTS 10
#define NUM_LOCOS 100
//#define LOCOS_PER_ROSTER_PAGE 10

#define NUM_ACCS 252

#define NUM_ROUTES 48


#define NAME_LEN 36
#define ADDR_LEN 5
#define SPEED_LEN 3
//#define DIR_LEN 1
#define FNUM_LEN 3
#define FNAME_LEN 10

#define NORMAL_MODE 0
#define EDIT_MODE 1
#define SELECT_MODE 2
#define GUEST_ACTIVE 3
#define GUEST_INACTIVE 4

#define MAX_COMMAND_LENGTH 10

#define NUM_NWKS 10

#define FORMAT_LITTLEFS_IF_FAILED true

#endif // DEFINES_H
