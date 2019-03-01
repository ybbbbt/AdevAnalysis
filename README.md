# AdevAnalysis
Allan deviation for IMU/MEMS intrinsic calibration.

Usage:
```bash
sensors_decode < 00_0.sensors
adev_analysis
```

For `.sensors` file format, please go to <https://github.com/itsuhane/libsensors>

This tool requires Eigen for sensors decoding and gnuplot for image output, feel free to remove this part in the code if you don't need the plot.

## To be Implemented
* frequency estimation (for now the frequency is hardcoded)
* outlier removal
