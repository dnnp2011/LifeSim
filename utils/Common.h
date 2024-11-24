#pragma once

#include <array>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <functional>
#include <imgui.h>
#include <mutex>
#include <ostream>
#include <queue>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>


// Forward Declarations
class ServiceContainer;

// Local Aliases
using Clock     = std::chrono::steady_clock;
using TimePoint = Clock::time_point;
using Duration  = std::chrono::duration<double, std::milli>;

// Constants
constexpr size_t ENTITY_COUNT{ 10 };
constexpr size_t EQ_DELTA{ static_cast<size_t>(0.1) };
inline size_t MAX_HARDWARE_THREADS{ std::thread::hardware_concurrency() };

// Enums
enum class ExitCode: uint8_t {
    SUCCESS            = 0,
    INVALID_SHAPE_TYPE = 1,
    COUNT,
};

enum class ShapeType: uint8_t {
    Rectangle,
    Circle,
    Triangle,
    COUNT
};

// Structs
struct Position {
    float x, y;

    explicit operator ImVec2() const
    {
        return { x, y };
    }

    Position() = default;

    Position(const float x, const float y) :
        x(x),
        y(y)
    { }

    Position(const Position& pos) = default;

    explicit Position(const ImVec2& vec) :
        x(vec.x),
        y(vec.y)
    { }
};

struct Velocity {
    float dx, dy;

    explicit operator ImVec2() const
    {
        return { dx, dy };
    }

    Velocity() = default;

    Velocity(const float dx, const float dy) :
        dx(dx),
        dy(dy)
    { }

    Velocity(const Velocity& vel) = default;

    explicit Velocity(const ImVec2& vec) :
        dx(vec.x),
        dy(vec.y)
    { }
};

struct Collider {
    unsigned int width, height;
};

struct Entity {
    unsigned int id;
};

struct Color {
    float r, g, b, a;
};

struct EntityData {
    std::array<Entity, ENTITY_COUNT> entities;
    std::array<Position, ENTITY_COUNT> positions;
    std::array<Velocity, ENTITY_COUNT> velocities;
    std::array<Collider, ENTITY_COUNT> colliders;
    std::array<ShapeType, ENTITY_COUNT> shapes;
    std::array<Color, ENTITY_COUNT> colors;
};

// Global Helper Functions
template<typename T>
std::string ToString(const T& input);

template<>
inline std::string ToString(const ShapeType& input)
{
    switch (input) {
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
inline std::string ToString(const ExitCode& input)
{
    switch (input) {
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

inline void HandleError(const ExitCode& exitCode, const std::string& message = "")
{
    switch (exitCode) {
        case ExitCode::INVALID_SHAPE_TYPE:
            ImGui::OpenPopup("Invalid Shape Type");
            break;
        default:
            fprintf(stderr, "exit(%s):: %s\n", ToString(exitCode).c_str(), message.c_str());
            exit(static_cast<int>(exitCode));
    }
}

// Overloads
inline std::ostream& operator<<(std::ostream& stream, const ImVec2& vec)
{
    // TODO: Rewrite using my custom Vector class
    return stream << "(" << vec.x << ", " << vec.y << ")";
}

/**
 * @brief Debounce a callback
 * @param callback Callback to invoke
 * @param msDelay Time in milliseconds to wait before invoking the callback
 * @param id Unique identifier for each call site
 */
inline void Debounce(
    const std::function<void(const std::string& id)>& callback,
    const double& msDelay,
    const std::string& id
)
{
    static std::unordered_map<std::string, TimePoint> lastCallTimes;

    auto& lastCallTime         = lastCallTimes[id];
    const auto currentTime     = Clock::now();
    const Duration elapsedTime = currentTime - lastCallTime;

    if (elapsedTime.count() > msDelay) {
        callback(id);

        lastCallTime = currentTime;
    }
}

// Utilities
namespace ImMath {
    inline ImVec2 operator+(const ImVec2& a, const ImVec2& b)
    {
        return { a.x + b.x, a.y + b.y };
    }

    inline ImVec2 operator-(const ImVec2& a, const ImVec2& b)
    {
        return { a.x - b.x, a.y - b.y };
    }

    inline ImVec2 operator*(const ImVec2& vec, const float& scalar)
    {
        return { vec.x * scalar, vec.y * scalar };
    }

    inline ImVec2 operator/(const ImVec2& vec, const float& scalar)
    {
        return { vec.x / scalar, vec.y / scalar };
    }

    inline ImVec2 Normalize(const ImVec2& vec)
    {
        const auto magnitude = std::sqrt(vec.x * vec.x + vec.y * vec.y);

        return { vec.x / magnitude, vec.y / magnitude };
    }

    inline ImVec2 CalculateNormal(const ImVec2& p1, const ImVec2& p2)
    {
        const auto normal = ImVec2{ p2.y - p1.y, -(p1.x - p2.x) };

        return Normalize(normal);
    }

    inline float Dot(const ImVec2& a, const ImVec2& b)
    {
        return (a.x * b.x) + (a.y * b.y);
    }

    inline ImVec2 Cross(const ImVec2& a, const ImVec2& b)
    {
        return { a.x * b.y, a.y * b.x };
    }

    inline ImVec2 Reflect(const ImVec2& velocity, const ImVec2& normal)
    {
        const auto dotProduct = Dot(velocity, normal);

        return velocity - (normal * (2.0f * dotProduct));
    }

    inline float Magnitude(const ImVec2& vec)
    {
        return std::sqrt(vec.x * vec.x + vec.y * vec.y);
    }

    inline float Distance(const ImVec2& a, const ImVec2& b)
    {
        return Magnitude(a - b);
    }

    inline ImVec2 Lerp(const ImVec2& current, const ImVec2& target, const float t)
    {
        return {
            current.x + t * (target.x - current.x),
            current.y + t * (target.y - current.y)
        };
    }

    inline ImVec2& Invert(ImVec2& vec)
    {
        vec.x = -vec.x;
        vec.y = -vec.y;

        return vec;
    }

    template<typename T>
    inline T Lerp(T current, T target, const float t)
    {
        return current + t * (target - current);
    }

    template<typename T>
    inline T Dampen(T current, T target, const float smoothing, const float t)
    {
        return current + (target - current) * (1 - std::exp(-smoothing * t));
    }

    template<typename T>
    inline T EaseInOut(T current, T target, const float t)
    {
        float factor = (1 - std::cos(t * M_PI)) / 2;

        return current * (1 - factor) + target * factor;
    }

    template<typename T>
    inline bool IsEqual(const T& a, const T& b, const T& delta = EQ_DELTA)
    {
        return std::abs(a - b) <= delta;
    }

    template<typename T>
    inline bool IsLessThanOrEqual(const T& a, const T& b, const T& delta = EQ_DELTA)
    {
        return (a < b) || ImMath::IsEqual(a, b, delta);
    }

    template<typename T>
    inline bool IsGreaterThanOrEqual(const T& a, const T& b, const T& delta = EQ_DELTA)
    {
        return (a > b) || ImMath::IsEqual(a, b, delta);
    }
}

namespace Threads {
    class ThreadPool {
    public:
        explicit ThreadPool(size_t numThreads);
        ~ThreadPool();
        explicit ThreadPool(const ThreadPool&) = delete;
        explicit ThreadPool(ThreadPool&&)      = delete;

        void enqueue(std::function<void()> task);

    private:
        std::vector<std::thread> m_workers;
        std::queue<std::function<void()>> m_tasks;
        std::mutex m_queueMutex;
        std::condition_variable m_condition;
        bool m_stop;

        void workerThread();
    };

    inline ThreadPool::ThreadPool(const size_t numThreads) :
        m_stop(false)
    {
        for (size_t i = 0; i < numThreads; ++i)
            m_workers.emplace_back(&ThreadPool::workerThread, this);
    }

    inline ThreadPool::~ThreadPool()
    {
        {
            std::lock_guard lock(m_queueMutex);
            m_stop = true;
        }

        m_condition.notify_all();

        for (std::thread& worker: m_workers)
            worker.join();
    }

    inline void ThreadPool::enqueue(std::function<void()> task)
    {
        {
            std::lock_guard lock(m_queueMutex);
            m_tasks.push(std::move(task));
        }

        m_condition.notify_one();
    }

    inline void ThreadPool::workerThread()
    {
        while (true) {
            std::function<void()> task;

            {
                std::unique_lock lock(m_queueMutex);

                m_condition.wait(
                    lock,
                    [this] {
                        return m_stop || !m_tasks.empty();
                    }
                );

                if (m_stop && m_tasks.empty())
                    return;

                task = std::move(m_tasks.front());

                m_tasks.pop();
            }

            task();
        }
    }
}

// Shared Aliases
using Container = ServiceContainer;
template<typename T>
using ServicePtr      = std::shared_ptr<T>;
using EntityBuffer    = std::vector<Entity>;
using PositionBuffer  = std::unordered_map<unsigned int, Position>;
using ColliderBuffer  = std::unordered_map<unsigned int, Collider>;
using VelocityBuffer  = std::unordered_map<unsigned int, Velocity>;
using ShapeTypeBuffer = std::unordered_map<unsigned int, ShapeType>;
using ThreadBuffer    = std::vector<std::thread>;
