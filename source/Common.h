#pragma once

#include <chrono>
#include <cmath>
#include <functional>
#include <imgui.h>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

constexpr size_t ENTITY_COUNT = 10;
constexpr size_t EQ_DELTA     = 0.01;


struct Position {
    float x, y;
};

struct Velocity {
    float dx, dy;
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
 * @param debounce Time in milliseconds to wait before invoking the callback
 */
inline void Debounce(const std::function<void()>& callback, const double& debounce) {
    static auto lastCallTime = std::chrono::steady_clock::now();
    const auto currentTime   = std::chrono::steady_clock::now();

    const std::chrono::duration<double> elapsedTime = currentTime - lastCallTime;

    if (elapsedTime.count() > (debounce / 1000)) {
        callback();
        lastCallTime = currentTime;
    }
}
