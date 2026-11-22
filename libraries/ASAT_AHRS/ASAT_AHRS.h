// 1. Επικοινωνία & Ανάγνωση Αισθητήρων (Low-Level Drivers)
// 2. Βαθμονόμηση (Calibration) & Φιλτράρισμα Θορύβου
// 3. Τα Μαθηματικά της Στάσης (Sensor Fusion Algorithm)
// 4. Ακριβής Χρονομέτρηση ($dt$)
#pragma once

#include <math.h>
#include "hal.h" // Για το chThdSleepMilliseconds

class ASAT_AHRS {

   public:
     float gyro_bias_x = 0.0f, gyro_bias_y = 0.0f, gyro_bias_z = 0.0f;

     float roll = 0.0f, pitch = 0.0f, yaw = 0.0f;

     bool ahrs_calibrate_gyro();
     void ahrs_init();
     void ahrs_init_attitude();




}