#pragma once
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include "../tensors/Tensor.h"

namespace py = pybind11;

inline void bind_tensor(py::module &m)
{
     py::class_<ryupy::Tensor, std::shared_ptr<ryupy::Tensor>>(m, "Tensor")
         .def_property_readonly("shape", &ryupy::Tensor::getShape)
         .def_property_readonly("flattenedData", &ryupy::Tensor::getFlattenedData)
         .def_property_readonly("data", &ryupy::Tensor::getData)

         .def_property("grad", [](ryupy::Tensor &t)
                       { return t.grad; }, [](ryupy::Tensor &t, std::shared_ptr<ryupy::Tensor> new_grad)
                       { t.grad = new_grad; })
         .def_readwrite("requires_grad", &ryupy::Tensor::requires_grad)
         .def("backward", &ryupy::Tensor::backward, py::arg("gradient") = nullptr)
         .def_readwrite("prev", &ryupy::Tensor::prev)

         .def("reshape", &ryupy::Tensor::reshape)
         .def("transpose", &ryupy::Tensor::transpose)

         .def("broadcast_to", &ryupy::Tensor::broadcast_to)
         .def("is_broadcastable_to", &ryupy::Tensor::is_broadcastable_to)

         .def("copy", &ryupy::Tensor::copy)
         .def("__repr__", &ryupy::Tensor::repr)
         .def("__getitem__", &ryupy::Tensor::getItem)
         .def("__setitem__", &ryupy::Tensor::setItem)

         // Arithmetic operators
         .def("__add__", &ryupy::Tensor::operator+)
         .def("__sub__", &ryupy::Tensor::operator-)
         .def("__mul__", &ryupy::Tensor::operator*)
         .def("__truediv__", &ryupy::Tensor::operator/)
         .def("__mod__", &ryupy::Tensor::operator%)

         // In-place arithmetic operators
         .def("__iadd__", &ryupy::Tensor::operator+=)
         .def("__isub__", &ryupy::Tensor::operator-=)
         .def("__imul__", &ryupy::Tensor::operator*=)
         .def("__itruediv__", &ryupy::Tensor::operator/=)
         .def("__imod__", &ryupy::Tensor::operator%=)

         // Power operators
         .def("__pow__", &ryupy::Tensor::pow)
         .def("__ipow__", &ryupy::Tensor::ipow)

         // Comparison operators
         .def("__eq__", &ryupy::Tensor::operator==)
         .def("__ne__", &ryupy::Tensor::operator!=)
         .def("__lt__", &ryupy::Tensor::operator<)
         .def("__le__", &ryupy::Tensor::operator<=)
         .def("__gt__", &ryupy::Tensor::operator>)
         .def("__ge__", &ryupy::Tensor::operator>=)

         // Bitwise operators
         .def("__and__", &ryupy::Tensor::operator&)
         .def("__or__", &ryupy::Tensor::operator|)
         .def("__xor__", &ryupy::Tensor::operator^)
         .def("__invert__", &ryupy::Tensor::operator~)
         .def("__lshift__", &ryupy::Tensor::operator<<)
         .def("__rshift__", &ryupy::Tensor::operator>>)

         // In-place bitwise operators
         .def("__iand__", &ryupy::Tensor::operator&=)
         .def("__ior__", &ryupy::Tensor::operator|=)
         .def("__ixor__", &ryupy::Tensor::operator^=)
         .def("__ilshift__", &ryupy::Tensor::operator<<=)
         .def("__irshift__", &ryupy::Tensor::operator>>=)

         .def("sum", &ryupy::Tensor::sum, py::arg("dim") = std::nullopt, py::arg("keepdim") = false)
         .def("__neg__", &ryupy::Tensor::negate)
         .def("__matmul__", &ryupy::Tensor::matmul);

     m.def("zeros", &ryupy::Tensor::zeros,
           py::arg("shape"),
           py::kw_only(),
           py::arg("grad") = false)
         .def("ones", &ryupy::Tensor::ones,
              py::arg("shape"),
              py::kw_only(),
              py::arg("grad") = false)
         .def("fill", &ryupy::Tensor::fill,
              py::arg("shape"),
              py::arg("value"),
              py::kw_only(),
              py::arg("grad") = false)
         .def("arange", &ryupy::Tensor::arange,
              py::arg("start"),
              py::arg("stop"),
              py::kw_only(),
              py::arg("step") = 1.0f,
              py::arg("grad") = false)
         .def("linspace", &ryupy::Tensor::linspace,
              py::arg("start"),
              py::arg("stop"),
              py::arg("num"),
              py::kw_only(),
              py::arg("grad") = false)
         .def("rand", &ryupy::Tensor::random_uniform,
              py::arg("shape"),
              py::kw_only(),
              py::arg("low") = 0.0f,
              py::arg("high") = 1.0f,
              py::arg("grad") = false)
         .def("randn", &ryupy::Tensor::random_normal,
              py::arg("shape"),
              py::kw_only(),
              py::arg("mean") = 0.0f,
              py::arg("std") = 1.0f,
              py::arg("grad") = false);
}