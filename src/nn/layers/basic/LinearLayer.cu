#include "../../../tensors/Tensor.h"
#include "LinearLayer.h"
#include <cuda_runtime.h>
#include <iostream>

namespace ryupy
{
    namespace nn
    {
        LinearLayer::LinearLayer(int in_features, int out_features, InitType init_type)
        {
            std::vector<int> weight_shape = {in_features, out_features};

            switch (init_type)
            {
            case InitType::XAVIER_UNIFORM:
                weight = Tensor::xavier_uniform(weight_shape);
                break;
            case InitType::XAVIER_NORMAL:
                weight = Tensor::xavier_normal(weight_shape);
                break;
            case InitType::KAIMING_UNIFORM:
                weight = Tensor::kaiming_uniform(weight_shape);
                break;
            case InitType::KAIMING_NORMAL:
                weight = Tensor::kaiming_normal(weight_shape);
                break;
            }

            std::vector<int> bias_shape = {out_features};
            bias = Tensor::zeros(bias_shape);
        }

        std::shared_ptr<Tensor> LinearLayer::forward(Tensor &tensor)
        {
            return tensor.matmul(*weight);
        }

        std::shared_ptr<LinearLayer> LinearLayer::create(int in_features, int out_features, InitType init_type)
        {
            return std::make_shared<LinearLayer>(in_features, out_features, init_type);
        }
    }
}