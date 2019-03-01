#include <imu_data.h>

#include <iostream>
#include <vector>

int main(int argc, char *argv[])
{
    ImuCsv acc_csv("acc");
    ImuCsv gyr_csv("gyr");
    acc_csv.load("acc.csv");
    gyr_csv.load("gyr.csv");

    imu_data acc_mean = acc_csv.mean_value();
    imu_data gyr_mean = gyr_csv.mean_value();

    std::cout << "accelerometer. data num = " << acc_csv.items.size()
              << ", real freq = " << acc_mean[0]
              << "hz, mean = [ " << acc_mean[1] << ", " << acc_mean[2] << ", " << acc_mean[3] << " ]\n";
    std::cout << "gyroscope.     data num = " << gyr_csv.items.size()
              << ", real freq = " << gyr_mean[0]
              << "hz, mean = [ " << gyr_mean[1] << ", " << gyr_mean[2] << ", " << gyr_mean[3] << " ]\n";

    return 0;
}
