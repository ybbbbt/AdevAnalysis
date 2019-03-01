#pragma once

#include <cmath>
#include <map>
#include <vector>
#include <array>

struct AdevNoiseModel {
    typedef std::array<double, 3> point_type;
    static const int n_fit = 1;
    bool fit(const std::vector<point_type> &points, const std::vector<unsigned char> &inlier_mask) {
        double new_noise = 0.0;
        size_t count = 0;
        for (size_t i = 0; i < inlier_mask.size(); ++i) {
            if (inlier_mask[i]) {
                double b = log10(points[i][1]) + log10(points[i][0]) * 0.5;
                new_noise += b;
                count++;
            }
        }
        m_noise = new_noise / count;
        return true;
    }

    bool consensus(const point_type &point) {
        return abs(pow(10.0, m_noise - log10(point[0])*0.5) - point[1]) <= m_err;
    }

    double m_err;
    double m_noise;
};

struct AdevRandomWalkModel {
    typedef std::array<double, 3> point_type;
    static const int n_fit = 1;
    bool fit(const std::vector<point_type> &points, const std::vector<unsigned char> &inlier_mask) {
        double new_noise = 0.0;
        size_t count = 0;
        for (size_t i = 0; i < inlier_mask.size(); ++i) {
            if (inlier_mask[i]) {
                double b = log10(points[i][1]) - log10(points[i][0]) * 0.5;
                new_noise += b;
                count++;
            }
        }
        m_noise = new_noise / count;
        return true;
    }

    bool consensus(const point_type &point) {
        return abs(pow(10.0, m_noise + log10(point[0])*0.5) - point[1]) <= m_err;
    }

    double m_err;
    double m_noise;
};
