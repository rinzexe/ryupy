#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include <memory>
#include <cudnn.h>

namespace py = pybind11;

namespace ryupy
{
    class Tensor : public std::enable_shared_from_this<Tensor>
    {
    public:
        std::vector<int> shape;
        float *d_data;
        int size;
        cudnnTensorDescriptor_t tensor_desc;

        std::vector<std::shared_ptr<Tensor>> prev;
        std::vector<std::weak_ptr<Tensor>> next;
        std::shared_ptr<Tensor> grad;        
        bool requiresGrad;                   
        std::function<void(void)> backward_fn; 
        bool is_leaf;

        Tensor() = default;
        explicit Tensor(const py::object &data);
        explicit Tensor(std::vector<int> shape);
        explicit Tensor(int size, std::vector<int> shape);
        virtual ~Tensor();

        static std::shared_ptr<Tensor> zeros(const std::vector<int> &shape);
        static std::shared_ptr<Tensor> ones(const std::vector<int> &shape);
        static std::shared_ptr<Tensor> fill(const std::vector<int> &shape, float value);
        static std::shared_ptr<Tensor> arange(float start, float stop, float step = 1.0f);
        static std::shared_ptr<Tensor> linspace(float start, float stop, int num);
        static std::shared_ptr<Tensor> eye(int n);
        static std::shared_ptr<Tensor> random_uniform(const std::vector<int> &shape, float low = 0.0f, float high = 1.0f);
        static std::shared_ptr<Tensor> random_normal(const std::vector<int> &shape, float mean = 0.0f, float std = 1.0f);
        static std::shared_ptr<Tensor> xavier_normal(const std::vector<int> &shape);
        static std::shared_ptr<Tensor> xavier_uniform(const std::vector<int> &shape);
        static std::shared_ptr<Tensor> kaiming_normal(const std::vector<int> &shape);
        static std::shared_ptr<Tensor> kaiming_uniform(const std::vector<int> &shape);

        static std::pair<int, int> calculate_fans(const std::vector<int> &shape);

        std::vector<int> inferShape(const py::object &obj);
        std::vector<float> flattenPythonData(const py::object &obj);
        py::object reshapeData(const std::vector<float> &data, const std::vector<int> &shape) const;

        std::string repr() const;

        const std::vector<int> getShape() const;
        py::object getData() const;
        py::object getFlattenedData() const;

        typedef void (*KernelFunc)(const float *, const float *, float *, int);
        typedef void (*KernelShiftFunc)(const float *, float *, int, int);
        typedef void (*KernelEmptyFunc)(const float *, float *, int);

        std::shared_ptr<Tensor> handleOperator(const Tensor &other, KernelFunc kernel) const;
        std::shared_ptr<Tensor> handleInPlaceOperator(const Tensor &other, KernelFunc kernel);
        std::shared_ptr<Tensor> handleShiftOperator(const int shift, KernelShiftFunc kernel) const;
        std::shared_ptr<Tensor> handleEmptyOperator(KernelEmptyFunc kernel) const;
        std::shared_ptr<Tensor> handleInPlaceShiftOperator(const int shift, KernelShiftFunc kernel);
        std::shared_ptr<Tensor> handleInPlaceEmptyOperator(KernelEmptyFunc kernel);

        // Basic arithmetic operators
        std::shared_ptr<Tensor> operator+(const Tensor &other) const;
        std::shared_ptr<Tensor> operator-(const Tensor &other) const;
        std::shared_ptr<Tensor> operator*(const Tensor &other) const;
        std::shared_ptr<Tensor> operator/(const Tensor &other) const;
        std::shared_ptr<Tensor> operator%(const Tensor &other) const;

        // In-place arithmetic operators
        std::shared_ptr<Tensor> operator+=(const Tensor &other);
        std::shared_ptr<Tensor> operator-=(const Tensor &other);
        std::shared_ptr<Tensor> operator*=(const Tensor &other);
        std::shared_ptr<Tensor> operator/=(const Tensor &other);
        std::shared_ptr<Tensor> operator%=(const Tensor &other);

        // Power operator
        std::shared_ptr<Tensor> pow(const Tensor &other) const;
        std::shared_ptr<Tensor> ipow(const Tensor &other);

        // Comparison operators
        std::shared_ptr<Tensor> operator==(const Tensor &other) const;
        std::shared_ptr<Tensor> operator!=(const Tensor &other) const;
        std::shared_ptr<Tensor> operator<(const Tensor &other) const;
        std::shared_ptr<Tensor> operator<=(const Tensor &other) const;
        std::shared_ptr<Tensor> operator>(const Tensor &other) const;
        std::shared_ptr<Tensor> operator>=(const Tensor &other) const;

        // Bitwise operators
        std::shared_ptr<Tensor> operator&(const Tensor &other) const;
        std::shared_ptr<Tensor> operator|(const Tensor &other) const;
        std::shared_ptr<Tensor> operator^(const Tensor &other) const;
        std::shared_ptr<Tensor> operator~() const;
        std::shared_ptr<Tensor> operator<<(int shift) const;
        std::shared_ptr<Tensor> operator>>(int shift) const;

        // In-place bitwise operators
        std::shared_ptr<Tensor> operator&=(const Tensor &other);
        std::shared_ptr<Tensor> operator|=(const Tensor &other);
        std::shared_ptr<Tensor> operator^=(const Tensor &other);
        std::shared_ptr<Tensor> operator<<=(int shift);
        std::shared_ptr<Tensor> operator>>=(int shift);

        // Matrix multiplication
        std::shared_ptr<Tensor> matmul(const Tensor &other) const;

        // Logical operators (for boolean tensors)
        // std::shared_ptr<Tensor> logical_and(const Tensor &other) const;
        // std::shared_ptr<Tensor> logical_or(const Tensor &other) const;
        // std::shared_ptr<Tensor> logical_xor(const Tensor &other) const;
        // std::shared_ptr<Tensor> logical_not() const;

        // Unary operations
        // std::shared_ptr<Tensor> abs() const;  // Absolute value
        // std::shared_ptr<Tensor> sqrt() const; // Square root
        // std::shared_ptr<Tensor> exp() const;  // Exponential
        // std::shared_ptr<Tensor> log() const;  // Natural logarithm
        // std::shared_ptr<Tensor> sin() const;  // Sine
        // std::shared_ptr<Tensor> cos() const;  // Cosine
        // std::shared_ptr<Tensor> tan() const;  // Tangent

        // Floor, ceil, and rounding
        // std::shared_ptr<Tensor> floor() const;
        // std::shared_ptr<Tensor> ceil() const;
        // std::shared_ptr<Tensor> round() const;
    };
}
