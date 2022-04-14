/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef OHOS_ZIDL_ASTPARAMETER_H
#define OHOS_ZIDL_ASTPARAMETER_H

#include "ast/ast_node.h"
#include "ast/ast_type.h"
#include "util/autoptr.h"
#include "util/string.h"

namespace OHOS {
namespace Zidl {

class ASTParameter : public ASTNode {
public:
    inline void SetName(
        /* [in] */ const String& name)
    {
        name_ = name;
    }

    inline String GetName()
    {
        return name_;
    }

    inline void SetType(
        /* [in] */ ASTType* type)
    {
        type_ = type;
    }

    inline AutoPtr<ASTType> GetType()
    {
        return type_;
    }

    inline void SetInParameter(
        /* [in] */ bool inParameter)
    {
        isInParameter_ = inParameter;
    }

    inline bool IsInParameter()
    {
        return isInParameter_;
    }

    inline void SetOutParameter(
        /* [in] */ bool outParameter)
    {
        isOutParameter_ = outParameter;
    }

    inline bool IsOutParameter()
    {
        return isOutParameter_;
    }

    String Dump(
        /* [in] */ const String& prefix) override;

private:
    String name_;
    AutoPtr<ASTType> type_;
    bool isInParameter_ = false;
    bool isOutParameter_ = false;
};

}
}

#endif // OHOS_ZIDL_ASTPARAMETER_H
