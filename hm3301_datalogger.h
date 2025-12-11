#ifndef HM3301_DATALOGGER_H
#define HM3301_DATALOGGER_H

#include <stdint.h>

// New structure for HM3301 data
typedef struct {
    // Standard Particulate Matter (CF=1)
    uint16_t pm1_0_std;
    uint16_t pm2_5_std;
    uint16_t pm10_std;
} air_quality_reading_t;

#endif // HM3301_DATALOGGER_H