#include "hls_adder.h"

#include "gtest/gtest.h"

namespace vitis {
namespace {

class HlsAdderTest : public testing::Test {};

TEST_F(HlsAdderTest, one_plus_one) {
  hls::stream<fixed> a;
  hls::stream<fixed> b;
  hls::stream<fixed> c;
  a.write(1);
  b.write(1);
  adder(a, b, c);
  EXPECT_EQ(c.read(), 2);
}

TEST_F(HlsAdderTest, quarter_plus_quarter) {
  hls::stream<fixed> a;
  hls::stream<fixed> b;
  hls::stream<fixed> c;
  a.write(0.25);
  b.write(0.25);
  adder(a, b, c);
  EXPECT_EQ(c.read(), 0.5);
}

}  // namespace
}  // namespace vitis