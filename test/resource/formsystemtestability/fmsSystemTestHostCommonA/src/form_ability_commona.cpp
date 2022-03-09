/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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
#include "form_ability_commona.h"
#include "hilog_wrapper.h"
#include "form_st_common_info.h"
#include "form_test_utils.h"
#include "system_test_form_util.h"

namespace {
using namespace OHOS::AAFwk;
using namespace OHOS::EventFwk;
}

namespace OHOS {
namespace AppExecFwk {
std::vector<std::string> eventList = {
    FORM_EVENT_REQ_ONE_NORMAL_FORM,
    FORM_EVENT_REQ_ONE_NORMAL_FORM_DEL,
    FORM_EVENT_REQ_ACQUIRE_FORM_BATCH,
    FORM_EVENT_REQ_CLEAR_FORM_BATCH
};
void FormAbilityCommonA::AcquireFormCallback::OnAcquired(const int32_t result, const FormJsInfo &formJsInfo) const
{
    HILOG_INFO("%{public}s called[%{public}s]", __func__, std::to_string(formJsInfo.formId).c_str());
}

void FormAbilityCommonA::AcquireFormCallback::OnUpdate(const int32_t result, const FormJsInfo &formJsInfo) const
{
    HILOG_INFO("%{public}s called", __func__);
    FormTestUtils::PublishEvent(FORM_EVENT_RECV_ONE_NORMAL_FORM, EVENT_CODE_100, std::to_string(formJsInfo.formId));
}

void FormAbilityCommonA::AcquireFormCallback::OnFormUninstall(const int64_t formId) const
{
    HILOG_INFO("%{public}s called", __func__);
}

// Create one form(temp/normal)
void FormAbilityCommonA::FMS_acquireForm(std::string data)
{
    HILOG_INFO("%{public}s called, data: %{public}s", __func__, data.c_str());
    std::shared_ptr<AcquireFormCallback> callback = std::make_shared<AcquireFormCallback>();
    // Set Want info begin
    Want want;
    want.SetParam(Constants::PARAM_FORM_DIMENSION_KEY, FORM_DIMENSION_1);
    want.SetParam(Constants::PARAM_FORM_NAME_KEY, PARAM_FORM_NAME1);
    want.SetParam(Constants::PARAM_MODULE_NAME_KEY, PARAM_PROVIDER_MODULE_NAME1);
    if (data == "true") {
        want.SetParam(Constants::PARAM_FORM_TEMPORARY_KEY, FORM_TEMP_FORM_FLAG_TRUE);
    } else {
        want.SetParam(Constants::PARAM_FORM_TEMPORARY_KEY, FORM_TEMP_FORM_FLAG_FALSE);
    }
    want.SetElementName(FORM_TEST_DEVICEID, FORM_PROVIDER_BUNDLE_NAME1, FORM_PROVIDER_ABILITY_NAME1);
    // Set Want info end
    bool bResult = AcquireForm(0, want, callback);
    if (bResult) {
        HILOG_INFO("[form_ability_commonA]AcquireForm end");
    } else {
        HILOG_ERROR("[form_ability_commonA]AcquireForm error");
        FormTestUtils::PublishEvent(FORM_EVENT_RECV_ONE_NORMAL_FORM, EVENT_CODE_100, "");
    }
}

void FormAbilityCommonA::FMS_deleteForm(std::string data)
{
    HILOG_INFO("%{public}s formId: %{public}s", __func__, data.c_str());
    bool bResult = DeleteForm(atoll(data.c_str()));
    if (bResult) {
        HILOG_INFO("%{public}s DeleteForm end", __func__);
        FormTestUtils::PublishEvent(FORM_EVENT_RECV_ONE_NORMAL_FORM_DEL, EVENT_CODE_101, "true");
    } else {
        HILOG_ERROR("%{public}s DeleteForm error", __func__);
        FormTestUtils::PublishEvent(FORM_EVENT_RECV_ONE_NORMAL_FORM_DEL, EVENT_CODE_101, "false");
    }
}

void FormAbilityCommonA::FMS_acquireForm_batch(std::string data)
{
    HILOG_INFO("%{public}s called", __func__);
    // Set Want info begin
    Want want;
    want.SetParam(Constants::PARAM_FORM_DIMENSION_KEY, FORM_DIMENSION_1);
    want.SetParam(Constants::PARAM_FORM_NAME_KEY, PARAM_FORM_NAME1);
    want.SetParam(Constants::PARAM_MODULE_NAME_KEY, PARAM_PROVIDER_MODULE_NAME1);
    want.SetParam(Constants::PARAM_FORM_TEMPORARY_KEY, FORM_TEMP_FORM_FLAG_FALSE);
    want.SetElementName(FORM_TEST_DEVICEID, FORM_PROVIDER_BUNDLE_NAME1, FORM_PROVIDER_ABILITY_NAME1);

    int formCount = std::stoi(data);
    HILOG_INFO("%{public}s, formCount: %{public}d", __func__, formCount);
    want.SetParam(Constants::PARAM_FORM_ADD_COUNT, formCount);
    // Set Want info end
    int result = STtools::SystemTestFormUtil::BatchAddFormRecords(want);
    if (result == ERR_OK) {
        HILOG_INFO("Batch add form end");
        FormTestUtils::PublishEvent(FORM_EVENT_RECV_ACQUIRE_FORM_BATCH, EVENT_CODE_BATCH, "true");
    } else {
        HILOG_ERROR("Batch add form error");
        FormTestUtils::PublishEvent(FORM_EVENT_RECV_ACQUIRE_FORM_BATCH, EVENT_CODE_BATCH, "false");
    }
}

void FormAbilityCommonA::FMS_deleteFormBatch(std::string strFormId)
{
    HILOG_INFO("%{public}s called", __func__);
    int result = STtools::SystemTestFormUtil::ClearFormRecords();
    if (result == ERR_OK) {
        HILOG_INFO("Clear form records end");
        FormTestUtils::PublishEvent(FORM_EVENT_RECV_CLEAR_FORM_BATCH, EVENT_CODE_CLEAR_BATCH, "true");
    } else {
        HILOG_ERROR("Clear form records error");
        FormTestUtils::PublishEvent(FORM_EVENT_RECV_CLEAR_FORM_BATCH, EVENT_CODE_CLEAR_BATCH, "false");
    }
}

FormAbilityCommonA::~FormAbilityCommonA()
{
    CommonEventManager::UnSubscribeCommonEvent(subscriber_);
}

void FormAbilityCommonA::OnStart(const Want &want)
{
    HILOG_INFO("FormAbilityCommonA::onStart");
    Ability::OnStart(want);
}

void FormAbilityCommonA::OnActive()
{
    HILOG_INFO("FormAbilityCommonA::OnActive");
    Ability::OnActive();
    std::string eventData = GetAbilityName() + FORM_ABILITY_STATE_ONACTIVE;
    FormTestUtils::PublishEvent(FORM_EVENT_ABILITY_ONACTIVED, 0, eventData);
}

void FormAbilityCommonA::OnStop()
{
    HILOG_INFO("FormAbilityCommonA::OnStop");

    Ability::OnStop();
}

void FormAbilityCommonA::OnInactive()
{
    HILOG_INFO("FormAbilityCommonA::OnInactive");

    Ability::OnInactive();
}

void FormAbilityCommonA::OnBackground()
{
    HILOG_INFO("FormAbilityCommonA::OnBackground");

    Ability::OnBackground();
}

void FormAbilityCommonA::SubscribeEvent()
{
    HILOG_INFO("FormAbilityCommonA::SubscribeEvent");
    MatchingSkills matchingSkills;
    for (const auto &e : eventList) {
        matchingSkills.AddEvent(e);
    }
    CommonEventSubscribeInfo subscribeInfo(matchingSkills);
    subscribeInfo.SetPriority(1);
    subscriber_ = std::make_shared<FormEventSubscriberForCommonA>(subscribeInfo);
    subscriber_->ability_ = this;
    CommonEventManager::SubscribeCommonEvent(subscriber_);
}

// KitTest End
void FormAbilityCommonA::Init(const std::shared_ptr<AbilityInfo> &abilityInfo,
    const std::shared_ptr<OHOSApplication> &application, std::shared_ptr<AbilityHandler> &handler,
    const sptr<IRemoteObject> &token)
{
    HILOG_INFO("FormAbilityCommonA::Init");
    Ability::Init(abilityInfo, application, handler, token);
    memberFuncMap_[FORM_EVENT_REQ_ONE_NORMAL_FORM] = &FormAbilityCommonA::FMS_acquireForm;
    memberFuncMap_[FORM_EVENT_REQ_ONE_NORMAL_FORM_DEL] = &FormAbilityCommonA::FMS_deleteForm;
    memberFuncMap_[FORM_EVENT_REQ_ACQUIRE_FORM_BATCH] = &FormAbilityCommonA::FMS_acquireForm_batch;
    memberFuncMap_[FORM_EVENT_REQ_CLEAR_FORM_BATCH] = &FormAbilityCommonA::FMS_deleteFormBatch;
    SubscribeEvent();
}

void FormAbilityCommonA::handleEvent(std::string action, std::string data)
{
    HILOG_INFO("%{public}s called", __func__);
    auto itFunc = memberFuncMap_.find(action);
    if (itFunc != memberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            return (this->*memberFunc)(data);
        }
    }
}

void FormEventSubscriberForCommonA::OnReceiveEvent(const CommonEventData &data)
{
    HILOG_INFO("FormEventSubscriberForCommonA::OnReceiveEvent:event=%{public}s", data.GetWant().GetAction().c_str());
    HILOG_INFO("KitTestEventSubscriber::OnReceiveEvent:data=%{public}s", data.GetData().c_str());
    HILOG_INFO("FormEventSubscriberForCommonA::OnReceiveEvent:code=%{public}d", data.GetCode());
    auto eventName = data.GetWant().GetAction();
    ability_->handleEvent(eventName, data.GetData());
    if (eventName == FORM_EVENT_REQ_ONE_NORMAL_FORM && data.GetCode() == EVENT_CODE_100) {
        return;
    }
    CommonEventManager::UnSubscribeCommonEvent(ability_->subscriber_);
}

void FormEventSubscriberForCommonA::KitTerminateAbility()
{
    if (ability_ != nullptr) {
        ability_->TerminateAbility();
    }
}

REGISTER_AA(FormAbilityCommonA)
}  // namespace AppExecFwk
}  // namespace OHOS