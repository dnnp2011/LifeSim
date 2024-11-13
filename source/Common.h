#pragma once

#include <chrono>
#include <cmath>
#include <functional>
#include <imgui.h>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>


using Clock     = std::chrono::steady_clock;
using TimePoint = Clock::time_point;
using Duration  = std::chrono::duration<double, std::milli>;

constexpr size_t ENTITY_COUNT = 10;
constexpr size_t EQ_DELTA     = 0.1;

struct Position {
    float x, y;

    explicit operator ImVec2() const {
        return { x, y };
    }

    Position() = default;
    Position(const float x, const float y) : x(x), y(y) { }
    Position(const Position& pos) = default;
    explicit Position(const ImVec2& vec) : x(vec.x), y(vec.y) { }
};

struct Velocity {
    float dx, dy;

    explicit operator ImVec2() const {
        return { dx, dy };
    }

    Velocity() = default;
    Velocity(const float dx, const float dy) : dx(dx), dy(dy) { }
    Velocity(const Velocity& vel) = default;
    explicit Velocity(const ImVec2& vec) : dx(vec.x), dy(vec.y) { }
};

struct Collider {
    int width, height;
};

struct Entity {
    int id;
};

enum class ExitCode: unsigned int {
    SUCCESS            = 0,
    INVALID_SHAPE_TYPE = 1,
    COUNT,
};

enum class ShapeType: unsigned int {
    Rectangle,
    Circle,
    Triangle,
    COUNT
};

using EntityBuffer    = std::vector<Entity>;
using PositionBuffer  = std::unordered_map<int, Position>;
using ColliderBuffer  = std::unordered_map<int, Collider>;
using VelocityBuffer  = std::unordered_map<int, Velocity>;
using ShapeTypeBuffer = std::unordered_map<int, ShapeType>;
using ThreadBuffer    = std::vector<std::thread>;

template<typename T>
std::string ToString(const T& value);

template<>
inline std::string ToString(const ShapeType& shape) {
    switch (shape) {
        case ShapeType::Rectangle:
            return "Rectangle";
        case ShapeType::Circle:
            return "Circle";
        case ShapeType::Triangle:
            return "Triangle";
        case ShapeType::COUNT:
            return "ShapeType options: " + std::to_string(static_cast<int>(ShapeType::COUNT));
        default:
            return "Invalid ShapeType";
    }
}

template<>
inline std::string ToString(const ExitCode& code) {
    switch (code) {
        case ExitCode::SUCCESS:
            return "Program executed successfully";
        case ExitCode::INVALID_SHAPE_TYPE:
            return "Invalid ShapeType enum was provided";
        case ExitCode::COUNT:
            return "ExitCode options: " + std::to_string(static_cast<int>(ExitCode::COUNT));
        default:
            return "Invalid ExitCode";
    }
}

template<typename T>
bool IsEqual(const T& a, const T& b, const T& delta = EQ_DELTA) {
    return std::abs(a - b) <= delta;
}

template<typename T>
bool IsLessThanOrEqual(const T& a, const T& b, const T& delta = EQ_DELTA) {
    return (a < b) || IsEqual(a, b, delta);
}

template<typename T>
bool IsGreaterThanOrEqual(const T& a, const T& b, const T& delta = EQ_DELTA) {
    return (a > b) || IsEqual(a, b, delta);
}

inline void HandleError(const ExitCode& exitCode, const std::string& message = "") {
    switch (exitCode) {
        case ExitCode::INVALID_SHAPE_TYPE:
            ImGui::OpenPopup("Invalid Shape Type");
            break;
        default:
            fprintf(stderr, "exit(%s):: %s\n", ToString(exitCode).c_str(), message.c_str());
            exit(static_cast<int>(exitCode));
    }
}

/**
 * @brief Debounce a callback
 * @param callback Callback to invoke
 * @param msDelay Time in milliseconds to wait before invoking the callback
 * @param id Unique identifier for each call site
 */
inline void Debounce(const std::function<void(const std::string& id)>& callback, const double& msDelay, const std::string& id) {
    static std::unordered_map<std::string, TimePoint> lastCallTimes;

    auto& lastCallTime         = lastCallTimes[id];
    const auto currentTime     = Clock::now();
    const Duration elapsedTime = currentTime - lastCallTime;

    if (elapsedTime.count() > msDelay) {
        callback(id);

        lastCallTime = currentTime;
    }
}

namespace ImMath {
    /**
     * @brief Add two ImVec2 vectors
     * @param a Vector A
     * @param b  Vector B
     * @return Sum of two vectors
     */
    inline ImVec2 operator+(const ImVec2& a, const ImVec2& b) {
        return { a.x + b.x, a.y + b.y };
    }

    /**
     * @brief Subtract two ImVec2 vectors
     * @param a Vector A
     * @param b  Vector B
     * @return Difference of two vectors
     */
    inline ImVec2 operator-(const ImVec2& a, const ImVec2& b) {
        return { a.x - b.x, a.y - b.y };
    }

    /**
     * @brief Multiply an ImVec2 vector by a scalar
     * @param vec Vector to scale
     * @param scalar Scalar value
     * @return Scaled vector
     */
    inline ImVec2 operator*(const ImVec2& vec, const float& scalar) {
        return { vec.x * scalar, vec.y * scalar };
    }

    /**
     * @brief Divide an ImVec2 vector by a scalar
     * @param vec Vector to scale
     * @param scalar Scalar value
     * @return Scaled vector
     */
    inline ImVec2 operator/(const ImVec2& vec, const float& scalar) {
        return { vec.x / scalar, vec.y / scalar };
    }

    inline ImVec2 Normalize(const ImVec2& vec) {
        const auto magnitude = std::sqrt(vec.x * vec.x + vec.y * vec.y);

        return { vec.x / magnitude, vec.y / magnitude };
    }

    inline ImVec2 CalculateNormal(const ImVec2& p1, const ImVec2& p2) {
        const auto normal = ImVec2{ p2.y - p1.y, -(p1.x - p2.x) };

        return Normalize(normal);
    }

    inline float Dot(const ImVec2& a, const ImVec2& b) {
        return (a.x * b.x) + (a.y * b.y);
    }

    inline ImVec2 Cross(const ImVec2& a, const ImVec2& b) {
        return { a.x * b.y, a.y * b.x };
    }

    inline ImVec2 Reflect(const ImVec2& velocity, const ImVec2& normal) {
        const auto dotProduct = Dot(velocity, normal);

        return velocity - (normal * (2.0f * dotProduct));
    }
}
