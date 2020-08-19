// Copyright 2020 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "integrations/tensorflow/compiler/dialect/tf_tensorlist/ir/tf_tensorlist_dialect.h"

#include "integrations/tensorflow/compiler/dialect/tf_tensorlist/conversion/convert_flow_to_hal.h"
#include "mlir/IR/DialectImplementation.h"

namespace mlir {
namespace tf_tensorlist {

static DialectRegistration<TfTensorListDialect> registration;

//===----------------------------------------------------------------------===//
// TfTensorListDialect Dialect
//===----------------------------------------------------------------------===//

TfTensorListDialect::TfTensorListDialect(MLIRContext *context)
    : Dialect(getDialectNamespace(), context,
              TypeID::get<TfTensorListDialect>()) {
  addInterfaces<iree_compiler::TfTensorListToHALConversionInterface>();
  addOperations<
#define GET_OP_LIST
#include "integrations/tensorflow/compiler/dialect/tf_tensorlist/ir/tf_tensorlist_ops.cc.inc"
      >();
  addTypes<TensorListType>();
}

Type TfTensorListDialect::parseType(DialectAsmParser &parser) const {
  StringRef type_name;
  if (parser.parseKeyword(&type_name)) return nullptr;
  if (type_name == "list") {
    return TensorListType::get(getContext());
  }
  parser.emitError(parser.getCurrentLocation(),
                   "unknown type in `tf_tensorlist` dialect");
  return nullptr;
}

void TfTensorListDialect::printType(Type type,
                                    DialectAsmPrinter &printer) const {
  printer << "list";
}

}  // namespace tf_tensorlist
}  // namespace mlir