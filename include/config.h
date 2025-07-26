#pragma once
#include <cstdint>

constexpr uint8_t BEGASEP_NUM_CLIENTS = 40;
constexpr uint32_t BEGASEP_NUM_MIN = 0xe0ffff00;
constexpr uint32_t BEGASEP_NUM_MAX = 0xe0ffffaa;

constexpr uint16_t SERVER_PORT = 2222;
constexpr uint16_t WORKERS = 4;

enum MessageType : uint8_t
{
    BEGASEP_OPEN = 1,
    BEGASEP_ACCEPT,
    BEGASEP_BET,
    BEGASEP_RESULT,
};
