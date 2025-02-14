#pragma once

#include <array>
#include <Common.h>
#include <concepts>
#include <memory>
#include <typeindex>
#include <unordered_map>


constexpr uint8_t MAX_SERVICES{ 20 };

template<typename T, typename ...Args>
concept NotSameAsT = (!std::same_as<std::decay_t<Args>, T> && ...);

class ServiceContainer {
public:
    ServiceContainer() = delete;

    ServiceContainer(const ServiceContainer&) = delete;

    ServiceContainer& operator=(const ServiceContainer&) = delete;

    ServiceContainer(ServiceContainer&&) = delete;

    ~ServiceContainer() = delete;

    template<typename T, typename ...Args> requires NotSameAsT<T, Args ...>
    static ServicePtr<T> Bind(Args&& ...args)
    {
        const auto type = std::type_index(typeid(T));
        const auto ptr  = std::make_shared<T>(std::forward<Args>(args) ...);

        m_Services[type] = Service{ type, ptr };

        for (Service& service: m_Container) {
            if (service.type == type) {
                fprintf(stderr, "Service %s already bound to Container\n", type.name());

                return nullptr;
            }

            if (!service.instance) {
                service.type     = type;
                service.instance = ptr;

                return ptr;
            }
        }

        throw std::runtime_error("Service Container is full");
    }

    template<typename T>
    static ServicePtr<T> Bind(T& instance)
    {
        const auto type = std::type_index{ typeid(T) };
        const auto ptr  = std::make_shared<T>(instance);

        m_Services[type] = Service{ type, ptr };

        for (Service& service: m_Container) {
            if (service.type == type) {
                fprintf(stderr, "Service %s already bound to Container\n", type.name());

                return nullptr;
            }

            if (!service.instance) {
                service.type     = type;
                service.instance = ptr;

                return ptr;
            }
        }

        throw std::runtime_error("Service Container is full");
    }

    template<typename T>
    static ServicePtr<T> Bind(T* instance)
    {
        const auto type = std::type_index{ typeid(T) };
        const auto ptr  = std::shared_ptr<T>(instance);

        m_Services[type] = Service{ type, ptr };

        for (Service& service: m_Container) {
            if (service.type == type) {
                fprintf(stderr, "Service %s already bound to Container\n", type.name());

                return nullptr;
            }

            if (!service.instance) {
                service.type     = type;
                service.instance = ptr;

                return ptr;
            }
        }

        throw std::runtime_error("Service Container is full");
    }

    template<typename T>
    static ServicePtr<T> Bind(std::shared_ptr<T> ptr)
    {
        const auto type = std::type_index{ typeid(T) };

        m_Services[type] = Service{ type, ptr };

        for (Service& service: m_Container) {
            if (service.type == type) {
                fprintf(stderr, "Service %s already bound to Container\n", type.name());

                return nullptr;
            }

            if (!service.instance) {
                service.type     = type;
                service.instance = ptr;

                return ptr;
            }
        }

        throw std::runtime_error("Service Container is full");
    }

    template<typename T>
    static ServicePtr<T> Get()
    {
        const auto type = std::type_index(typeid(T));

        if (!m_Services.contains(type)) {
            fprintf(stderr, "Service %s not found in Container\n", type.name());

            return nullptr;
        }

        return std::static_pointer_cast<T>(m_Services[type].instance);
    }

    template<typename T>
    static ServicePtr<T> Resolve()
    {
        const auto type = std::type_index(typeid(T));

        for (const auto& service: m_Container) {
            if (service.type == type)
                return std::static_pointer_cast<T>(service.instance);
        }

        char err[50];
        snprintf(err, sizeof(err), "Service %s not found in Container", type.name());
        throw std::runtime_error(err);
    }

private:
    struct Service {
        std::type_index type;
        ServicePtr<void> instance;

        explicit Service() :
            type(typeid(void)),
            instance(nullptr)
        { }

        template<typename T>
        explicit Service(const std::type_index type, const ServicePtr<T> instance) :
            type(type),
            instance(instance)
        { }

        Service& operator=(const Service&) = default;

        Service(const Service& other) = delete;
    };

    static std::array<Service, MAX_SERVICES> m_Container;
    static std::unordered_map<std::type_index, Service> m_Services;
};

inline std::array<ServiceContainer::Service, MAX_SERVICES> ServiceContainer::m_Container;
inline std::unordered_map<std::type_index, ServiceContainer::Service> ServiceContainer::m_Services;
