#include <torch/extension.h>
#include "ATen/ATen.h"
#include <iostream>
typedef at::Half fp16;

void cuda_mm8_seq(int B, int N, int M,
                  fp16 *x, int x_stride,
                  uint8_t *w, int w_stride,

                  fp16 *y, int y_stride);

void mm8_seq(int64_t B, int64_t N, int64_t M,
             torch::Tensor &x, torch::Tensor &w,
             torch::Tensor &y)
{
    // assert(x.stride(1) == 1);
    // assert(w.stride(0) == 1);

    // assert(y.stride(1) == 1);
    cuda_mm8_seq(
        B, N, M,
        x.data_ptr<fp16>(), x.stride(0),
        w.data_ptr<uint8_t>(), w.stride(1),
        y.data_ptr<fp16>(), y.stride(0));
}

PYBIND11_MODULE(TORCH_EXTENSION_NAME, m)
{
    m.def("mm8_seq", &mm8_seq, "mm8 seq");
}

TORCH_LIBRARY(rwkv, m)
{
    m.def("mm8_seq", mm8_seq);
}