#include <cstdio>
#include <array>
#include <libsensors.h>
#include <imu_data.h>

const double GRAVITY_NOMINAL = -9.80665;

ImuCsv acc_csv("acc");
ImuCsv gyr_csv("gyr");

// NOTE: the accelerometer data must not be multiplied to GRAVITY_NOMINAL before
void on_accelerometer(double t, double x, double y, double z) {
    acc_csv.append(t, GRAVITY_NOMINAL * x, GRAVITY_NOMINAL * y, GRAVITY_NOMINAL * z);
}

void on_gyroscope(double t, double x, double y, double z) {
    gyr_csv.append(t, x, y, z);
}

int main() {
    sensors_accelerometer_handler_set(&on_accelerometer);
    sensors_gyroscope_handler_set(&on_gyroscope);

    std::array<unsigned char, 8192> buffer;
    while (true) {
        size_t nread = fread(buffer.data(), 1, buffer.size(), stdin);
        if (nread > 0) {
            sensors_parse_data(buffer.data(), nread);
        } else {
            if (feof(stdin) || ferror(stdin)) {
                break;
            }
        }
    }

    acc_csv.save("acc.csv");
    gyr_csv.save("gyr.csv");

    acc_csv.save_column("acc_x.csv", 1);
    acc_csv.save_column("acc_y.csv", 2);
    acc_csv.save_column("acc_z.csv", 3);
    gyr_csv.save_column("gyr_x.csv", 1);
    gyr_csv.save_column("gyr_y.csv", 2);
    gyr_csv.save_column("gyr_z.csv", 3);

    return 0;
}
