#include <cstdio>
#include <array>
#include <vector>
#include <map>
#include <thread>

#include <ransac.h>
#include <gnuplot.h>
#include <adev_analysis.h>

std::array<std::vector<double>, 3> read_vectors(const std::string &filename) {
    std::array<std::vector<double>, 3> result;
    if (FILE *csv = fopen(filename.c_str(), "r")) {
        char header_line[2048];
        fscanf(csv, "%2047[^\n]", header_line);
        double x, y, z;
        while (!feof(csv) && fscanf(csv, "%*lf,%lf,%lf,%lf\n", &x, &y, &z) == 3) {
            result[0].emplace_back(x);
            result[1].emplace_back(y);
            result[2].emplace_back(z);
        }
        fclose(csv);
    }
    return result;
}

void adev(const std::vector<double> &data, const size_t n, std::pair<double, double> &a) {
    size_t M = data.size() / n;
    double ws = 0.0, dev = 0.0, nws, d;
    for (size_t i = 0; i < n; ++i) {
        ws += data[i];
    }
    for (size_t j = 1; j < M; ++j) {
        nws = 0.0;
        for (size_t i = 0; i < n; ++i) {
            nws += data[j*n + i];
        }
        d = (nws - ws) / n;
        ws = nws;
        dev += d*d;
    }
    dev /= 2 * (M - 1);
    dev = sqrt(dev);

    a.first = dev;
    a.second = dev / sqrt(M + 1);
}

void process_data(const std::vector<double> &data, double freq, const std::string name) {
    std::map<size_t, std::pair<double, double>> adev_data;
    for (size_t N = data.size() / 2; N >= 1; N = (size_t)(N / 1.1)) {
        adev_data[N] = std::make_pair(0.0, 0.0);
    }

    for (auto it = adev_data.begin(); it != adev_data.end();) {
        std::vector<std::thread> workers;
        size_t threads = std::thread::hardware_concurrency();
        for (size_t j = 0; j < threads && it != adev_data.end(); ++j, ++it)
            workers.emplace_back(&adev, std::ref(data), it->first, std::ref(it->second));
        for (auto &t : workers)
            t.join();
        printf(".");
    }
    printf("\n");

    std::vector<std::array<double, 3>> adev_data_vector;
    for (auto &p : adev_data) {
        adev_data_vector.push_back({ p.first / freq, p.second.first, p.second.second });
    }

    AdevNoiseModel model;
    AdevRandomWalkModel rwmodel;
    rwmodel.m_err = model.m_err = adev_data_vector[0][2];
    ransac(model, adev_data_vector);
    ransac(rwmodel, adev_data_vector);
    double crw = pow(10, model.m_noise);
    double cws = pow(10, rwmodel.m_noise);
    printf("continuous white noise density: %.7e\n", crw);
    printf("  discrete white noise density: %.7e\n", crw*sqrt(freq));
    printf("continuous random walk density: %.7e\n", cws*sqrt(3));
    printf("  discrete random walk density: %.7e\n", cws*sqrt(3) / sqrt(freq));

    gnuplot plot;
    plot.open();

    plot.command("$DATA << EOD");
    for (auto p : adev_data_vector) {
        plot.command(std::to_string(p[0]) + " " + std::to_string(p[1]) + " " + std::to_string(p[2]));
    }
    plot.command("EOD");

    // plot.command("set terminal qt enhanced");
    plot.command("set terminal png");
    plot.command("set out '" + name + ".png'");
    plot.command("set title 'Noise Analysis'");
    plot.command("set ylabel 'ADEV'");
    plot.command("set xlabel 'tau'");
    plot.command("set logscale xy");
    plot.command("f(x) = a/sqrt(x)+b*sqrt(x)");
    // plot.command("fit f(x) $DATA using 1:2 via a,b");
    plot.command("plot $DATA using 1:2:3 with errorbars title 'ADEV', " + \
    std::to_string(crw) + " / sqrt(x) title 'White Noise', " + \
    std::to_string(cws) + "*sqrt(x) title 'Random Walk'");
    // plot.command("replot a / sqrt(x) title 'WN Fit'");
    // plot.command("replot b*sqrt(x) title 'RW Fit'");
    // plot.command("replot " + std::to_string(crw) + " / sqrt(x) title 'White Noise'");
    // plot.command("replot " + std::to_string(cws) + "*sqrt(x) title 'Random Walk'");

    plot.command("pause mouse");
    plot.command("exit");
    plot.close();
}

int main(int argc, char* argv[]) {
    std::array<std::vector<double>, 3> data;
    double freq = 100;
    printf("freq = %.7e\n", freq);

    printf("reading acc.csv...\n");
    data = read_vectors("acc.csv");
    printf("acc.csv points = %zu\n", data[0].size());
    printf("calculating ax");
    process_data(data[0], freq, "ax");
    printf("calculating ay");
    process_data(data[1], freq, "ay");
    printf("calculating az");
    process_data(data[2], freq, "az");

    printf("reading gyr.csv...\n");
    data = read_vectors("gyr.csv");
    printf("gyr.csv points = %zu\n", data[0].size());
    printf("calculating gx");
    process_data(data[0], freq, "gx");
    printf("calculating gy");
    process_data(data[1], freq, "gy");
    printf("calculating gz");
    process_data(data[2], freq, "gz");

    printf("images output to *.png\n");

    return 0;
}
