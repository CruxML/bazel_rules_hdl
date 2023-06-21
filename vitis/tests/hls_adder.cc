#include "hls_adder.h"

namespace vitis {

void adder(hls::stream<fixed> &a, hls::stream<fixed> &b,
           hls::stream<fixed> &c) {
#pragma HLS interface axis port = a
#pragma HLS interface axis port = b
#pragma HLS interface axis port = c
#pragma HLS pipeline II = 1
  c.write(a.read() + b.read());
}

}  // namespace vitis