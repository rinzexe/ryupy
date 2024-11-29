#pragma once

#include "../../tensors/Tensor.h"

namespace ryupy
{
    namespace nn
    {
        class Layer
        {
        public:
            virtual ~Layer() = default;

            std::shared_ptr<Tensor> weight;

            virtual std::shared_ptr<Tensor> forward(Tensor &tensor) = 0;
        };
    }
}