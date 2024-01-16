#include "catch2/catch_all.hpp"

#include "core/layer.h"
#include "core/layer_stack.h"

using namespace eve;

class MockLayer : public Layer {
 public:
  MockLayer(Ref<State>& state) : Layer(state) {}

  void OnUpdate(float ds) override {
    // Mock layer update
  }
};

TEST_CASE("Layer and LayerStack Functionality", "[Layer][LayerStack]") {
  Ref<State> test_state = CreateRef<State>();

  SECTION("LayerStack Functionality") {
    LayerStack layer_stack;

    // Create and push layers to the layer stack
    MockLayer* layer1 = new MockLayer(test_state);
    MockLayer* layer2 = new MockLayer(test_state);

    layer_stack.PushLayer(layer1);
    layer_stack.PushOverlay(layer2);

    REQUIRE(layer_stack.begin() != layer_stack.end());
    REQUIRE(layer_stack.rbegin() != layer_stack.rend());

    REQUIRE(*layer_stack.begin() == layer1);
    REQUIRE(*layer_stack.rbegin() == layer2);
  }

  SECTION("LayerStack Order and Iteration") {
    LayerStack layer_stack;

    // Create and push layers to the layer stack
    MockLayer* layer1 = new MockLayer(test_state);
    MockLayer* layer2 = new MockLayer(test_state);

    layer_stack.PushLayer(layer1);
    layer_stack.PushOverlay(layer2);

    // Check the order and iteration of the layer stack
    std::vector<Layer*> expected_order = {layer1, layer2};
    std::vector<Layer*> actual_order;

    for (const auto& layer : layer_stack) {
      actual_order.push_back(layer);
    }

    REQUIRE(actual_order == expected_order);

    std::vector<Layer*> expected_reverse_order = {layer2, layer1};
    std::vector<Layer*> actual_reverse_order;

    for (auto i = layer_stack.rbegin(); i != layer_stack.rend(); ++i) {
      actual_reverse_order.push_back(*i);
    }

    REQUIRE(actual_reverse_order == expected_reverse_order);
  }
}
