#pragma once

template<typename T>
class optional
{
public:
    optional()
        : have_value_(false)
    {
    }

    optional(
        const T& value)
        : have_value_(true)
        , value_(value)
    {
    }

    bool have_value() const
    {
        return have_value_;
    }

    const T& value() const
    {
        return value_;
    }

    const T& value_or_default(
        const T& default_value) const
    {
        return have_value_ ? value_ : default_value;
    }

    bool operator==(
        const optional<T>& other) const
    {
        if (have_value_ ^ other.have_value_)
        {
            return false;
        }

        if (!have_value_)
        {
            return true;
        }

        return value_ == other.value_;
    }

private:
    bool have_value_;
    T value_;
};
