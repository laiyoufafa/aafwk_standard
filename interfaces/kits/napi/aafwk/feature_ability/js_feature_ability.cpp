/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include "js_feature_ability.h"

#include "distribute_constants.h"
#include "distribute_req_param.h"
#include "js_runtime_utils.h"
#include "hilog_wrapper.h"
#include "napi_common_util.h"

namespace OHOS {
namespace AbilityRuntime {
using namespace OHOS::AppExecFwk;

void JsFeatureAbility::Finalizer(NativeEngine* engine, void* data, void* hint)
{
    HILOG_INFO("JsFeatureAbility::Finalizer is called");
    std::unique_ptr<JsFeatureAbility>(static_cast<JsFeatureAbility*>(data));
}

NativeValue* JsFeatureAbility::StartAbility(NativeEngine* engine, NativeCallbackInfo* info)
{
    JsFeatureAbility* me = CheckParamsAndGetThis<JsFeatureAbility>(engine, info);
    return (me != nullptr) ? me->OnStartAbility(*engine, *info) : nullptr;
}

NativeValue* JsFeatureAbility::StartAbilityForResult(NativeEngine* engine, NativeCallbackInfo* info)
{
    JsFeatureAbility* me = CheckParamsAndGetThis<JsFeatureAbility>(engine, info);
    return (me != nullptr) ? me->OnStartAbilityForResult(*engine, *info) : nullptr;
}

NativeValue* JsFeatureAbility::OnStartAbility(NativeEngine &engine, NativeCallbackInfo &info)
{
    HILOG_INFO("%{public}s is called", __FUNCTION__);
    if (info.argc != 1) {
        HILOG_ERROR("Params not match");
        return engine.CreateUndefined();
    }

    Ability* ability = GetAbility(reinterpret_cast<napi_env>(&engine));
    if (ability == nullptr) {
        HILOG_ERROR("ability is nullptr");
        return engine.CreateUndefined();
    }

    DistributeReqParam requestParam;
    if (!UnWrapRequestParams(reinterpret_cast<napi_env>(&engine), reinterpret_cast<napi_value>(info.argv[0]),
        requestParam)) {
        HILOG_ERROR("unwrap request params failed");
        return engine.CreateUndefined();
    }

    Want want = GetWant(requestParam);
    AsyncTask::CompleteCallback complete =
        [want, ability](NativeEngine &engine, AsyncTask &task, int32_t status) {
            auto errcode = ability->StartAbility(want);
            task.Resolve(engine, JsFeatureAbility::CreateJsResult(engine, errcode, "Start Ability failed."));
        };

    NativeValue* result = nullptr;
    AsyncTask::Schedule(
        engine, CreateAsyncTaskWithLastParam(engine, nullptr, nullptr, std::move(complete), &result));
    return result;
}

NativeValue* JsFeatureAbility::OnStartAbilityForResult(NativeEngine& engine, NativeCallbackInfo& info)
{
    HILOG_INFO("%{public}s is called", __FUNCTION__);
    if (info.argc != 1) {
        HILOG_ERROR("Params not match");
        return engine.CreateUndefined();
    }
    Ability* ability = GetAbility(reinterpret_cast<napi_env>(&engine));
    if (ability == nullptr) {
        HILOG_ERROR("ability is nullptr");
        return engine.CreateUndefined();
    }

    DistributeReqParam requestParam;
    if (!UnWrapRequestParams(reinterpret_cast<napi_env>(&engine), reinterpret_cast<napi_value>(info.argv[0]),
        requestParam)) {
        HILOG_ERROR("unwrap request params failed");
        return engine.CreateUndefined();
    }

    Want want = GetWant(requestParam);
    requestCode_ = (requestCode_ == INT_MAX) ? 0 : (requestCode_ + 1);
    AsyncTask::CompleteCallback complete =
        [want, ability, requestCode = requestCode_](NativeEngine &engine, AsyncTask &task, int32_t status) {
            auto errcode = ability->StartAbilityForResult(want, requestCode);
            task.Resolve(engine, JsFeatureAbility::CreateJsResult(engine, errcode, "Start Ability failed."));
        };

    NativeValue* result = nullptr;
    AsyncTask::Schedule(
        engine, CreateAsyncTaskWithLastParam(engine, nullptr, nullptr, std::move(complete), &result));
    return result;
}

Ability* JsFeatureAbility::GetAbility(napi_env env)
{
    napi_status ret;
    napi_value global = 0;
    const napi_extended_error_info *errorInfo = nullptr;
    ret = napi_get_global(env, &global);
    if (ret != napi_ok) {
        napi_get_last_error_info(env, &errorInfo);
        HILOG_ERROR("get_global=%{public}d err:%{public}s", ret, errorInfo->error_message);
        return nullptr;
    }

    napi_value abilityObj = 0;
    ret = napi_get_named_property(env, global, "ability", &abilityObj);
    if (ret != napi_ok) {
        napi_get_last_error_info(env, &errorInfo);
        HILOG_ERROR("get_named_property=%{public}d err:%{public}s", ret, errorInfo->error_message);
        return nullptr;
    }

    Ability* ability = nullptr;
    ret = napi_get_value_external(env, abilityObj, (void **)&ability);
    if (ret != napi_ok) {
        napi_get_last_error_info(env, &errorInfo);
        HILOG_ERROR("get_value_external=%{public}d err:%{public}s", ret, errorInfo->error_message);
    return nullptr;
    }

    return ability;
}

Want JsFeatureAbility::GetWant(DistributeReqParam &requestParam)
{
    Want want;
    Uri parseUri("");
    if (CheckThenGetDeepLinkUri(requestParam, parseUri)) {
        want.SetUri(parseUri);
        want.SetAction(requestParam.GetAction());
        for (auto entity : requestParam.GetEntities()) {
            want.AddEntity(entity);
        }
        if (!requestParam.GetType().empty()) {
            want.SetType(requestParam.GetType());
        }
        return want;
    }

    if (requestParam.GetDeviceType() == DistributeConstants::DEVICE_TYPE_DEFAULT) {
        want.AddFlags(want.FLAG_ABILITYSLICE_MULTI_DEVICE);
        want.AddFlags(want.FLAG_NOT_OHOS_COMPONENT);
        want.SetDeviceId(requestParam.GetNetworkId());
    }

    if (requestParam.GetDeviceType() == DistributeConstants::DEVICE_TYPE_LOCAL) {
        want.AddFlags(want.FLAG_NOT_OHOS_COMPONENT);
    }
    want.AddFlags(requestParam.GetFlag());

    if (!requestParam.GetData().empty()) {
        want.SetParam(DistributeConstants::START_ABILITY_PARAMS_KEY, requestParam.GetData());
    }

    if (!requestParam.GetUrl().empty()) {
        want.SetParam(DistributeConstants::START_ABILITY_URL_KEY, requestParam.GetUrl());
        want.SetUri(Uri(requestParam.GetUrl()));
    }

    if (!requestParam.GetType().empty()) {
        want.SetType(requestParam.GetType());
    }

    GetExtraParams(requestParam, want);

    if (!requestParam.GetBundleName().empty() && !requestParam.GetAbilityName().empty()) {
        want.SetElementName(requestParam.GetNetworkId(), requestParam.GetBundleName(),
            requestParam.GetAbilityName());
    } else {
        want.SetAction(requestParam.GetAction());
        for (auto entity : requestParam.GetEntities()) {
            want.AddEntity(entity);
        }
    }

    return want;
}

void JsFeatureAbility::GetExtraParams(DistributeReqParam &requestParam, Want &want)
{
    return;
}

bool JsFeatureAbility::CheckThenGetDeepLinkUri(DistributeReqParam &requestParam, Uri &uri)
{
    std::string url = requestParam.GetUrl();
    std::string action = requestParam.GetAction();
    if (url.empty() || action.empty()) {
        return false;
    }

    if (action != DistributeConstants::DEEP_LINK_ACTION_NAME) {
        return false;
    }

    uri = Uri(url);
    if (uri.GetScheme().empty() || uri.GetHost().empty()) {
        return false;
    }

    return true;
}

bool JsFeatureAbility::UnWrapRequestParams(napi_env env, napi_value param, DistributeReqParam &requestParam)
{
    HILOG_INFO("%{public}s called.", __func__);

    if (!IsTypeForNapiValue(env, param, napi_object)) {
        HILOG_INFO("%{public}s called. Params is invalid.", __func__);
        return false;
    }

    std::string bundleName;
    if (UnwrapStringByPropertyName(env, param, "bundleName", bundleName)) {
        requestParam.SetBundleName(bundleName);
    }

    std::string abilityName;
    if (UnwrapStringByPropertyName(env, param, "abilityName", abilityName)) {
        requestParam.SetAbilityName(abilityName);
    }

    std::vector<std::string> entities;
    if (UnwrapStringArrayByPropertyName(env, param, "entities", entities)) {
        requestParam.SetEntities(entities);
    }

    std::string action;
    if (UnwrapStringByPropertyName(env, param, "action", action)) {
        requestParam.SetAction(action);
    }

    std::string networkId;
    if (UnwrapStringByPropertyName(env, param, "networkId", networkId)) {
        requestParam.SetNetWorkId(networkId);
    }

    int32_t deviceType = 0;
    if (UnwrapInt32ByPropertyName(env, param, "deviceType", deviceType)) {
        requestParam.SetDeviceType(deviceType);
    }

    std::string data;
    if (UnwrapStringByPropertyName(env, param, "data", data)) {
        requestParam.SetData(data);
    }

    int32_t flag = 0;
    if (UnwrapInt32ByPropertyName(env, param, "flag", flag)) {
        requestParam.SetFlag(flag);
    }

    std::string url;
    if (UnwrapStringByPropertyName(env, param, "url", url)) {
        requestParam.SetUrl(url);
    }

    return true;
}

NativeValue* JsFeatureAbility::CreateJsResult(NativeEngine &engine, int32_t errCode, const std::string &message)
{
    NativeValue* jsResult = engine.CreateObject();
    NativeObject* result = ConvertNativeValueTo<NativeObject>(jsResult);
    result->SetProperty("code", engine.CreateNumber(errCode));
    if (errCode == 0) {
        result->SetProperty("data", engine.CreateUndefined());
    } else {
        result->SetProperty("data", engine.CreateString(message.c_str(), message.length()));
    }

    return jsResult;
}

NativeValue* JsFeatureAbility::CreateJsFeatureAbility(NativeEngine &engine)
{
    NativeValue* objValue = engine.CreateObject();
    NativeObject* object = ConvertNativeValueTo<NativeObject>(objValue);

    std::unique_ptr<JsFeatureAbility> jsFeatureAbility = std::make_unique<JsFeatureAbility>();
    object->SetNativePointer(jsFeatureAbility.release(), JsFeatureAbility::Finalizer, nullptr);

    BindNativeFunction(engine, *object, "startAbility", JsFeatureAbility::StartAbility);
    BindNativeFunction(engine, *object, "startAbilityForResult", JsFeatureAbility::StartAbilityForResult);

    return objValue;
}

NativeValue* JsFeatureAbilityInit(NativeEngine* engine, NativeValue* exportObj)
{
    HILOG_INFO("%{public}s called.", __func__);
    if (engine == nullptr) {
        HILOG_ERROR("%{public}s engine nullptr.", __func__);
        return nullptr;
    }

    NativeValue* global = engine->GetGlobal();
    if (global->TypeOf() != NATIVE_OBJECT) {
        HILOG_ERROR("global is not NativeObject");
        return nullptr;
    }

    NativeObject* object = ConvertNativeValueTo<NativeObject>(global);
    if (object == nullptr) {
        HILOG_ERROR("%{public}s convertNativeValueTo result is nullptr.", __func__);
        return nullptr;
    }

    object->SetProperty("FeatureAbility", JsFeatureAbility::CreateJsFeatureAbility(*engine));

    HILOG_INFO("%{public}s called end.", __func__);
    return global;
}
}  // namespace AbilityRuntime
}  // namespace OHOS