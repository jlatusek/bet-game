#pragma once
#include <format>
#include <stdexcept>

namespace protocol
{

constexpr unsigned int VER_BIT_LEN = 3;
constexpr unsigned int LEN_BIT_LEN = 5;
constexpr unsigned int TYPE_BIT_LEN = 8;
constexpr unsigned int CLIENT_ID_BIT_LEN = 16;

template <uint32_t bit_len> class UnsignedField
{
  public:
    explicit UnsignedField(const unsigned int value) : value_(value)
    {
        if (value > ((1 << bit_len) - 1))
        {
            throw std::runtime_error(std::format("Value provided {} requires more bits than {}", value, bit_len));
        }
    }

    unsigned int getValue() const
    {
        return value_;
    }

    UnsignedField &operator=(const unsigned int value)
    {
        value_ = value;
        return *this;
    }

    UnsignedField &operator|=(const unsigned int value)
    {
        value_ |= value;
        return *this;
    }

  private:
    unsigned int value_;
};

class Header
{
  public:
    Header(const unsigned int ver, const unsigned int len, const unsigned int type, const unsigned int client_id)
        : ver(ver), len(len), type(type), client_id(client_id)
    {
    }

    Header(const std::array<unsigned int, 4> buff)
        : ver((buff[0] & 0x7) >> 5), len(buff[0] & (1 << 5) - 1), type(buff[1]), client_id(buff[2] << 8)
    {
        client_id |= buff[3] & 0xFF;
    }

    [[nodiscard]] std::array<unsigned int, 4> serialize() const
    {
        std::array<unsigned int, 4> buff = {};
        buff[0] = ver.getValue() << 5 | len.getValue();
        buff[1] = type.getValue();
        buff[2] = client_id.getValue() & 0xFF00 >> 8;
        buff[3] = client_id.getValue() & 0xFF;
        return buff;
    }

  private:
    typedef UnsignedField<VER_BIT_LEN> ver_field;
    typedef UnsignedField<LEN_BIT_LEN> len_field;
    typedef UnsignedField<TYPE_BIT_LEN> type_field;
    typedef UnsignedField<CLIENT_ID_BIT_LEN> client_id_field;
    ver_field ver;
    len_field len;
    type_field type;
    client_id_field client_id;
};
} // namespace protocol
