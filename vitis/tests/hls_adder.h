
#ifdef __SYNTHESIS__
#include <ap_fixed.h>
#include <hls_stream.h>
#else
#ifdef WITH_VIVADO_HLS
#include "vitis/v2020_1/ap_fixed.h"
#include "vitis/v2020_1/hls_stream.h"
#else
#include "vitis/v2021_2/ap_fixed.h"
#include "vitis/v2021_2/hls_stream.h"
#endif
#endif

#include "vitis/tests/consts.h"
namespace vitis {

typedef ap_fixed<16, 9> fixed;
void adder(hls::stream<fixed> &a, hls::stream<fixed> &b, hls::stream<fixed> &c);

}  // namespace vitis