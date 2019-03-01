#include <libsensors.h>
#include <cstdint>
#include <memory>
#include <vector>

class Sensors {
  public:
    static Sensors* sensors() {
        static std::unique_ptr<Sensors> s_instance = std::make_unique<Sensors>();
        return s_instance.get();
    }

    virtual ~Sensors() {
        if (has_init) {
            deinit();
        }
    }

    void set_gyroscope_handler(gyroscope_handler h) {
        m_gyroscope_handler = h;
    }

    void set_accelerometer_handler(accelerometer_handler h) {
        m_accelerometer_handler = h;
    }

    void set_error_handler(error_handler h) {
        m_error_handler = h;
    }

    void parse_data(const void* bytes, size_t size) {
        buffer.insert(buffer.end(), (const unsigned char*)bytes, ((const unsigned char*)bytes) + size);

        while (true) {
            size_t consumed = 0;
            std::uint8_t type;
            double timestamp;

            if (!advance(type, consumed)) goto end_parse;
            if (!advance(timestamp, consumed)) goto end_parse;

            switch (type) {
            case 0x01: // gyroscope
            {
                double x, y, z;
                if (!advance(x, consumed)) goto end_parse;
                if (!advance(y, consumed)) goto end_parse;
                if (!advance(z, consumed)) goto end_parse;
                if (m_gyroscope_handler) {
                    (*m_gyroscope_handler)(timestamp, x, y, z);
                }
            } break;
            case 0x02: // accelerometer
            {
                double x, y, z;
                if (!advance(x, consumed)) goto end_parse;
                if (!advance(y, consumed)) goto end_parse;
                if (!advance(z, consumed)) goto end_parse;
                if (m_accelerometer_handler) {
                    (*m_accelerometer_handler)(timestamp, x, y, z);
                }
            } break;
            default: {
                error("unknown data type.");
            } break;
            }
            if (consumed > 0) {
                buffer.erase(buffer.begin(), buffer.begin() + consumed);
            }
        }

    end_parse:
        return;
    }

  private:
    bool try_advance_size(size_t size, size_t consumed) const {
        if (buffer.size() >= consumed + size) {
            return true;
        } else {
            return false;
        }
    }

    template <typename T>
    bool advance(T& value, size_t& consumed) const {
        if (buffer.size() >= consumed + sizeof(value)) {
            value = *(const T*)(buffer.data() + consumed);
            consumed += sizeof(value);
            return true;
        } else {
            return false;
        }
    }

    bool advance_size(size_t size, size_t& consumed, std::vector<std::uint8_t>& buf) const {
        if (buffer.size() >= consumed + size) {
            buf.resize(size);
            memcpy(buf.data(), buffer.data() + consumed, size);
            consumed += size;
            return true;
        } else {
            error("fatal error: buffer overrun.");
            exit(EXIT_FAILURE);
            return false;
        }
    }

    void error(const char* msg) const {
        if (m_error_handler) {
            (*m_error_handler)(msg);
        }
    }

    gyroscope_handler m_gyroscope_handler = nullptr;
    accelerometer_handler m_accelerometer_handler = nullptr;

    error_handler m_error_handler = nullptr;

    std::vector<unsigned char> buffer;

  public:
    void init() {
        has_init = true;
    }
    void deinit() {
        has_init = false;
    }
  private:
    bool has_init = false;
};

void sensors_gyroscope_handler_set(gyroscope_handler h) {
    Sensors::sensors()->set_gyroscope_handler(h);
}

void sensors_accelerometer_handler_set(accelerometer_handler h) {
    Sensors::sensors()->set_accelerometer_handler(h);
}

void sensors_error_handler_set(error_handler h) {
    Sensors::sensors()->set_error_handler(h);
}

void sensors_init() {
    Sensors::sensors()->init();
}

void sensors_deinit() {
    Sensors::sensors()->deinit();
}

void sensors_parse_data(const void* bytes, long size) {
    Sensors::sensors()->parse_data(bytes, size);
}
