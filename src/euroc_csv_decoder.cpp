#include <cstdio>
#include <array>
#include <libsensors.h>
#include <imu_data.h>

const double GRAVITY_NOMINAL = -9.80665;

ImuCsv acc_csv("acc");
ImuCsv gyr_csv("gyr");


int main(int argc, char **argv) {
    if (FILE *csv = fopen(argv[1], "r")) {
        char header_line[2048];
        fscanf(csv, "%2047[^\n]\n", header_line);
        double t, wx, wy, wz, ax, ay, az;
        while (!feof(csv) && fscanf(csv, "%lf,%lf,%lf,%lf,%lf,%lf,%lf%*[,]\n", &t, &wx, &wy, &wz, &ax, &ay, &az) == 7) {
            t *= 1e-9;
            acc_csv.append(t, ax, ay, az);
            gyr_csv.append(t, wx, wy, wz);
        }
        fclose(csv);
    }
    acc_csv.items.resize(acc_csv.items.size() * 4 / 5);
    gyr_csv.items.resize(gyr_csv.items.size() * 4 / 5);
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
