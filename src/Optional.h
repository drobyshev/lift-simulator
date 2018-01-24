#ifndef OPTIONAL_H
#define OPTIONAL_H

namespace lift_simulator
{
template <typename T>
class Optional
{
public:
    Optional() :
        m_isInit(false)
    {}

    Optional(const T& value) :
        m_value(value),
        m_isInit(true)
    {}

    Optional& operator=(const T& value)
    {
        m_value = value;
        m_isInit = true;
        return *this;
    }

    const T& Get() const
    {
        return m_value;
    }

    T& Get()
    {
        return m_value;
    }

    operator bool() const
    {
        return m_isInit;
    }

private:
    T    m_value;
    bool m_isInit;
};
}

#endif // OPTIONAL_H
