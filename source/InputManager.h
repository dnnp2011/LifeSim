#pragma once


class Renderer;
class Instrumentation;

class InputManager {
private:
    Renderer* m_renderer;
    Instrumentation* m_instrumentation;

    float m_maxZoom{ 10.0f };
    float m_minZoom{ 0.1f };

public:
    explicit InputManager();

    [[nodiscard]]
    int Poll() const;
};
