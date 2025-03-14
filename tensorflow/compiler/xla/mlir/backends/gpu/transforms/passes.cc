/* Copyright 2022 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "tensorflow/compiler/xla/mlir/backends/gpu/transforms/passes.h"

#include "mlir/Pass/PassManager.h"  // from @llvm-project
#include "mlir/Transforms/Passes.h"  // from @llvm-project

namespace xla {
namespace gpu {

using namespace mlir;  // NOLINT

void populateXlaGpuRuntimePasses(mlir::OpPassManager& pm,
                                 ThunkSequence* thunk_sequence,
                                 const GpuPipelineOpts& opts) {
  // Lower operations with registered IR emitters to Gpu launches.
  pm.addPass(createConvertLmhloToGpuLaunchPass(thunk_sequence));

  // Clean up IR before converting it to the runtime operations.
  pm.addPass(createCSEPass());

  // Convert global memrefs corresponding to constant arguments.
  pm.addPass(createConvertMemrefGetGlobalToArgPass());
  pm.addPass(createSymbolDCEPass());  // Clean up unused global constants.

  // Outline CUDA-Graph-compatible operations into graph capture functions.
  pm.addPass(
      createOutlineCudaGraphsPass(opts.cuda_graph_level, opts.min_graph_size));
  if (opts.enable_concurrent_region) {
    pm.addPass(createAddConcurrentRegionsPass());
  }

  // Lower all Gpu operations to the XLA Gpu runtime custom calls.
  pm.addPass(createConvertLmhloGpuToGpuRuntimePass());
  pm.addPass(createConvertLmhloToGpuRuntimePass());
  pm.addPass(createConvertGpuToGpuRuntimePass());

  // Add performance tracing annotations.
  pm.addPass(createAddHloTraceAnnotationsPass());
}

}  // namespace gpu
}  // namespace xla
