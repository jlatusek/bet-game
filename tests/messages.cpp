#include "protocol/messages.h"

#include <gtest/gtest.h>

TEST(MESSAGES, FIELD_CREATE)
{
    ASSERT_NO_THROW(protocol::UnsignedField<8> field(255));
    EXPECT_THROW(protocol::UnsignedField<8> field2(256), std::runtime_error);
}
