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

#include <cinttypes>

#include "appexecfwk_errors.h"
#include "bundle_info.h"
#include "common_event_manager.h"
#include "common_event_support.h"
#include "form_bms_helper.h"
#include "form_cache_mgr.h"
#include "form_constants.h"
#include "form_data_mgr.h"
#include "form_db_cache.h"
#include "form_db_info.h"
#include "form_info_mgr.h"
#include "form_provider_mgr.h"
#include "form_sys_event_receiver.h"
#include "form_timer_mgr.h"
#include "form_util.h"
#include "hilog_wrapper.h"
#include "in_process_call_wrapper.h"
#include "want.h"

namespace OHOS {
namespace AppExecFwk {
const std::string KEY_UID = "uid";
const std::string KEY_USER_ID = "userId";
/**
 * @brief Receiver Constructor.
 * @param subscriberInfo Subscriber info.
 */
FormSysEventReceiver::FormSysEventReceiver(const EventFwk::CommonEventSubscribeInfo &subscriberInfo)
    : EventFwk::CommonEventSubscriber(subscriberInfo)
{}
/**
 * @brief Receive common event.
 * @param eventData Common event data.
 */
void FormSysEventReceiver::OnReceiveEvent(const EventFwk::CommonEventData &eventData)
{
    const AAFwk::Want& want = eventData.GetWant();
    std::string action = want.GetAction();
    std::string bundleName = want.GetElement().GetBundleName();
    if (action.empty() || (action != EventFwk::CommonEventSupport::COMMON_EVENT_USER_REMOVED && bundleName.empty())) {
        HILOG_ERROR("%{public}s failed, invalid param, action: %{public}s, bundleName: %{public}s",
            __func__, action.c_str(), bundleName.c_str());
        return;
    }
    if (eventHandler_ == nullptr) {
        HILOG_ERROR("%{public}s fail, eventhandler invalidate.", __func__);
        return;
    }
    HILOG_INFO("%{public}s, action:%{public}s.", __func__, action.c_str());
    if (action == EventFwk::CommonEventSupport::COMMON_EVENT_PACKAGE_ADDED ||
        action == EventFwk::CommonEventSupport::COMMON_EVENT_PACKAGE_CHANGED) {
        // install or update
        auto task = [this, want, bundleName]() {
            HILOG_INFO("%{public}s, bundle changed, bundleName: %{public}s", __func__, bundleName.c_str());
            int userId = want.GetIntParam(KEY_USER_ID, 0);
            HandleBundleFormInfoChanged(bundleName);
            HandleProviderUpdated(bundleName, userId);
        };
        eventHandler_->PostTask(task);
    } else if (action == EventFwk::CommonEventSupport::COMMON_EVENT_PACKAGE_REMOVED) {
        auto task = [this, bundleName]() {
            HILOG_INFO("%{public}s, bundle removed, bundleName: %{public}s", __func__, bundleName.c_str());
            HandleBundleFormInfoRemoved(bundleName);
            HandleProviderRemoved(bundleName);
        };
        eventHandler_->PostTask(task);
    } else if (action == EventFwk::CommonEventSupport::COMMON_EVENT_ABILITY_UPDATED) {
        auto task = [this, want, bundleName]() {
            HILOG_INFO("%{public}s, bundle updated, bundleName: %{public}s", __func__, bundleName.c_str());
            int userId = want.GetIntParam(KEY_USER_ID, 0);
            HandleProviderUpdated(bundleName, userId);
        };
        eventHandler_->PostTask(task);
    } else if (action == EventFwk::CommonEventSupport::COMMON_EVENT_PACKAGE_DATA_CLEARED) {
        auto task = [this, want, bundleName]() {
            int userId = want.GetIntParam(KEY_USER_ID, 0);
            sptr<IBundleMgr> iBundleMgr = FormBmsHelper::GetInstance().GetBundleMgr();
            if (iBundleMgr == nullptr) {
                HILOG_ERROR("%{public}s error, failed to get IBundleMgr.", __func__);
                return;
            }
            int uid = IN_PROCESS_CALL(iBundleMgr->GetUidByBundleName(bundleName, userId));
            HandleBundleDataCleared(bundleName, uid);
        };
        eventHandler_->PostTask(task);
    } else if (action == EventFwk::CommonEventSupport::COMMON_EVENT_USER_REMOVED) {
        int32_t userId = eventData.GetCode();
        auto task = [this, userId]() {
            if (userId == -1) {
                HILOG_ERROR("%{public}s, failed to get userId", __func__);
                return;
            }
            HandleUserIdRemoved(userId);
        };
        eventHandler_->PostTask(task);
    } else {
        HILOG_WARN("%{public}s warnning, invalid action.", __func__);
    }
}
/**
 * @brief Handle provider updated event.
 * @param bundleName bundle name.
 * @param userId user ID.
 * @param uid uid.
 */
void FormSysEventReceiver::HandleProviderUpdated(const std::string &bundleName, const int userId)
{
    HILOG_INFO("%{public}s, bundleName:%{public}s, userId:%{public}d.", __func__, bundleName.c_str(), userId);
    std::vector<FormRecord> formInfos;
    if (!FormDataMgr::GetInstance().GetFormRecord(bundleName, formInfos)) {
        HILOG_INFO("%{public}s, no form info.", __func__);
        return;
    }

    sptr<IBundleMgr> iBundleMgr = FormBmsHelper::GetInstance().GetBundleMgr();
    if (iBundleMgr == nullptr) {
        // GetBundleMgr() has error log
        return;
    }

    std::vector<FormInfo> targetForms;
    if (FormInfoMgr::GetInstance().GetFormsInfoByBundle(bundleName, targetForms) != ERR_OK) {
        HILOG_ERROR("%{public}s error, failed to get forms info.", __func__);
        return;
    }

    std::vector<int64_t> removedForms;
    std::vector<int64_t> updatedForms;
    for (FormRecord& formRecord : formInfos) {
        HILOG_INFO("%{public}s, provider update, formName:%{public}s", __func__, formRecord.formName.c_str());
        int64_t formId = formRecord.formId;
        if (ProviderFormUpdated(formId, formRecord, targetForms)) {
            updatedForms.emplace_back(formId);
            continue;
        }

        HILOG_INFO("%{public}s, no such form anymore, delete it:%{public}s", __func__, formRecord.formName.c_str());
        if (formRecord.formTempFlg) {
            FormDataMgr::GetInstance().DeleteTempForm(formId);
        } else {
            FormDbCache::GetInstance().DeleteFormInfo(formId);
        }
        removedForms.emplace_back(formId);
        FormDataMgr::GetInstance().DeleteFormRecord(formId);
    }

    if (!removedForms.empty()) {
        HILOG_INFO("%{public}s, clean removed forms", __func__);
        FormDataMgr::GetInstance().CleanHostRemovedForms(removedForms);
    }

    HILOG_INFO("%{public}s, remove form timer", __func__);
    for (const int64_t id : removedForms) {
        FormTimerMgr::GetInstance().RemoveFormTimer(id);
    }

    HILOG_INFO("%{public}s, refresh form", __func__);
    Want want;
    want.SetParam(Constants::PARAM_FORM_USER_ID, userId);
    for (const int64_t id : updatedForms) {
        FormProviderMgr::GetInstance().RefreshForm(id, want, true);
    }
}

void FormSysEventReceiver::HandleProviderRemoved(const std::string &bundleName)
{
    HILOG_INFO("GET into HandleProviderRemoved with bundleName : %{public}s", bundleName.c_str());
    // clean removed form in DB
    std::set<int64_t> removedForms;
    {
        std::vector<FormDBInfo> removedDBForm;
        FormDbCache::GetInstance().DeleteFormInfoByBundleName(bundleName, removedDBForm);
        for (const auto &dbForm : removedDBForm) {
            removedForms.emplace(dbForm.formId);
            int32_t matchCount = FormDbCache::GetInstance().GetMatchCount(dbForm.bundleName, dbForm.moduleName);
            if (matchCount == 0) {
                FormBmsHelper::GetInstance().NotifyModuleRemovable(dbForm.bundleName, dbForm.moduleName);
            }
        }
    }
    // clean removed form in FormRecords
    FormDataMgr::GetInstance().CleanRemovedFormRecords(bundleName, removedForms);
    // clean removed temp form in FormRecords
    FormDataMgr::GetInstance().CleanRemovedTempFormRecords(bundleName, removedForms);
    // clean removed forms in FormHostRecords
    std::vector<int64_t> vRemovedForms;
    vRemovedForms.assign(removedForms.begin(), removedForms.end());
    FormDataMgr::GetInstance().CleanHostRemovedForms(vRemovedForms);

    // clean removed form timers
    for (auto &formId : removedForms) {
        FormTimerMgr::GetInstance().RemoveFormTimer(formId);
    }
}

bool FormSysEventReceiver::ProviderFormUpdated(const int64_t formId,
    const FormRecord &formRecord, const std::vector<FormInfo> &targetForms)
{
    HILOG_INFO("%{public}s start", __func__);
    if (targetForms.empty()) {
        HILOG_ERROR("%{public}s error, targetForms is empty", __func__);
        return false;
    }

    FormInfo updatedForm;
    bool bGetForm = FormDataMgr::GetInstance().GetUpdatedForm(formRecord, targetForms, updatedForm);
    if (bGetForm) {
        HILOG_INFO("%{public}s, form is still exist,form:%{public}s", __func__, formRecord.formName.c_str());
        // update resource
        FormDataMgr::GetInstance().SetNeedRefresh(formId, true);
        FormCacheMgr::GetInstance().DeleteData(formId);

        FormBmsHelper::GetInstance().NotifyModuleNotRemovable(formRecord.bundleName, formRecord.moduleName);
        FormTimerCfg timerCfg;
        GetTimerCfg(updatedForm.updateEnabled, updatedForm.updateDuration,
            updatedForm.scheduledUpdateTime, timerCfg);
        HandleTimerUpdate(formId, formRecord, timerCfg);
        FormDataMgr::GetInstance().SetVersionUpgrade(formId, true);
        return true;
    }
    HILOG_INFO("%{public}s, no updated form.", __func__);
    return false;
}

void FormSysEventReceiver::HandleBundleFormInfoChanged(const std::string &bundleName)
{
    FormInfoMgr::GetInstance().Update(bundleName);
}

void FormSysEventReceiver::HandleBundleFormInfoRemoved(const std::string &bundleName)
{
    FormInfoMgr::GetInstance().Remove(bundleName);
}

void FormSysEventReceiver::HandleBundleDataCleared(const std::string &bundleName, const int uid)
{
    HILOG_DEBUG("%{public}s, bundleName:%{public}s, uid:%{public}d", __func__, bundleName.c_str(), uid);
    // as provider data is cleared
    std::set<int64_t> reCreateForms;
    FormDataMgr::GetInstance().GetReCreateFormRecordsByBundleName(bundleName, reCreateForms);
    if (!reCreateForms.empty()) {
        for (int64_t formId : reCreateForms) {
            ReCreateForm(formId);
        }
    }

    // as form host data is cleared
    HandleFormHostDataCleared(uid);
}
void FormSysEventReceiver::HandleFormHostDataCleared(const int uid)
{
    HILOG_DEBUG("%{public}s, uid:%{public}d", __func__, uid);
    std::map<int64_t, bool> removedFormsMap;
    // clear formDBRecord
    ClearFormDBRecordData(uid, removedFormsMap);

    // clear temp form
    ClearTempFormRecordData(uid, removedFormsMap);

    // clear host data
    FormDataMgr::GetInstance().ClearHostDataByUId(uid);

    // delete forms timer
    for (const auto &removedForm : removedFormsMap) {
        if (removedForm.second) {
            FormTimerMgr::GetInstance().RemoveFormTimer(removedForm.first);
        }
    }
}
void FormSysEventReceiver::ClearFormDBRecordData(const int uid, std::map<int64_t, bool> &removedFormsMap)
{
    std::map<int64_t, bool> foundFormsMap;
    std::map<FormIdKey, std::set<int64_t>> noHostFormDbMap;
    FormDbCache::GetInstance().GetNoHostDBForms(uid, noHostFormDbMap, foundFormsMap);
    if (foundFormsMap.size() > 0) {
        for (const auto &element : foundFormsMap) {
            FormDataMgr::GetInstance().DeleteFormUserUid(element.first, uid);
        }
    }

    HILOG_DEBUG("%{public}s, noHostFormDbMap size:%{public}zu", __func__, noHostFormDbMap.size());
    if (noHostFormDbMap.size() > 0) {
        BatchDeleteNoHostDBForms(uid, noHostFormDbMap, foundFormsMap);
    }

    if (!foundFormsMap.empty()) {
        removedFormsMap.insert(foundFormsMap.begin(), foundFormsMap.end());
    }
}
void FormSysEventReceiver::ClearTempFormRecordData(const int uid, std::map<int64_t, bool> &removedFormsMap)
{
    std::map<int64_t, bool> foundFormsMap;
    std::map<FormIdKey, std::set<int64_t>> noHostTempFormsMap;
    FormDataMgr::GetInstance().GetNoHostTempForms(uid, noHostTempFormsMap, foundFormsMap);
    HILOG_DEBUG("%{public}s, noHostTempFormsMap size:%{public}zu", __func__, noHostTempFormsMap.size());
    if (noHostTempFormsMap.size() > 0) {
        BatchDeleteNoHostTempForms(uid, noHostTempFormsMap, foundFormsMap);
    }
    if (!foundFormsMap.empty()) {
        removedFormsMap.insert(foundFormsMap.begin(), foundFormsMap.end());
    }
}
void FormSysEventReceiver::BatchDeleteNoHostDBForms(const int uid, std::map<FormIdKey,
    std::set<int64_t>> &noHostFormDbMap, std::map<int64_t, bool> &removedFormsMap)
{
    std::set<FormIdKey> removableModuleSet;
    for (const auto &element: noHostFormDbMap) {
        std::set<int64_t> formIds = element.second;
        FormIdKey formIdKey = element.first;
        std::string bundleName = formIdKey.bundleName;
        std::string abilityName = formIdKey.abilityName;
        int result = FormProviderMgr::GetInstance().NotifyProviderFormsBatchDelete(bundleName, abilityName, formIds);
        if (result != ERR_OK) {
            HILOG_ERROR("%{public}s error, NotifyProviderFormsBatchDelete failed! bundleName:%{public}s,\
            abilityName:%{public}s",
                __func__, bundleName.c_str(), abilityName.c_str());
            for (int64_t formId : formIds) {
                FormDBInfo dbInfo;
                int errCode = FormDbCache::GetInstance().GetDBRecord(formId, dbInfo);
                if (errCode == ERR_OK) {
                    dbInfo.formUserUids.emplace_back(uid);
                    FormDbCache::GetInstance().SaveFormInfo(dbInfo);
                }
            }
        } else {
            for (const int64_t formId : formIds) {
                removedFormsMap.emplace(formId, true);
                FormDBInfo dbInfo;
                int errCode = FormDbCache::GetInstance().GetDBRecord(formId, dbInfo);
                if (errCode == ERR_OK) {
                    FormIdKey removableModuleFormIdKey(dbInfo.bundleName, dbInfo.moduleName);
                    removableModuleSet.emplace(removableModuleFormIdKey);
                    FormDbCache::GetInstance().DeleteFormInfo(formId);
                }
                FormDataMgr::GetInstance().DeleteFormRecord(formId);
            }
        }
    }

    for (const FormIdKey &item : removableModuleSet) {
        int32_t matchCount = FormDbCache::GetInstance().GetMatchCount(item.bundleName, item.moduleName);
        if (matchCount == 0) {
            FormBmsHelper::GetInstance().NotifyModuleRemovable(item.bundleName, item.moduleName);
        }
    }
}
/**
 * @brief Delete no host temp forms.
 * @param uid The caller uid.
 * @param noHostTempFormsMap no host temp forms.
 * @param foundFormsMap Form Id list.
 */
void FormSysEventReceiver::BatchDeleteNoHostTempForms(const int uid, std::map<FormIdKey,
    std::set<int64_t>> &noHostTempFormsMap, std::map<int64_t, bool> &foundFormsMap)
{
    for (const auto &element : noHostTempFormsMap) {
        std::set<int64_t> formIds = element.second;
        FormIdKey formIdKey = element.first;
        std::string bundleName = formIdKey.bundleName;
        std::string abilityName = formIdKey.abilityName;
        int result = FormProviderMgr::GetInstance().NotifyProviderFormsBatchDelete(bundleName, abilityName, formIds);
        if (result != ERR_OK) {
            HILOG_ERROR("%{public}s error, NotifyProviderFormsBatchDelete failed! bundleName:%{public}s,\
            abilityName:%{public}s",
                __func__, bundleName.c_str(), abilityName.c_str());
            for (int64_t formId : formIds) {
                FormDataMgr::GetInstance().AddFormUserUid(formId, uid);
            }
        } else {
            for (int64_t formId : formIds) {
                foundFormsMap.emplace(formId, true);
                FormDataMgr::GetInstance().DeleteFormRecord(formId);
                FormDataMgr::GetInstance().DeleteTempForm(formId);
            }
        }
    }
}
void FormSysEventReceiver::ReCreateForm(const int64_t formId)
{
    HILOG_INFO("%{public}s start, formId:%{public}" PRId64 "", __func__, formId);
    FormRecord reCreateRecord;
    FormRecord record;
    bool isGetForm = FormDataMgr::GetInstance().GetFormRecord(formId, record);
    if (!isGetForm) {
        HILOG_ERROR("%{public}s error, not exist such form:%{public}" PRId64 "", __func__, formId);
        return;
    }
    FormCacheMgr::GetInstance().DeleteData(formId);

    reCreateRecord.bundleName = record.bundleName;
    reCreateRecord.abilityName = record.abilityName;
    reCreateRecord.formName = record.formName;
    reCreateRecord.specification = record.specification;
    reCreateRecord.formTempFlg = record.formTempFlg;
    reCreateRecord.isInited = record.isInited;
    reCreateRecord.versionUpgrade = record.versionUpgrade;

    Want want;
    FormUtil::CreateFormWant(reCreateRecord.formName, reCreateRecord.specification, reCreateRecord.formTempFlg, want);
    want.SetParam(Constants::RECREATE_FORM_KEY, true);
    FormProviderMgr::GetInstance().ConnectAmsForRefresh(formId, reCreateRecord, want, false);
}
void FormSysEventReceiver::GetTimerCfg(const bool updateEnabled,
    const int updateDuration, const std::string &configUpdateAt, FormTimerCfg& cfg)
{
    HILOG_INFO("%{public}s start", __func__);
    if (!updateEnabled) {
        HILOG_INFO("%{public}s, update disable", __func__);
        return;
    }

    if (updateDuration > 0) {
        // interval timer
        HILOG_INFO("%{public}s,interval timer updateDuration:%{public}d", __func__, updateDuration);
        if (updateDuration <= Constants::MIN_CONFIG_DURATION) {
            cfg.updateDuration = Constants::MIN_PERIOD;
        } else if (updateDuration >= Constants::MAX_CONFIG_DURATION) {
            cfg.updateDuration = Constants::MAX_PERIOD;
        } else {
            cfg.updateDuration = updateDuration * Constants::TIME_CONVERSION;
        }
        cfg.enableUpdate = true;
        return;
    } else {
        // updateAtTimer
        if (configUpdateAt.empty()) {
            HILOG_INFO("%{public}s, configUpdateAt is empty", __func__);
            return;
        }
        HILOG_INFO("%{public}s,update at timer updateAt:%{public}s", __func__, configUpdateAt.c_str());

        std::vector<std::string> temp = FormUtil::StringSplit(configUpdateAt, Constants::TIME_DELIMETER);
        if (temp.size() != Constants::UPDATE_AT_CONFIG_COUNT) {
            HILOG_ERROR("%{public}s, invalid config", __func__);
            return;
        }
        int hour = -1;
        int min = -1;
        hour = std::stoi(temp[0]);
        min = std::stoi(temp[1]);
        if (hour < Constants::MIN_TIME || hour > Constants::MAX_HOUR || min < Constants::MIN_TIME || min >
            Constants::MAX_MINUTE) {
            HILOG_ERROR("%{public}s, time is invalid", __func__);
            return;
        }

        cfg.updateAtHour = hour;
        cfg.updateAtMin = min;
        cfg.enableUpdate = true;
        return;
    }
}

void FormSysEventReceiver::HandleTimerUpdate(const int64_t formId,
    const FormRecord &record, const FormTimerCfg &timerCfg)
{
    // both disable
    if (!record.isEnableUpdate && !timerCfg.enableUpdate) {
        return;
    }

    // enable to disable
    if (record.isEnableUpdate && !timerCfg.enableUpdate) {
        FormDataMgr::GetInstance().SetEnableUpdate(formId, false);
        FormTimerMgr::GetInstance().RemoveFormTimer(formId);
        return;
    }

    // disable to enable
    if (!record.isEnableUpdate && timerCfg.enableUpdate) {
        FormDataMgr::GetInstance().SetUpdateInfo(formId, true,
            timerCfg.updateDuration, timerCfg.updateAtHour, timerCfg.updateAtMin);
        if (timerCfg.updateDuration > 0) {
            HILOG_INFO("%{public}s, add interval timer:%{public}" PRId64 "", __func__, timerCfg.updateDuration);
            FormTimerMgr::GetInstance().AddFormTimer(formId, timerCfg.updateDuration, record.userId);
        } else {
            HILOG_INFO("%{public}s, add at timer:%{public}d, %{public}d", __func__,
                timerCfg.updateAtHour, timerCfg.updateAtMin);
            FormTimerMgr::GetInstance().AddFormTimer(formId, timerCfg.updateAtHour,
                                                     timerCfg.updateAtMin, record.userId);
        }

        return;
    }

    // both enable
    UpdateType type;
    if (record.updateDuration > 0) {
        if (timerCfg.updateDuration > 0) {
            // no change
            if (record.updateDuration == timerCfg.updateDuration) {
                return;
            }
            // interval change
            type = TYPE_INTERVAL_CHANGE;
        } else {
            // interval to updateat
            type = TYPE_INTERVAL_TO_ATTIME;
        }
    } else {
        if (timerCfg.updateDuration > 0) {
            // updateat to interval
            type = TYPE_ATTIME_TO_INTERVAL;
        } else {
            // no change;
            if (record.updateAtHour == timerCfg.updateAtHour && record.updateAtMin == timerCfg.updateAtMin) {
                return;
            }
            // updateat change
            type = TYPE_ATTIME_CHANGE;
        }
    }

    FormDataMgr::GetInstance().SetUpdateInfo(formId, true,
        timerCfg.updateDuration, timerCfg.updateAtHour, timerCfg.updateAtMin);
    FormTimerMgr::GetInstance().UpdateFormTimer(formId, type, timerCfg);
}

// multiuser
void FormSysEventReceiver::HandleUserIdRemoved(const int32_t userId)
{
    std::vector<int64_t> removedFormIds;
    FormDataMgr::GetInstance().DeleteFormsByUserId(userId, removedFormIds);
    FormDbCache::GetInstance().DeleteDBFormsByUserId(userId);

    // delete form timer
    std::vector<int64_t>::iterator itRemoved;
    for (itRemoved = removedFormIds.begin();itRemoved != removedFormIds.end(); ++itRemoved) {
        FormTimerMgr::GetInstance().RemoveFormTimer(*itRemoved);
    }
}
}  // namespace AppExecFwk
}  // namespace OHOS
