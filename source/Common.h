#pragma once

#include <string>
#include <cstdint>
#include <imgui.h>


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

enum class ExitCode: std::uint_fast8_t {
    SUCCESS            = 0,
    INVALID_SHAPE_TYPE = 1,
    COUNT,
};

enum class ShapeType: std::uint_fast8_t {
    Rectangle,
    Circle,
    Triangle,
    COUNT
};

template<typename T>
std::string ToString(const T &value);

template<>
inline std::string ToString(const ShapeType &shape) {
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
inline std::string ToString(const ExitCode &code) {
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

inline void HandleError(const ExitCode &exitCode, const std::string &message = "") {
    switch (exitCode) {
        case ExitCode::INVALID_SHAPE_TYPE:
            ImGui::OpenPopup("Invalid Shape Type");
            break;
        default:
            fprintf(stderr, "exit(%s):: %s\n", ToString(exitCode).c_str(), message.c_str());
            exit(static_cast<int>(exitCode));
    }
}
