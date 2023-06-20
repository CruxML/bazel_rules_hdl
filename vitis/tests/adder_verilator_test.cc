
#include <verilated.h>
#include <verilated_vcd_c.h>

#include "Vadder.h"
#include "gtest/gtest.h"
#include "hls_adder.h"

namespace vitis {
namespace {

class AdderTest {
 public:
  AdderTest() {
    v_adder_ = std::make_unique<Vadder>();
    cycle_ = 0;
  }
  ~AdderTest() {
    if (trace_) trace_->close();
  }

  void Clock() {
    v_adder_->ap_clk = 0;
    v_adder_->eval();
    if (trace_) trace_->dump(cycle_);
    cycle_++;
    v_adder_->ap_clk = 1;
    v_adder_->eval();
    if (trace_) trace_->dump(cycle_);
    cycle_++;
  }

  void Reset() {
    v_adder_->ap_rst_n = 0;
    Clock();
    v_adder_->ap_rst_n = 1;
    Clock();
  }

  void SetStart(bool state) { v_adder_->ap_start = state; }

  void SetInput(fixed a, fixed b) {
    v_adder_->a_TDATA = a;
    v_adder_->b_TDATA = b;
  }

  void SetInputValid(bool valid) {
    v_adder_->a_TVALID = valid;
    v_adder_->b_TVALID = valid;
  }

  bool GetInputReady() { return v_adder_->a_TREADY && v_adder_->b_TREADY; }
  bool GetApReady() { return v_adder_->ap_ready; }

  fixed GetOut() { return v_adder_->c_TDATA; }

  void SetOutputReady(bool ready) { v_adder_->c_TREADY = ready; }

  bool GetOutputValid() { return v_adder_->c_TVALID; }

 private:
  std::unique_ptr<VerilatedVcdC> trace_;
  int cycle_;
  std::unique_ptr<Vadder> v_adder_;
};

TEST(adder_test, test_add) {
  // set initial state
  std::unique_ptr<AdderTest> adder_tester = std::make_unique<AdderTest>();
  adder_tester->SetInputValid(false);
  adder_tester->SetOutputReady(false);
  adder_tester->Reset();
  adder_tester->Clock();
  adder_tester->SetStart(true);
  adder_tester->Clock();
  while (!adder_tester->GetInputReady()) {
    adder_tester->Clock();
  }
  adder_tester->SetInput(1, 1);
  adder_tester->SetInputValid(true);
  adder_tester->SetOutputReady(true);
  while (!adder_tester->GetOutputValid()) {
    adder_tester->Clock();
  }
  EXPECT_EQ(adder_tester->GetOut(), 2);
}
}  // namespace
}  // namespace vitis
int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
