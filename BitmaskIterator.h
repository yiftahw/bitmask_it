#pragma once

#include <iterator>

/**
 * @brief a forward iterator that iterates over the set bits in a bitmask
 * 
 * @note the values returned by the iterator are the bit positions, i.e {0,1,4} for a bitmask 0x13
 * 
 * @tparam T an unsigned integer type
 */
template <typename T>
class BitmaskIterator
{
private:
    T m_mask;
    T m_bit;
    static inline constexpr T max_bit = sizeof(T) * 8;

public:
    struct iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = T;
        using pointer = T *;
        using reference = T &;

        iterator(uint64_t mask, uint64_t start_bit) : m_mask(mask), m_bit(start_bit) 
        { 
            if (!is_bit_set(m_mask, m_bit)) // starting position is not set
            {
                find_next();
            }
        }

        T operator*() const
        {
            return m_bit;
        }

        iterator &operator++()
        {
            return find_next();
        }

        iterator operator++(int)
        {
            iterator tmp = *this;
            find_next();
            return tmp;
        }

        bool operator==(const iterator &other) const
        {
            return m_bit == other.m_bit;
        }

        bool operator!=(const iterator &other) const
        {
            return m_bit != other.m_bit;
        }

    private:
        T m_mask;
        T m_bit;

        static bool is_bit_set(T mask, T bit)
        {
            return (mask & (1 << bit)) != 0;
        }

        iterator &find_next()
        {
            while (m_bit < max_bit)
            {
                m_bit++;
                if (is_bit_set(m_mask, m_bit))
                {
                    break;
                }
            }
            return *this;
        }
    };

    BitmaskIterator(T mask, T start_bit = 0) : m_mask(mask), m_bit(start_bit)
    {
        static_assert(std::is_unsigned<T>::value, "T must be of an unsigned integer type");
    }

    iterator begin()
    {
        return iterator(m_mask, m_bit);
    }

    iterator end()
    {
        return iterator(m_mask, max_bit);
    }
};
