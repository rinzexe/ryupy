#include "Optim.h"
#include <iostream>

namespace ryupy
{
    namespace nn
    {
        namespace optim
        {

            SGD::SGD(std::shared_ptr<LayerBank> bank,
                     float lr,
                     float momentum,
                     float dampening,
                     float weight_decay,
                     bool nesterov)
                : Optimizer(bank, lr),
                  momentum(momentum),
                  dampening(dampening),
                  weight_decay(weight_decay),
                  nesterov(nesterov)
            {
            }

            void SGD::step()
            {
                std::vector<std::shared_ptr<Tensor>> parameters;
                for (const auto &layer : layer_bank->layers)
                {
                    parameters.push_back(layer.second->weight);
                    parameters.push_back(layer.second->bias);
                }

                for (auto &param : parameters)
                {
                    auto d_p = param->grad->copy();

                    auto lr_tensor = Tensor::fill(d_p->shape, lr, false);
                    auto update = (*d_p) * (*lr_tensor);
                    *param -= *update;
                }
            }
        }
    }
}