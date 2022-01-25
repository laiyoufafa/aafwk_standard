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

#ifndef FOUNDATION_APPEXECFWK_INTERFACES_INNERKITS_APPEXECFWK_CORE_INCLUDE_APPMGR_AMS_MGR_INTERFACE_H
#define FOUNDATION_APPEXECFWK_INTERFACES_INNERKITS_APPEXECFWK_CORE_INCLUDE_APPMGR_AMS_MGR_INTERFACE_H

#include "iremote_broker.h"
#include "iremote_object.h"

#include "ability_info.h"
#include "application_info.h"
#include "app_record_id.h"
#include "iapp_state_callback.h"
#include "running_process_info.h"
#include "istart_specified_ability_response.h"
#include "configuration.h"

namespace OHOS {
namespace AppExecFwk {
class IAmsMgr : public IRemoteBroker {
public:
    DECLARE_INTERFACE_DESCRIPTOR(u"ohos.appexecfwk.IAmsMgr");

    /**
     * LoadAbility, call LoadAbility() through proxy project, load the ability that needed to be started.
     *
     * @param token, the unique identification to start the ability.
     * @param preToken, the unique identification to call the ability.
     * @param abilityInfo, the ability information.
     * @param appInfo, the app information.
     * @return
     */
    virtual void LoadAbility(const sptr<IRemoteObject> &token, const sptr<IRemoteObject> &preToken,
        const std::shared_ptr<AbilityInfo> &abilityInfo, const std::shared_ptr<ApplicationInfo> &appInfo) = 0;

    /**
     * TerminateAbility, call TerminateAbility() through the proxy object, terminate the token ability.
     *
     * @param token, token, he unique identification to terminate the ability.
     * @return
     */
    virtual void TerminateAbility(const sptr<IRemoteObject> &token) = 0;

    /**
     * UpdateAbilityState, call UpdateAbilityState() through the proxy object, update the ability status.
     *
     * @param token, the unique identification to update the ability.
     * @param state, ability status that needs to be updated.
     * @return
     */
    virtual void UpdateAbilityState(const sptr<IRemoteObject> &token, const AbilityState state) = 0;

    /**
     * UpdateExtensionState, call UpdateExtensionState() through the proxy object, update the extension status.
     *
     * @param token, the unique identification to update the extension.
     * @param state, extension status that needs to be updated.
     * @return
     */
    virtual void UpdateExtensionState(const sptr<IRemoteObject> &token, const ExtensionState state) = 0;

    /**
     * RegisterAppStateCallback, call RegisterAppStateCallback() through the proxy object, register the callback.
     *
     * @param callback, Ams register the callback.
     * @return
     */
    virtual void RegisterAppStateCallback(const sptr<IAppStateCallback> &callback) = 0;

    /**
     * Reset,call Reset() through the proxy object, reset DFX of AppMgr.
     *
     * @return
     */
    virtual void Reset() = 0;

    /**
     * AbilityBehaviorAnalysis,call AbilityBehaviorAnalysis() through the proxy object,
     * ability behavior analysis assistant process optimization.
     *
     * @param token, the unique identification to start the ability.
     * @param preToken, the unique identification to call the ability.
     * @param visibility, the visibility information about windows info.
     * @param perceptibility, the Perceptibility information about windows info.
     * @param connectionState, the service ability connection state.
     * @return
     */
    virtual void AbilityBehaviorAnalysis(const sptr<IRemoteObject> &token, const sptr<IRemoteObject> &preToken,
        const int32_t visibility, const int32_t perceptibility, const int32_t connectionState) = 0;

    /**
     * KillProcessByAbilityToken, call KillProcessByAbilityToken() through proxy object,
     * kill the process by ability token.
     *
     * @param token, the unique identification to the ability.
     * @return
     */
    virtual void KillProcessByAbilityToken(const sptr<IRemoteObject> &token) = 0;

    /**
     * KillProcessesByUserId, call KillProcessesByUserId() through proxy object,
     * kill the processes by userId.
     *
     * @param userId, the user id.
     * @return
     */
    virtual void KillProcessesByUserId(int32_t userId) = 0;

    /**
     * KillApplication, call KillApplication() through proxy object, kill the application.
     *
     * @param  bundleName, bundle name in Application record.
     * @return ERR_OK, return back success, others fail.
     */
    virtual int KillApplication(const std::string &bundleName) = 0;

    /**
     * KillApplicationByUid, call KillApplicationByUid() through proxy object, kill the application.
     *
     * @param  bundleName, bundle name in Application record.
     * @param  userId, userId.
     * @return ERR_OK, return back success, others fail.
     */
    virtual int KillApplicationByUid(const std::string &bundleName, const int uid) = 0;

    virtual void AbilityAttachTimeOut(const sptr<IRemoteObject> &token) = 0;

    virtual void PrepareTerminate(const sptr<IRemoteObject> &token) = 0;

    /**
     * Checks whether a specified permission has been granted to the process identified by pid and uid
     *
     * @param permission Indicates the permission to check.
     * @param pid Indicates the ID of the process to check.
     * @param uid Indicates the UID of the process to check.
     * @param message Describe success or failure
     *
     * @return Returns ERR_OK on success, others on failure.
     */
    virtual int CompelVerifyPermission(const std::string &permission, int pid, int uid, std::string &message) = 0;

    virtual void GetRunningProcessInfoByToken(
        const sptr<IRemoteObject> &token, OHOS::AppExecFwk::RunningProcessInfo &info) = 0;

    virtual void StartSpecifiedAbility(const AAFwk::Want &want, const AppExecFwk::AbilityInfo &abilityInfo) = 0;

    virtual void RegisterStartSpecifiedAbilityResponse(const sptr<IStartSpecifiedAbilityResponse> &response) = 0;
	
	 /**
     *  ANotify application update system environment changes.
     *
     * @param config System environment change parameters.
     * @return Returns ERR_OK on success, others on failure.
     */
    virtual void UpdateConfiguration(const Configuration &config) = 0;

    enum class Message {
        LOAD_ABILITY = 0,
        TERMINATE_ABILITY,
        UPDATE_ABILITY_STATE,
        UPDATE_EXTENSION_STATE,
        REGISTER_APP_STATE_CALLBACK,
        RESET,
        ABILITY_BEHAVIOR_ANALYSIS,
        KILL_PEOCESS_BY_ABILITY_TOKEN,
        KILL_PROCESSES_BY_USERID,
        KILL_APPLICATION,
        ABILITY_ATTACH_TIMEOUT,
        COMPEL_VERIFY_PERMISSION,
        PREPARE_TERMINATE_ABILITY,
        KILL_APPLICATION_BYUID,
        GET_RUNNING_PROCESS_INFO_BY_TOKEN,
        START_SPECIFIED_ABILITY,
        REGISTER_START_SPECIFIED_ABILITY_RESPONSE,
        UPDATE_CONFIGURATION,
    };
};
}  // namespace AppExecFwk
}  // namespace OHOS

#endif  // FOUNDATION_APPEXECFWK_INTERFACES_INNERKITS_APPEXECFWK_CORE_INCLUDE_APPMGR_AMS_MGR_INTERFACE_H