#include "protocol/messages.h"

#include <gtest/gtest.h>

TEST(MESSAGES, FIELD_CREATE)
{
    ASSERT_NO_THROW(protocol::UnsignedField<8> field(255));
    EXPECT_THROW(protocol::UnsignedField<8> field2(256), std::runtime_error);
}

TEST(MESSAGES, EQUAL)
{
    protocol::UnsignedField<10> field(0);
    ASSERT_EQ(field.getValue(), 0);
    field = 10;
    ASSERT_EQ(field.getValue(), 10);
}

TEST(MESSAGES, OR_BIT)
{
    protocol::UnsignedField<10> field(0);
    ASSERT_EQ(field.getValue(), 0);
    field |= 10;
    ASSERT_EQ(field.getValue(), 10);
    field |= 20;
    ASSERT_EQ(field.getValue(), 10 | 20);
}
