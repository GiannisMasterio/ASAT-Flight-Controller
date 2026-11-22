#include "ASAT_AHRS.h"

ASAT_AHRS::ASAT_AHRS(){




}

bool ASAT_AHRS::ahrs_init() { // Κεντρικη συναρτηση για τη setup
    if (!ahrs_calibrate_gyro()) {
        return false; // Κουνήθηκε το drone, ξαναπροσπάθησε
    }
    
    ahrs_init_attitude(); // Calculate initial Roll/Pitch from Accel
    return true;
}

bool ASAT_AHRS::ahrs_calibrate_gyro() {
    
    float sum_gx = 0.0f, sum_gy = 0.0f, sum_gz = 0.0f; // γυροσκοπια
    float sum_sq_x = 0.0f, sum_sq_y = 0.0f, sum_sq_z = 0.0f;

    const int samples = 500; // θα παιρνουμε 500 δειγματα για αρχη
    
    // max_variance = 0.0005f αντιστοιχεί σε τυπική απόκλιση ~0.022 rad/s (~1.3 deg/s)
    const float max_variance = 0.0005f; 
    // Μέγιστο λογικό bias για υγιές γυροσκόπιο (~8.6 deg/s)
    const float max_bias = 0.15f;
    
    for(int i=0; i<samples; i++){
           float gx, gy, gz, ax, ay, az;
           imu_get_scaled_data(&gx, &gy, &gz, &ax, &ay, &az);

           sum_gx += gx;
           sum_gy += gy;
           sum_gz += gz;

           sum_sq_x += gx * gx;
           sum_sq_y += gy * gy;
           sum_sq_z += gz * gz;

           chThdSleepMilliseconds(2); 
           // edw mporei na mpei mia anamonh poy tha dw pws tha ylopoihthei
    }

   // Οταν το drone κάθεται εντελώς ακίνητο πάνω στο τραπέζι, η πραγματική του ταχύτητα περιστροφής είναι $0\text{ μοίρες/δευτερόλεπτο}$.
   // Όμως, λόγω κατασκευαστικών ατελειών και θερμοκρασίας, ο αισθητήρας (MEMS) δεν βγάζει ποτέ καθαρό 0.00, αλλά ένα μικρό σταθερό
   // νούμερο (π.χ. 0.45 στον άξονα X, -0.12 στον άξονα Y). Αυτό το μόνιμο σφάλμα λέγεται bias.
   // Αποθηκεύοντας αυτές τις 3 τιμές, ξέρεις πλέον πόσο "ψέματα" λέει ο αισθητήρας.
   // όταν το drone αρχίσει να πετάει, σε κάθε νέο διάβασμα θα κάνεις αφαίρεση:καθαρό_gyro_x = νέα_μέτρηση_x - gyro_bias_x;
   // αυτο βεβαια προυποθετει το drone να ναι ακινητο αρχικα για αυτο θα γινεται motion detect

   // ετσι βρισκω αρχικες πολωσεις των γυροσκοπιων

    float mean_x = sum_gx / samples;
    float mean_y = sum_gy / samples;
    float mean_z = sum_gz / samples;

    if (fabsf(mean_x) > max_bias || fabsf(mean_y) > max_bias || fabsf(mean_z) > max_bias) {
        return false;
    }

    float var_x = (sum_sq_x / samples) - (mean_x * mean_x);
    float var_y = (sum_sq_y / samples) - (mean_y * mean_y);
    float var_z = (sum_sq_z / samples) - (mean_z * mean_z);

    if (var_x > max_variance || var_y > max_variance || var_z > max_variance) {
        return false;
    }

   this->gyro_bias_x = mean_x;
   this->gyro_bias_y = mean_y;
   this->gyro_bias_z = mean_z; 



   return true;
}

void ASAT_AHRS::ahrs_init_attitude() {
    float sum_ax = 0.0f, sum_ay = 0.0f, sum_az = 0.0f;
    const int samples = 100; // 100 δείγματα αρκούν για να φύγει ο θόρυβος

    for (int i = 0; i < samples; i++) {
        float gx, gy, gz, ax, ay, az;
        imu_get_scaled_data(&gx, &gy, &gz, &ax, &ay, &az);

        sum_ax += ax;
        sum_ay += ay;
        sum_az += az;
        chThdSleepMilliseconds(2);
    }

    float avg_ax = sum_ax / samples;
    float avg_ay = sum_ay / samples;
    float avg_az = sum_az / samples;

    // Υπολογισμός αρχικού Roll & Pitch (σε radians)
    this->roll  = atan2f(avg_ay, avg_az);
    this->pitch = atan2f(-avg_ax, sqrtf(avg_ay * avg_ay + avg_az * avg_az));
    this->yaw   = 0.0f; // Χωρίς compass, το οριζω 0
}
