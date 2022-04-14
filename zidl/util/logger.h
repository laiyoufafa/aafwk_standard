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

#ifndef OHOS_ZIDL_LOGGER_H
#define OHOS_ZIDL_LOGGER_H

#include <stdarg.h>

namespace OHOS {
namespace Zidl {

class Logger {
public:
    static void D(
        /* [in] */ const char* tag,
        /* [in] */ const char* format, ...);

    static void E(
        /* [in] */ const char* tag,
        /* [in] */ const char* format, ...);

    static void V(
        /* [in] */ const char* tag,
        /* [in] */ const char* format, ...);

    inline static void SetLevel(
        /* [in] */ int level)
    {
        level_ = level;
    }

    static constexpr int VERBOSE = 0;
    static constexpr int DEBUG = 1;
    static constexpr int ERROR = 2;
    static constexpr int NOLOG = 3;

private:
    Logger();

    ~Logger();

    static void Log(
        /* [in] */ const char* tag,
        /* [in] */ const char* format,
        /* [in] */ va_list args);

    static void Err(
        /* [in] */ const char* tag,
        /* [in] */ const char* format,
        /* [in] */ va_list args);

    static int level_;
};

}
}

#endif // OHOS_ZIDL_LOGGER_H
