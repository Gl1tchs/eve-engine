// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

struct RenderStats;

template <typename VertexType>
struct RenderData;

template <typename VertexType>
class Primitive {
 public:
  Primitive(std::vector<VertexType>& vertices)
      : vertices_(vertices), max_elements_(vertices_.size()), mutable_(false) {}

  Primitive(size_t count) : max_elements_(count), mutable_(true) {
    vertices_.reserve(count);
  }

  virtual ~Primitive() = default;

  virtual void Render(RenderStats& stats) = 0;

  [[nodiscard]] virtual bool NeedsNewBatch() { return false; };

  [[nodiscard]] virtual bool NeedsNewBatch(uint32_t vertex_size,
                                           uint32_t index_size) {
    return false;
  };

  bool IsMutable() { return mutable_; }

  const VertexType* GetBatches() const { return vertices_.data(); }

  /**
   * @brief Adds new vertex to vector if not mutable
   * 
   * @param vertex Vertex to add
   */
  void AddVertex(const VertexType& vertex) {
    if (!mutable_) {
      return;
    }

    vertices_.push_back(vertex);
  }

  /**
 * @brief Resets and removes all elements from vector if not mutable
 * 
 */
  virtual void Reset() {
    if (!mutable_) {
      return;
    }

    vertices_.clear();
  }

  [[nodiscard]] size_t BatchCount() const { return vertices_.size(); }

 private:
  std::vector<VertexType> vertices_;
  size_t max_elements_;
  bool mutable_;
};
