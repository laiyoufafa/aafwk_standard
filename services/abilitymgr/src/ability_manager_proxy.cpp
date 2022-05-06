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

#include "ability_manager_proxy.h"

#include "errors.h"
#include "string_ex.h"

#include "ability_connect_callback_proxy.h"
#include "ability_connect_callback_stub.h"
#include "ability_manager_errors.h"
#include "ability_scheduler_proxy.h"
#include "ability_scheduler_stub.h"

namespace OHOS {
namespace AAFwk {
bool AbilityManagerProxy::WriteInterfaceToken(MessageParcel &data)
{
    if (!data.WriteInterfaceToken(AbilityManagerProxy::GetDescriptor())) {
        HILOG_ERROR("write interface token failed.");
        return false;
    }
    return true;
}

int AbilityManagerProxy::StartAbility(const Want &want, int32_t userId, int requestCode)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteParcelable(&want)) {
        HILOG_ERROR("want write failed.");
        return INNER_ERR;
    }

    if (!data.WriteInt32(userId)) {
        HILOG_ERROR("userId write failed.");
        return INNER_ERR;
    }

    if (!data.WriteInt32(requestCode)) {
        HILOG_ERROR("requestCode write failed.");
        return INNER_ERR;
    }

    error = Remote()->SendRequest(IAbilityManager::START_ABILITY, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

AppExecFwk::ElementName AbilityManagerProxy::GetTopAbility()
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return {};
    }

    int error = Remote()->SendRequest(IAbilityManager::GET_TOP_ABILITY, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return {};
    }
    std::unique_ptr<AppExecFwk::ElementName> name(reply.ReadParcelable<AppExecFwk::ElementName>());
    if (!name) {
        HILOG_ERROR("Read info failed.");
        return {};
    }
    AppExecFwk::ElementName result = *name;
    return result;
}

int AbilityManagerProxy::StartAbility(const Want &want, const AbilityStartSetting &abilityStartSetting,
    const sptr<IRemoteObject> &callerToken, int32_t userId, int requestCode)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteParcelable(&want)) {
        HILOG_ERROR("want write failed.");
        return INNER_ERR;
    }
    if (!data.WriteParcelable(&abilityStartSetting)) {
        HILOG_ERROR("abilityStartSetting write failed.");
        return INNER_ERR;
    }
    if (!data.WriteParcelable(callerToken)) {
        HILOG_ERROR("callerToken write failed.");
        return INNER_ERR;
    }
    if (!data.WriteInt32(userId)) {
        HILOG_ERROR("userId write failed.");
        return INNER_ERR;
    }
    if (!data.WriteInt32(requestCode)) {
        HILOG_ERROR("requestCode write failed.");
        return INNER_ERR;
    }
    error = Remote()->SendRequest(IAbilityManager::START_ABILITY_FOR_SETTINGS, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::StartAbility(
    const Want &want, const sptr<IRemoteObject> &callerToken, int32_t userId, int requestCode)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteParcelable(&want)) {
        HILOG_ERROR("want write failed.");
        return INNER_ERR;
    }
    if (!data.WriteParcelable(callerToken)) {
        HILOG_ERROR("callerToken write failed.");
        return INNER_ERR;
    }
    if (!data.WriteInt32(userId)) {
        HILOG_ERROR("userId write failed.");
        return INNER_ERR;
    }
    if (!data.WriteInt32(requestCode)) {
        HILOG_ERROR("requestCode write failed.");
        return INNER_ERR;
    }

    error = Remote()->SendRequest(IAbilityManager::START_ABILITY_ADD_CALLER, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::StartAbility(const Want &want, const StartOptions &startOptions,
    const sptr<IRemoteObject> &callerToken, int32_t userId, int requestCode)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteParcelable(&want)) {
        HILOG_ERROR("want write failed.");
        return INNER_ERR;
    }
    if (!data.WriteParcelable(&startOptions)) {
        HILOG_ERROR("startOptions write failed.");
        return INNER_ERR;
    }
    if (!data.WriteParcelable(callerToken)) {
        HILOG_ERROR("callerToken write failed.");
        return INNER_ERR;
    }
    if (!data.WriteInt32(userId)) {
        HILOG_ERROR("userId write failed.");
        return INNER_ERR;
    }
    if (!data.WriteInt32(requestCode)) {
        HILOG_ERROR("requestCode write failed.");
        return INNER_ERR;
    }
    error = Remote()->SendRequest(IAbilityManager::START_ABILITY_FOR_OPTIONS, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::TerminateAbility(const sptr<IRemoteObject> &token, int resultCode, const Want *resultWant)
{
    return TerminateAbility(token, resultCode, resultWant, true);
}

int AbilityManagerProxy::TerminateAbility(const sptr<IRemoteObject> &token,
    int resultCode, const Want *resultWant, bool flag)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteParcelable(token) || !data.WriteInt32(resultCode) || !data.WriteParcelable(resultWant)) {
        HILOG_ERROR("data write failed.");
        return INNER_ERR;
    }
    if (!data.WriteBool(flag)) {
        HILOG_ERROR("data write flag failed.");
        return INNER_ERR;
    }
    error = Remote()->SendRequest(IAbilityManager::TERMINATE_ABILITY, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::TerminateAbilityByCaller(const sptr<IRemoteObject> &callerToken, int requestCode)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteParcelable(callerToken) || !data.WriteInt32(requestCode)) {
        HILOG_ERROR("data write failed.");
        return INNER_ERR;
    }
    error = Remote()->SendRequest(IAbilityManager::TERMINATE_ABILITY_BY_CALLER, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::CloseAbility(const sptr<IRemoteObject> &token, int resultCode, const Want *resultWant)
{
    return TerminateAbility(token, resultCode, resultWant, false);
}

int AbilityManagerProxy::ConnectAbility(
    const Want &want, const sptr<IAbilityConnection> &connect, const sptr<IRemoteObject> &callerToken, int32_t userId)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteParcelable(&want)) {
        HILOG_ERROR("want write failed.");
        return ERR_INVALID_VALUE;
    }
    if (connect == nullptr) {
        HILOG_ERROR("connect ability fail, connect is nullptr");
        return ERR_INVALID_VALUE;
    }
    if (!data.WriteParcelable(connect->AsObject())) {
        HILOG_ERROR("connect write failed.");
        return ERR_INVALID_VALUE;
    }
    if (!data.WriteParcelable(callerToken)) {
        HILOG_ERROR("callerToken write failed.");
        return ERR_INVALID_VALUE;
    }
    if (!data.WriteInt32(userId)) {
        HILOG_ERROR("userId write failed.");
        return INNER_ERR;
    }
    error = Remote()->SendRequest(IAbilityManager::CONNECT_ABILITY, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::DisconnectAbility(const sptr<IAbilityConnection> &connect)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (connect == nullptr) {
        HILOG_ERROR("disconnect ability fail, connect is nullptr");
        return ERR_INVALID_VALUE;
    }
    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteParcelable(connect->AsObject())) {
        HILOG_ERROR("connect write failed.");
        return ERR_INVALID_VALUE;
    }

    error = Remote()->SendRequest(IAbilityManager::DISCONNECT_ABILITY, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

sptr<IAbilityScheduler> AbilityManagerProxy::AcquireDataAbility(
    const Uri &uri, bool tryBind, const sptr<IRemoteObject> &callerToken)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!callerToken) {
        HILOG_ERROR("invalid parameters for acquire data ability.");
        return nullptr;
    }
    if (!WriteInterfaceToken(data)) {
        return nullptr;
    }
    if (!data.WriteString(uri.ToString()) || !data.WriteBool(tryBind) || !data.WriteParcelable(callerToken)) {
        HILOG_ERROR("data write failed.");
        return nullptr;
    }
    error = Remote()->SendRequest(IAbilityManager::ACQUIRE_DATA_ABILITY, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return nullptr;
    }

    return iface_cast<IAbilityScheduler>(reply.ReadParcelable<IRemoteObject>());
}

int AbilityManagerProxy::ReleaseDataAbility(
    sptr<IAbilityScheduler> dataAbilityScheduler, const sptr<IRemoteObject> &callerToken)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!dataAbilityScheduler || !callerToken) {
        return ERR_INVALID_VALUE;
    }
    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteParcelable(dataAbilityScheduler->AsObject()) || !data.WriteParcelable(callerToken)) {
        HILOG_ERROR("data write failed.");
        return INNER_ERR;
    }
    error = Remote()->SendRequest(IAbilityManager::RELEASE_DATA_ABILITY, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::AttachAbilityThread(const sptr<IAbilityScheduler> &scheduler, const sptr<IRemoteObject> &token)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (scheduler == nullptr) {
        return ERR_INVALID_VALUE;
    }
    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteParcelable(scheduler->AsObject()) || !data.WriteParcelable(token)) {
        HILOG_ERROR("data write failed.");
        return INNER_ERR;
    }
    error = Remote()->SendRequest(IAbilityManager::ATTACH_ABILITY_THREAD, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::AbilityTransitionDone(const sptr<IRemoteObject> &token, int state, const PacMap &saveData)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteParcelable(token) || !data.WriteInt32(state)) {
        HILOG_ERROR("token or state write failed.");
        return INNER_ERR;
    }
    if (!data.WriteParcelable(&saveData)) {
        HILOG_ERROR("saveData write failed.");
        return INNER_ERR;
    }
    error = Remote()->SendRequest(IAbilityManager::ABILITY_TRANSITION_DONE, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::ScheduleConnectAbilityDone(
    const sptr<IRemoteObject> &token, const sptr<IRemoteObject> &remoteObject)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteParcelable(token)) {
        HILOG_ERROR("token write failed.");
        return ERR_INVALID_VALUE;
    }
    if (!data.WriteParcelable(remoteObject)) {
        HILOG_ERROR("remoteObject write failed.");
        return ERR_INVALID_VALUE;
    }

    error = Remote()->SendRequest(IAbilityManager::CONNECT_ABILITY_DONE, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::ScheduleDisconnectAbilityDone(const sptr<IRemoteObject> &token)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteParcelable(token)) {
        HILOG_ERROR("token write failed.");
        return ERR_INVALID_VALUE;
    }

    error = Remote()->SendRequest(IAbilityManager::DISCONNECT_ABILITY_DONE, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::ScheduleCommandAbilityDone(const sptr<IRemoteObject> &token)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteParcelable(token)) {
        HILOG_ERROR("token write failed.");
        return ERR_INVALID_VALUE;
    }

    error = Remote()->SendRequest(IAbilityManager::COMMAND_ABILITY_DONE, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

void AbilityManagerProxy::AddWindowInfo(const sptr<IRemoteObject> &token, int32_t windowToken)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return;
    }
    if (!data.WriteParcelable(token) || !data.WriteInt32(windowToken)) {
        HILOG_ERROR("data write failed.");
        return;
    }
    error = Remote()->SendRequest(ADD_WINDOW_INFO, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
    }
}

void AbilityManagerProxy::DumpSysState(
    const std::string& args, std::vector<std::string>& state, bool isClient, bool isUserId, int UserId)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return;
    }
    data.WriteString16(Str8ToStr16(args));

    if (!data.WriteBool(isClient)) {
        HILOG_ERROR("data write failed.");
        return ;
    }
    if (!data.WriteBool(isUserId)) {
        HILOG_ERROR("data write failed.");
        return ;
    }
    if (!data.WriteInt32(UserId)) {
        HILOG_ERROR("data write failed.");
        return ;
    }
    error = Remote()->SendRequest(IAbilityManager::DUMPSYS_STATE, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("AbilityManagerProxy: SendRequest err %{public}d", error);
        return;
    }
    int32_t stackNum = reply.ReadInt32();
    for (int i = 0; i < stackNum; i++) {
        std::string stac = Str16ToStr8(reply.ReadString16());
        state.emplace_back(stac);
    }
}

void AbilityManagerProxy::DumpState(const std::string &args, std::vector<std::string> &state)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return;
    }
    data.WriteString16(Str8ToStr16(args));
    error = Remote()->SendRequest(IAbilityManager::DUMP_STATE, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("AbilityManagerProxy: SendRequest err %{public}d", error);
        return;
    }
    int32_t stackNum = reply.ReadInt32();
    for (int i = 0; i < stackNum; i++) {
        std::string stac = Str16ToStr8(reply.ReadString16());
        state.emplace_back(stac);
    }
}

int AbilityManagerProxy::TerminateAbilityResult(const sptr<IRemoteObject> &token, int startId)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteParcelable(token) || !data.WriteInt32(startId)) {
        HILOG_ERROR("data write failed.");
        return ERR_INVALID_VALUE;
    }
    error = Remote()->SendRequest(IAbilityManager::TERMINATE_ABILITY_RESULT, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::MinimizeAbility(const sptr<IRemoteObject> &token, bool fromUser)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteParcelable(token)) {
        HILOG_ERROR("data write failed.");
        return ERR_INVALID_VALUE;
    }
    if (!data.WriteBool(fromUser)) {
        HILOG_ERROR("data write failed.");
        return ERR_INVALID_VALUE;
    }
    error = Remote()->SendRequest(IAbilityManager::MINIMIZE_ABILITY, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::StopServiceAbility(const Want &want, int32_t userId)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteParcelable(&want)) {
        HILOG_ERROR("want write failed.");
        return INNER_ERR;
    }
    if (!data.WriteInt32(userId)) {
        HILOG_ERROR("userId write failed.");
        return INNER_ERR;
    }
    error = Remote()->SendRequest(IAbilityManager::STOP_SERVICE_ABILITY, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::GetAllStackInfo(StackInfo &stackInfo)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_SYNC);

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    sptr<IRemoteObject> remote = Remote();
    if (remote == nullptr) {
        HILOG_ERROR("remote is nullptr.");
        return ERR_UNKNOWN_OBJECT;
    }
    error = remote->SendRequest(IAbilityManager::LIST_STACK_INFO, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    int32_t result = reply.ReadInt32();
    if (result != ERR_OK) {
        HILOG_ERROR("Read info failed, err %{public}d", result);
        return result;
    }
    std::unique_ptr<StackInfo> info(reply.ReadParcelable<StackInfo>());
    if (!info) {
        HILOG_ERROR("Read info failed.");
        return ERR_UNKNOWN_OBJECT;
    }
    stackInfo = *info;
    return result;
}

template <typename T>
int AbilityManagerProxy::GetParcelableInfos(MessageParcel &reply, std::vector<T> &parcelableInfos)
{
    int32_t infoSize = reply.ReadInt32();
    for (int32_t i = 0; i < infoSize; i++) {
        std::unique_ptr<T> info(reply.ReadParcelable<T>());
        if (!info) {
            HILOG_ERROR("Read Parcelable infos failed.");
            return ERR_INVALID_VALUE;
        }
        parcelableInfos.emplace_back(*info);
    }
    return NO_ERROR;
}

int AbilityManagerProxy::GetRecentMissions(
    const int32_t numMax, const int32_t flags, std::vector<AbilityMissionInfo> &recentList)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteInt32(numMax)) {
        HILOG_ERROR("numMax write failed.");
        return ERR_INVALID_VALUE;
    }
    if (!data.WriteInt32(flags)) {
        HILOG_ERROR("flags write failed.");
        return ERR_INVALID_VALUE;
    }
    error = Remote()->SendRequest(IAbilityManager::GET_RECENT_MISSION, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    error = GetParcelableInfos<AbilityMissionInfo>(reply, recentList);
    if (error != NO_ERROR) {
        HILOG_ERROR("Get info error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::GetMissionSnapshot(const int32_t missionId, MissionPixelMap &missionPixelMap)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteInt32(missionId)) {
        HILOG_ERROR("missionId write failed.");
        return ERR_INVALID_VALUE;
    }
    error = Remote()->SendRequest(IAbilityManager::GET_MISSION_SNAPSHOT, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    std::unique_ptr<MissionPixelMap> info(reply.ReadParcelable<MissionPixelMap>());
    if (!info) {
        HILOG_ERROR("readParcelableInfo failed.");
        return ERR_UNKNOWN_OBJECT;
    }
    missionPixelMap = *info;
    return reply.ReadInt32();
}

int AbilityManagerProxy::GetMissionSnapshot(const std::string& deviceId, int32_t missionId, MissionSnapshot& snapshot)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteString(deviceId)) {
        HILOG_ERROR("deviceId write failed.");
        return INNER_ERR;
    }
    if (!data.WriteInt32(missionId)) {
        HILOG_ERROR("missionId write failed.");
        return ERR_INVALID_VALUE;
    }
    error = Remote()->SendRequest(IAbilityManager::GET_MISSION_SNAPSHOT_INFO, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    std::unique_ptr<MissionSnapshot> info(reply.ReadParcelable<MissionSnapshot>());
    if (!info) {
        HILOG_ERROR("readParcelableInfo failed.");
        return ERR_UNKNOWN_OBJECT;
    }
    snapshot = *info;
    return reply.ReadInt32();
}

int AbilityManagerProxy::MoveMissionToTop(int32_t missionId)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteInt32(missionId)) {
        HILOG_ERROR("missionId write failed.");
        return ERR_INVALID_VALUE;
    }
    error = Remote()->SendRequest(IAbilityManager::MOVE_MISSION_TO_TOP, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::MoveMissionToEnd(const sptr<IRemoteObject> &token, const bool nonFirst)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteParcelable(token) || !data.WriteBool(nonFirst)) {
        HILOG_ERROR("data write failed.");
        return ERR_INVALID_VALUE;
    }
    error = Remote()->SendRequest(IAbilityManager::MOVE_MISSION_TO_END, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::RemoveMission(int id)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteInt32(id)) {
        HILOG_ERROR("id write failed.");
        return ERR_INVALID_VALUE;
    }
    error = Remote()->SendRequest(IAbilityManager::REMOVE_MISSION, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::KillProcess(const std::string &bundleName)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteString16(Str8ToStr16(bundleName))) {
        HILOG_ERROR("bundleName write failed.");
        return ERR_INVALID_VALUE;
    }
    int error = Remote()->SendRequest(IAbilityManager::KILL_PROCESS, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::ForceTimeoutForTest(const std::string &abilityName, const std::string &state)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteString16(Str8ToStr16(abilityName))) {
        HILOG_ERROR("abilityName write failed.");
        return ERR_INVALID_VALUE;
    }
    if (!data.WriteString16(Str8ToStr16(state))) {
        HILOG_ERROR("abilityName write failed.");
        return ERR_INVALID_VALUE;
    }
    int error = Remote()->SendRequest(IAbilityManager::FORCE_TIMEOUT, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::ClearUpApplicationData(const std::string &bundleName)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteString16(Str8ToStr16(bundleName))) {
        HILOG_ERROR("bundleName write failed.");
        return ERR_INVALID_VALUE;
    }
    int error = Remote()->SendRequest(IAbilityManager::CLEAR_UP_APPLICATION_DATA, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::UninstallApp(const std::string &bundleName, int32_t uid)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteString16(Str8ToStr16(bundleName))) {
        HILOG_ERROR("bundleName write failed.");
        return ERR_INVALID_VALUE;
    }
    if (!data.WriteInt32(uid)) {
        HILOG_ERROR("uid write failed.");
        return ERR_INVALID_VALUE;
    }
    int error = Remote()->SendRequest(IAbilityManager::UNINSTALL_APP, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::MoveMissionToFloatingStack(const MissionOption &missionOption)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteParcelable(&missionOption)) {
        HILOG_ERROR("fail to write to parcel.");
        return INNER_ERR;
    }
    int error = Remote()->SendRequest(IAbilityManager::MOVE_MISSION_TO_FLOATING_STACK, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("%s, error: %d", __func__, error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::MoveMissionToSplitScreenStack(const MissionOption &primary, const MissionOption &secondary)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteParcelable(&primary) || !data.WriteParcelable(&secondary)) {
        HILOG_ERROR("fail to write to parcel.");
        return INNER_ERR;
    }
    int error = Remote()->SendRequest(IAbilityManager::MOVE_MISSION_TO_SPLITSCREEN_STACK, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("%s, error: %d", __func__, error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::ChangeFocusAbility(
    const sptr<IRemoteObject> &lostFocusToken, const sptr<IRemoteObject> &getFocusToken)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteParcelable(lostFocusToken) || !data.WriteParcelable(getFocusToken)) {
        HILOG_ERROR("change focus ability failed");
        return ERR_INVALID_VALUE;
    }
    auto error = Remote()->SendRequest(IAbilityManager::CHANGE_FOCUS_ABILITY, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("change focus ability error: %d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::MinimizeMultiWindow(int missionId)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteInt32(missionId)) {
        HILOG_ERROR("WriteInt32 fail.");
        return ERR_INVALID_VALUE;
    }
    auto error = Remote()->SendRequest(IAbilityManager::MINIMIZE_MULTI_WINDOW, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("minimize multi window error: %d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::MaximizeMultiWindow(int missionId)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteInt32(missionId)) {
        HILOG_ERROR("WriteInt32 fail.");
        return ERR_INVALID_VALUE;
    }
    auto error = Remote()->SendRequest(IAbilityManager::MAXIMIZE_MULTI_WINDOW, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("maximize multi window error: %d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::GetFloatingMissions(std::vector<AbilityMissionInfo> &list)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    auto error = Remote()->SendRequest(IAbilityManager::GET_FLOATING_MISSIONS, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("get floating mission fail, error: %{public}d", error);
        return error;
    }
    error = GetParcelableInfos<AbilityMissionInfo>(reply, list);
    if (error != NO_ERROR) {
        HILOG_ERROR("GetParcelableInfos fail, error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::CloseMultiWindow(int missionId)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteInt32(missionId)) {
        HILOG_ERROR("WriteInt32 fail.");
        return ERR_INVALID_VALUE;
    }
    auto error = Remote()->SendRequest(IAbilityManager::CLOSE_MULTI_WINDOW, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("close multi window error: %d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::SetMissionStackSetting(const StackSetting &stackSetting)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteParcelable(&stackSetting)) {
        HILOG_ERROR("%{public}s WriteParcelable", __func__);
        return INNER_ERR;
    }
    auto error = Remote()->SendRequest(IAbilityManager::SET_STACK_SETTING, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("%{public}s for result fail, error: %d", __func__, error);
        return error;
    }
    return reply.ReadInt32();
}

bool AbilityManagerProxy::IsFirstInMission(const sptr<IRemoteObject> &token)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return false;
    }
    if (!data.WriteParcelable(token)) {
        HILOG_ERROR("token write failed.");
        return false;
    }
    auto error = Remote()->SendRequest(IAbilityManager::IS_FIRST_IN_MISSION, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return false;
    }
    return reply.ReadBool();
}

int AbilityManagerProxy::PowerOff()
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    auto error = Remote()->SendRequest(IAbilityManager::POWER_OFF, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::PowerOn()
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    auto error = Remote()->SendRequest(IAbilityManager::POWER_ON, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::LockMission(int missionId)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteInt32(missionId)) {
        HILOG_ERROR("lock mission by id , WriteInt32 fail.");
        return ERR_INVALID_VALUE;
    }
    error = Remote()->SendRequest(IAbilityManager::LUCK_MISSION, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("lock mission by id , error: %d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::UnlockMission(int missionId)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteInt32(missionId)) {
        HILOG_ERROR("unlock mission by id , WriteInt32 fail.");
        return ERR_INVALID_VALUE;
    }
    error = Remote()->SendRequest(IAbilityManager::UNLUCK_MISSION, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("unlock mission by id , error: %d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::SetMissionDescriptionInfo(
    const sptr<IRemoteObject> &token, const MissionDescriptionInfo &description)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteParcelable(token)) {
        HILOG_ERROR("%{public}s for result fail", __func__);
        return false;
    }
    if (!data.WriteParcelable(&description)) {
        HILOG_ERROR("%{public}s for result fail", __func__);
        return false;
    }
    error = Remote()->SendRequest(IAbilityManager::SET_MISSION_INFO, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("unlock mission by id , error: %d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::GetMissionLockModeState()
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    error = Remote()->SendRequest(IAbilityManager::GET_MISSION_LOCK_MODE_STATE, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("get mission luck mode state , error: %d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::UpdateConfiguration(const AppExecFwk::Configuration &config)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteParcelable(&config)) {
        return INNER_ERR;
    }
    error = Remote()->SendRequest(IAbilityManager::UPDATE_CONFIGURATION, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("update configuration, error: %d", error);
        return error;
    }
    return reply.ReadInt32();
}

sptr<IWantSender> AbilityManagerProxy::GetWantSender(
    const WantSenderInfo &wantSenderInfo, const sptr<IRemoteObject> &callerToken)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return nullptr;
    }
    if (!data.WriteParcelable(&wantSenderInfo)) {
        HILOG_ERROR("wantSenderInfo write failed.");
        return nullptr;
    }
    if (!data.WriteParcelable(callerToken)) {
        HILOG_ERROR("callerToken write failed.");
        return nullptr;
    }
    auto error = Remote()->SendRequest(IAbilityManager::GET_PENDING_WANT_SENDER, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return nullptr;
    }
    sptr<IWantSender> wantSender = iface_cast<IWantSender>(reply.ReadParcelable<IRemoteObject>());
    if (!wantSender) {
        return nullptr;
    }
    return wantSender;
}

int AbilityManagerProxy::SendWantSender(const sptr<IWantSender> &target, const SenderInfo &senderInfo)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (target == nullptr || !data.WriteParcelable(target->AsObject())) {
        HILOG_ERROR("target write failed.");
        return INNER_ERR;
    }
    if (!data.WriteParcelable(&senderInfo)) {
        HILOG_ERROR("senderInfo write failed.");
        return INNER_ERR;
    }

    auto error = Remote()->SendRequest(IAbilityManager::SEND_PENDING_WANT_SENDER, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

void AbilityManagerProxy::CancelWantSender(const sptr<IWantSender> &sender)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return;
    }
    if (sender == nullptr || !data.WriteParcelable(sender->AsObject())) {
        HILOG_ERROR("sender write failed.");
        return;
    }
    auto error = Remote()->SendRequest(IAbilityManager::CANCEL_PENDING_WANT_SENDER, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return;
    }
}

int AbilityManagerProxy::GetPendingWantUid(const sptr<IWantSender> &target)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (target == nullptr || !data.WriteParcelable(target->AsObject())) {
        HILOG_ERROR("target write failed.");
        return ERR_INVALID_VALUE;
    }
    auto error = Remote()->SendRequest(IAbilityManager::GET_PENDING_WANT_UID, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return INNER_ERR;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::GetPendingWantUserId(const sptr<IWantSender> &target)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (target == nullptr || !data.WriteParcelable(target->AsObject())) {
        HILOG_ERROR("target write failed.");
        return ERR_INVALID_VALUE;
    }
    auto error = Remote()->SendRequest(IAbilityManager::GET_PENDING_WANT_USERID, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return INNER_ERR;
    }
    return reply.ReadInt32();
}

std::string AbilityManagerProxy::GetPendingWantBundleName(const sptr<IWantSender> &target)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return "";
    }
    if (target == nullptr || !data.WriteParcelable(target->AsObject())) {
        HILOG_ERROR("target write failed.");
        return "";
    }
    auto error = Remote()->SendRequest(IAbilityManager::GET_PENDING_WANT_BUNDLENAME, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return "";
    }
    return Str16ToStr8(reply.ReadString16());
}

int AbilityManagerProxy::GetPendingWantCode(const sptr<IWantSender> &target)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (target == nullptr || !data.WriteParcelable(target->AsObject())) {
        HILOG_ERROR("target write failed.");
        return ERR_INVALID_VALUE;
    }
    auto error = Remote()->SendRequest(IAbilityManager::GET_PENDING_WANT_CODE, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return INNER_ERR;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::GetPendingWantType(const sptr<IWantSender> &target)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (target == nullptr || !data.WriteParcelable(target->AsObject())) {
        HILOG_ERROR("target write failed.");
        return ERR_INVALID_VALUE;
    }
    auto error = Remote()->SendRequest(IAbilityManager::GET_PENDING_WANT_TYPE, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return INNER_ERR;
    }
    return reply.ReadInt32();
}

void AbilityManagerProxy::RegisterCancelListener(const sptr<IWantSender> &sender, const sptr<IWantReceiver> &receiver)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return;
    }
    if (sender == nullptr || !data.WriteParcelable(sender->AsObject())) {
        HILOG_ERROR("sender write failed.");
        return;
    }
    if (receiver == nullptr || !data.WriteParcelable(receiver->AsObject())) {
        HILOG_ERROR("receiver write failed.");
        return;
    }
    auto error = Remote()->SendRequest(IAbilityManager::REGISTER_CANCEL_LISTENER, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return;
    }
}

void AbilityManagerProxy::UnregisterCancelListener(const sptr<IWantSender> &sender, const sptr<IWantReceiver> &receiver)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return;
    }
    if (sender == nullptr || !data.WriteParcelable(sender->AsObject())) {
        HILOG_ERROR("sender write failed.");
        return;
    }
    if (receiver == nullptr || !data.WriteParcelable(receiver->AsObject())) {
        HILOG_ERROR("receiver write failed.");
        return;
    }
    auto error = Remote()->SendRequest(IAbilityManager::UNREGISTER_CANCEL_LISTENER, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return;
    }
}

int AbilityManagerProxy::GetPendingRequestWant(const sptr<IWantSender> &target, std::shared_ptr<Want> &want)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (target == nullptr || !data.WriteParcelable(target->AsObject())) {
        HILOG_ERROR("target write failed.");
        return INNER_ERR;
    }
    if (want == nullptr || !data.WriteParcelable(want.get())) {
        HILOG_ERROR("want write failed.");
        return INNER_ERR;
    }
    auto error = Remote()->SendRequest(IAbilityManager::GET_PENDING_REQUEST_WANT, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    std::unique_ptr<Want> wantInfo(reply.ReadParcelable<Want>());
    if (!wantInfo) {
        HILOG_ERROR("readParcelableInfo failed");
        return INNER_ERR;
    }
    want = std::move(wantInfo);

    return NO_ERROR;
}

int AbilityManagerProxy::GetWantSenderInfo(const sptr<IWantSender> &target, std::shared_ptr<WantSenderInfo> &info)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (target == nullptr || !data.WriteParcelable(target->AsObject())) {
        HILOG_ERROR("target write failed.");
        return INNER_ERR;
    }
    if (info == nullptr || !data.WriteParcelable(info.get())) {
        HILOG_ERROR("info write failed.");
        return INNER_ERR;
    }
    auto error = Remote()->SendRequest(IAbilityManager::GET_PENDING_WANT_SENDER_INFO, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    std::unique_ptr<WantSenderInfo> wantSenderInfo(reply.ReadParcelable<WantSenderInfo>());
    if (!wantSenderInfo) {
        HILOG_ERROR("readParcelable Info failed");
        return INNER_ERR;
    }
    info = std::move(wantSenderInfo);

    return NO_ERROR;
}

int AbilityManagerProxy::SetShowOnLockScreen(bool isAllow)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteBool(isAllow)) {
        HILOG_ERROR("data write failed.");
        return ERR_INVALID_VALUE;
    }
    error = Remote()->SendRequest(IAbilityManager::SET_SHOW_ON_LOCK_SCREEN, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

/**
 * Get system memory information.
 * @param SystemMemoryAttr, memory information.
 */
void AbilityManagerProxy::GetSystemMemoryAttr(AppExecFwk::SystemMemoryAttr &memoryInfo)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        HILOG_ERROR("WriteInterfaceToken failed");
        return;
    }

    auto error = Remote()->SendRequest(IAbilityManager::GET_SYSTEM_MEMORY_ATTR, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return;
    }

    std::shared_ptr<AppExecFwk::SystemMemoryAttr> remoteRetsult(reply.ReadParcelable<AppExecFwk::SystemMemoryAttr>());
    if (remoteRetsult == nullptr) {
        HILOG_ERROR("recv SystemMemoryAttr failed");
        return;
    }

    memoryInfo = *remoteRetsult;
}

int AbilityManagerProxy::GetAppMemorySize()
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        HILOG_ERROR("WriteInterfaceToken faild");
        return INNER_ERR;
    }
    auto error = Remote()->SendRequest(IAbilityManager::GET_APP_MEMORY_SIZE, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

bool AbilityManagerProxy::IsRamConstrainedDevice()
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        HILOG_ERROR("WriteInterfaceToken faild");
        return false;
    }
    auto error = Remote()->SendRequest(IAbilityManager::IS_RAM_CONSTRAINED_DEVICE, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return false;
    }
    return reply.ReadBool();
}

int AbilityManagerProxy::ContinueMission(const std::string &srcDeviceId, const std::string &dstDeviceId,
    int32_t missionId, const sptr<IRemoteObject> &callBack, AAFwk::WantParams &wantParams)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteString(srcDeviceId)) {
        HILOG_ERROR("srcDeviceId write failed.");
        return INNER_ERR;
    }
    if (!data.WriteString(dstDeviceId)) {
        HILOG_ERROR("dstDeviceId write failed.");
        return INNER_ERR;
    }
    if (!data.WriteInt32(missionId)) {
        HILOG_ERROR("missionId write failed.");
        return INNER_ERR;
    }
    if (!data.WriteRemoteObject(callBack)) {
        HILOG_ERROR("callBack write failed.");
        return INNER_ERR;
    }
    if (!data.WriteParcelable(&wantParams)) {
        HILOG_ERROR("wantParams write failed.");
        return INNER_ERR;
    }

    auto error = Remote()->SendRequest(IAbilityManager::CONTINUE_MISSION, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::ContinueAbility(const std::string &deviceId, int32_t missionId)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteString(deviceId)) {
        HILOG_ERROR("deviceId write failed.");
        return INNER_ERR;
    }
    if (!data.WriteInt32(missionId)) {
        HILOG_ERROR("missionId write failed.");
        return INNER_ERR;
    }

    auto error = Remote()->SendRequest(IAbilityManager::CONTINUE_ABILITY, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::StartContinuation(const Want &want, const sptr<IRemoteObject> &abilityToken, int32_t status)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteParcelable(&want)) {
        HILOG_ERROR("want write failed.");
        return INNER_ERR;
    }
    if (!data.WriteParcelable(abilityToken)) {
        HILOG_ERROR("abilityToken write failed.");
        return INNER_ERR;
    }
    if (!data.WriteInt32(status)) {
        HILOG_ERROR("status write failed.");
        return INNER_ERR;
    }
    auto error = Remote()->SendRequest(IAbilityManager::START_CONTINUATION, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

void AbilityManagerProxy::NotifyCompleteContinuation(const std::string &deviceId, int32_t sessionId, bool isSuccess)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return;
    }
    if (!data.WriteString(deviceId)) {
        HILOG_ERROR("deviceId write failed.");
        return;
    }
    if (!data.WriteInt32(sessionId)) {
        HILOG_ERROR("sessionId write failed.");
        return;
    }
    if (!data.WriteBool(isSuccess)) {
        HILOG_ERROR("result write failed.");
        return;
    }

    auto error = Remote()->SendRequest(IAbilityManager::NOTIFY_COMPLETE_CONTINUATION, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return;
    }
}

int AbilityManagerProxy::NotifyContinuationResult(int32_t missionId, int32_t result)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteInt32(missionId)) {
        HILOG_ERROR("missionId write failed.");
        return INNER_ERR;
    }
    if (!data.WriteInt32(result)) {
        HILOG_ERROR("result write failed.");
        return INNER_ERR;
    }

    auto error = Remote()->SendRequest(IAbilityManager::NOTIFY_CONTINUATION_RESULT, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::LockMissionForCleanup(int32_t missionId)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteInt32(missionId)) {
        HILOG_ERROR("lock mission by id , WriteInt32 fail.");
        return ERR_INVALID_VALUE;
    }
    error = Remote()->SendRequest(IAbilityManager::LOCK_MISSION_FOR_CLEANUP, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("lock mission by id , error: %d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::UnlockMissionForCleanup(int32_t missionId)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteInt32(missionId)) {
        HILOG_ERROR("unlock mission by id , WriteInt32 fail.");
        return ERR_INVALID_VALUE;
    }
    error = Remote()->SendRequest(IAbilityManager::UNLOCK_MISSION_FOR_CLEANUP, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("unlock mission by id , error: %d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::RegisterMissionListener(const sptr<IMissionListener> &listener)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!listener) {
        HILOG_ERROR("register mission listener, listener is nullptr");
        return ERR_INVALID_VALUE;
    }

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteParcelable(listener->AsObject())) {
        HILOG_ERROR("write mission listener failed when register mission listener.");
        return ERR_INVALID_VALUE;
    }

    error = Remote()->SendRequest(IAbilityManager::REGISTER_MISSION_LISTENER, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::RegisterMissionListener(const std::string &deviceId,
    const sptr<IRemoteMissionListener> &listener)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteString(deviceId)) {
        HILOG_ERROR("deviceId write failed.");
        return INNER_ERR;
    }
    if (!data.WriteRemoteObject(listener->AsObject())) {
        HILOG_ERROR("listener write failed.");
        return INNER_ERR;
    }

    auto error = Remote()->SendRequest(IAbilityManager::REGISTER_REMOTE_MISSION_LISTENER, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::UnRegisterMissionListener(const sptr<IMissionListener> &listener)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!listener) {
        HILOG_ERROR("unregister mission listener, listener is nullptr");
        return ERR_INVALID_VALUE;
    }

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteParcelable(listener->AsObject())) {
        HILOG_ERROR("write mission listener failed when unregister mission listener.");
        return ERR_INVALID_VALUE;
    }

    error = Remote()->SendRequest(IAbilityManager::UNREGISTER_MISSION_LISTENER, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::GetMissionInfos(const std::string& deviceId, int32_t numMax,
    std::vector<MissionInfo> &missionInfos)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteString16(Str8ToStr16(deviceId))) {
        HILOG_ERROR("write deviceId failed when GetMissionInfos.");
        return ERR_INVALID_VALUE;
    }
    if (!data.WriteInt32(numMax)) {
        HILOG_ERROR("GetMissionInfos numMax write failed.");
        return ERR_INVALID_VALUE;
    }
    error = Remote()->SendRequest(IAbilityManager::GET_MISSION_INFOS, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("GetMissionInfos Send request error: %{public}d", error);
        return error;
    }
    error = GetParcelableInfos<MissionInfo>(reply, missionInfos);
    if (error != NO_ERROR) {
        HILOG_ERROR("GetMissionInfos error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::GetMissionInfo(const std::string& deviceId, int32_t missionId,
    MissionInfo &missionInfo)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteString16(Str8ToStr16(deviceId))) {
        HILOG_ERROR("write deviceId failed when GetMissionInfo.");
        return ERR_INVALID_VALUE;
    }
    if (!data.WriteInt32(missionId)) {
        HILOG_ERROR("GetMissionInfo write missionId failed.");
        return ERR_INVALID_VALUE;
    }
    error = Remote()->SendRequest(IAbilityManager::GET_MISSION_INFO_BY_ID, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("GetMissionInfo Send request error: %{public}d", error);
        return error;
    }

    std::unique_ptr<MissionInfo> info(reply.ReadParcelable<MissionInfo>());
    if (!info) {
        HILOG_ERROR("read missioninfo failed.");
        return ERR_UNKNOWN_OBJECT;
    }
    missionInfo = *info;
    return reply.ReadInt32();
}

int AbilityManagerProxy::CleanMission(int32_t missionId)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteInt32(missionId)) {
        HILOG_ERROR("clean mission by id , WriteInt32 fail.");
        return ERR_INVALID_VALUE;
    }
    error = Remote()->SendRequest(IAbilityManager::CLEAN_MISSION, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("clean mission by id , error: %d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::CleanAllMissions()
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    error = Remote()->SendRequest(IAbilityManager::CLEAN_ALL_MISSIONS, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("lock mission by id ,SendRequest error: %d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::MoveMissionToFront(int32_t missionId)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteInt32(missionId)) {
        HILOG_ERROR("move mission to front , WriteInt32 fail.");
        return ERR_INVALID_VALUE;
    }
    error = Remote()->SendRequest(IAbilityManager::MOVE_MISSION_TO_FRONT, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("move mission to front, SendRequest error: %d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::MoveMissionToFront(int32_t missionId, const StartOptions &startOptions)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteInt32(missionId)) {
        HILOG_ERROR("move mission to front , WriteInt32 fail.");
        return ERR_INVALID_VALUE;
    }
    if (!data.WriteParcelable(&startOptions)) {
        HILOG_ERROR("startOptions write failed.");
        return INNER_ERR;
    }
    error = Remote()->SendRequest(IAbilityManager::MOVE_MISSION_TO_FRONT_BY_OPTIONS, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("move mission to front, SendRequest error: %d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::StartUser(int userId)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteInt32(userId)) {
        HILOG_ERROR("StartUser:WriteInt32 fail.");
        return ERR_INVALID_VALUE;
    }
    error = Remote()->SendRequest(IAbilityManager::START_USER, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("StartUser:SendRequest error: %d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::StopUser(int userId, const sptr<IStopUserCallback> &callback)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteInt32(userId)) {
        HILOG_ERROR("StopUser:WriteInt32 fail.");
        return ERR_INVALID_VALUE;
    }

    if (!callback) {
        data.WriteBool(false);
    } else {
        data.WriteBool(true);
        if (!data.WriteParcelable(callback->AsObject())) {
            HILOG_ERROR("StopUser:write IStopUserCallback fail.");
            return ERR_INVALID_VALUE;
        }
    }
    error = Remote()->SendRequest(IAbilityManager::STOP_USER, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("StopUser:SendRequest error: %d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::SetMissionLabel(const sptr<IRemoteObject> &token, const std::string &label)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteRemoteObject(token)) {
        HILOG_ERROR("SetMissionLabel write token failed.");
        return ERR_INVALID_VALUE;
    }
    if (!data.WriteString16(Str8ToStr16(label))) {
        HILOG_ERROR("SetMissionLabel write label failed.");
        return ERR_INVALID_VALUE;
    }
    auto error = Remote()->SendRequest(IAbilityManager::SET_MISSION_LABEL, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("SetMissionLabel Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

#ifdef SUPPORT_GRAPHICS
int AbilityManagerProxy::SetMissionIcon(const sptr<IRemoteObject> &token,
    const std::shared_ptr<OHOS::Media::PixelMap> &icon)
{
    if (!token || !icon) {
        HILOG_ERROR("SetMissionIcon abilitytoken or icon is invalid.");
        return ERR_INVALID_VALUE;
    }

    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteRemoteObject(token)) {
        HILOG_ERROR("SetMissionIcon write token failed.");
        return ERR_INVALID_VALUE;
    }

    if (!data.WriteParcelable(icon.get())) {
        HILOG_ERROR("SetMissionIcon write icon failed.");
        return ERR_INVALID_VALUE;
    }

    auto error = Remote()->SendRequest(IAbilityManager::SET_MISSION_ICON, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("SetMissionIcon Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}
#endif

int AbilityManagerProxy::GetAbilityRunningInfos(std::vector<AbilityRunningInfo> &info)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }

    auto error = Remote()->SendRequest(IAbilityManager::GET_ABILITY_RUNNING_INFO, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Get ability running info, error: %{public}d", error);
        return error;
    }
    error = GetParcelableInfos<AbilityRunningInfo>(reply, info);
    if (error != NO_ERROR) {
        HILOG_ERROR("GetParcelableInfos fail, error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::GetExtensionRunningInfos(int upperLimit, std::vector<ExtensionRunningInfo> &info)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }

    if (!data.WriteInt32(upperLimit)) {
        HILOG_ERROR("upperLimit write failed.");
        return INNER_ERR;
    }

    auto error = Remote()->SendRequest(IAbilityManager::GET_EXTENSION_RUNNING_INFO, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Get extension running info failed., error: %{public}d", error);
        return error;
    }
    error = GetParcelableInfos<ExtensionRunningInfo>(reply, info);
    if (error != NO_ERROR) {
        HILOG_ERROR("GetParcelableInfos fail, error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::GetProcessRunningInfos(std::vector<AppExecFwk::RunningProcessInfo> &info)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }

    auto error = Remote()->SendRequest(IAbilityManager::GET_PROCESS_RUNNING_INFO, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Get process running info, error: %{public}d", error);
        return error;
    }
    error = GetParcelableInfos<AppExecFwk::RunningProcessInfo>(reply, info);
    if (error != NO_ERROR) {
        HILOG_ERROR("GetParcelableInfos fail, error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::StartSyncRemoteMissions(const std::string& devId, bool fixConflict, int64_t tag)
{
    HILOG_INFO("called");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        HILOG_ERROR("WriteInterfaceToken failed");
        return ERR_INVALID_VALUE;
    }
    if (!data.WriteString(devId)) {
        HILOG_ERROR("write deviceId fail.");
        return ERR_INVALID_VALUE;
    }

    if (!data.WriteBool(fixConflict)) {
        HILOG_ERROR("WriteBool fail.");
        return ERR_INVALID_VALUE;
    }

    if (!data.WriteInt64(tag)) {
        HILOG_ERROR("WriteInt64 fail.");
        return ERR_INVALID_VALUE;
    }

    auto error = Remote()->SendRequest(IAbilityManager::START_SYNC_MISSIONS, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int32_t AbilityManagerProxy::StopSyncRemoteMissions(const std::string& devId)
{
    HILOG_INFO("called");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        HILOG_ERROR("WriteInterfaceToken failed");
        return ERR_INVALID_VALUE;
    }
    if (!data.WriteString(devId)) {
        HILOG_ERROR("write deviceId fail.");
        return ERR_INVALID_VALUE;
    }
    auto error = Remote()->SendRequest(IAbilityManager::STOP_SYNC_MISSIONS, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::UnRegisterMissionListener(const std::string &deviceId,
    const sptr<IRemoteMissionListener> &listener)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteString(deviceId)) {
        HILOG_ERROR("deviceId write failed.");
        return INNER_ERR;
    }
    if (!data.WriteRemoteObject(listener->AsObject())) {
        HILOG_ERROR("listener write failed.");
        return INNER_ERR;
    }

    auto error = Remote()->SendRequest(IAbilityManager::UNREGISTER_REMOTE_MISSION_LISTENER, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::StartAbilityByCall(
    const Want &want, const sptr<IAbilityConnection> &connect, const sptr<IRemoteObject> &callerToken)
{
    HILOG_DEBUG("AbilityManagerProxy::StartAbilityByCall begin.");
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteParcelable(&want)) {
        HILOG_ERROR("want write failed.");
        return ERR_INVALID_VALUE;
    }
    if (connect == nullptr) {
        HILOG_ERROR("resolve ability fail, connect is nullptr");
        return ERR_INVALID_VALUE;
    }
    if (!data.WriteParcelable(connect->AsObject())) {
        HILOG_ERROR("resolve write failed.");
        return ERR_INVALID_VALUE;
    }
    if (!data.WriteParcelable(callerToken)) {
        HILOG_ERROR("callerToken write failed.");
        return INNER_ERR;
    }

    HILOG_DEBUG("AbilityManagerProxy::StartAbilityByCall SendRequest Call.");
    error = Remote()->SendRequest(IAbilityManager::START_CALL_ABILITY, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    HILOG_DEBUG("AbilityManagerProxy::StartAbilityByCall end.");
    return reply.ReadInt32();
}

int AbilityManagerProxy::ReleaseAbility(const sptr<IAbilityConnection> &connect, const AppExecFwk::ElementName &element)
{
    int error;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (connect == nullptr) {
        HILOG_ERROR("release calll ability fail, connect is nullptr");
        return ERR_INVALID_VALUE;
    }
    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteParcelable(connect->AsObject())) {
        HILOG_ERROR("release ability connect write failed.");
        return ERR_INVALID_VALUE;
    }
    if (!data.WriteParcelable(&element)) {
        HILOG_ERROR("element error.");
        return ERR_INVALID_VALUE;
    }

    error = Remote()->SendRequest(IAbilityManager::RELEASE_CALL_ABILITY, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::RegisterSnapshotHandler(const sptr<ISnapshotHandler>& handler)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteRemoteObject(handler->AsObject())) {
        HILOG_ERROR("snapshot: handler write failed.");
        return INNER_ERR;
    }
    auto error = Remote()->SendRequest(IAbilityManager::REGISTER_SNAPSHOT_HANDLER, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("snapshot: send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::SetAbilityController(const sptr<AppExecFwk::IAbilityController> &abilityController,
    bool imAStabilityTest)
{
    if (!abilityController) {
        HILOG_ERROR("abilityController nullptr");
        return ERR_INVALID_VALUE;
    }
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteRemoteObject(abilityController->AsObject())) {
        HILOG_ERROR("abilityController write failed.");
        return ERR_INVALID_VALUE;
    }
    if (!data.WriteBool(imAStabilityTest)) {
        HILOG_ERROR("imAStabilityTest write failed.");
        return ERR_INVALID_VALUE;
    }
    auto error = Remote()->SendRequest(IAbilityManager::SET_ABILITY_CONTROLLER, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

bool AbilityManagerProxy::IsRunningInStabilityTest()
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return false;
    }
    auto error = Remote()->SendRequest(IAbilityManager::IS_USER_A_STABILITY_TEST, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return false;
    }
    return reply.ReadBool();
}

int AbilityManagerProxy::StartUserTest(const Want &want, const sptr<IRemoteObject> &observer)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteParcelable(&want)) {
        HILOG_ERROR("want write failed.");
        return INNER_ERR;
    }
    if (!data.WriteParcelable(observer)) {
        HILOG_ERROR("observer write failed.");
        return INNER_ERR;
    }
    auto error = Remote()->SendRequest(IAbilityManager::START_USER_TEST, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::FinishUserTest(const std::string &msg, const int &resultCode, const std::string &bundleName)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteString(msg)) {
        HILOG_ERROR("msg write failed.");
        return ERR_INVALID_VALUE;
    }
    if (!data.WriteInt32(resultCode)) {
        HILOG_ERROR("resultCode:WriteInt32 fail.");
        return ERR_INVALID_VALUE;
    }
    if (!data.WriteString(bundleName)) {
        HILOG_ERROR("bundleName write failed.");
        return ERR_INVALID_VALUE;
    }

    auto error = Remote()->SendRequest(IAbilityManager::FINISH_USER_TEST, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }
    return reply.ReadInt32();
}

int AbilityManagerProxy::GetCurrentTopAbility(sptr<IRemoteObject> &token)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }

    auto error = Remote()->SendRequest(IAbilityManager::GET_CURRENT_TOP_ABILITY, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }

    token = sptr<IRemoteObject>(reply.ReadParcelable<IRemoteObject>());
    if (!token) {
        HILOG_ERROR("read IRemoteObject failed.");
        return ERR_UNKNOWN_OBJECT;
    }

    return reply.ReadInt32();
}

int AbilityManagerProxy::DelegatorDoAbilityForeground(const sptr<IRemoteObject> &token)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }

    if (!data.WriteParcelable(token)) {
        HILOG_ERROR("data write failed.");
        return ERR_INVALID_VALUE;
    }

    auto error = Remote()->SendRequest(IAbilityManager::DELEGATOR_DO_ABILITY_FOREGROUND, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }

    return reply.ReadInt32();
}

int AbilityManagerProxy::DelegatorDoAbilityBackground(const sptr<IRemoteObject> &token)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }

    if (!data.WriteParcelable(token)) {
        HILOG_ERROR("data write failed.");
        return ERR_INVALID_VALUE;
    }

    auto error = Remote()->SendRequest(IAbilityManager::DELEGATOR_DO_ABILITY_BACKGROUND, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }

    return reply.ReadInt32();
}

int AbilityManagerProxy::DoAbilityForeground(const sptr<IRemoteObject> &token, uint32_t flag)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }

    if (!data.WriteParcelable(token)) {
        HILOG_ERROR("data write failed.");
        return ERR_INVALID_VALUE;
    }

    if (!data.WriteUint32(flag)) {
        HILOG_ERROR("flag write failed.");
        return ERR_INVALID_VALUE;
    }

    auto error = Remote()->SendRequest(IAbilityManager::DO_ABILITY_FOREGROUND, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }

    return reply.ReadInt32();
}

int AbilityManagerProxy::DoAbilityBackground(const sptr<IRemoteObject> &token, uint32_t flag)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }

    if (!data.WriteParcelable(token)) {
        HILOG_ERROR("data write failed.");
        return ERR_INVALID_VALUE;
    }

    if (!data.WriteUint32(flag)) {
        HILOG_ERROR("flag write failed.");
        return ERR_INVALID_VALUE;
    }

    auto error = Remote()->SendRequest(IAbilityManager::DO_ABILITY_BACKGROUND, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return error;
    }

    return reply.ReadInt32();
}

int AbilityManagerProxy::SendANRProcessID(int pid)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        return INNER_ERR;
    }
    if (!data.WriteInt32(pid)) {
        HILOG_ERROR("pid WriteInt32 fail.");
        return ERR_INVALID_VALUE;
    }
    auto error = Remote()->SendRequest(IAbilityManager::SEND_APP_NOT_RESPONSE_PROCESS_ID, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("SendANRProcessID error: %d", error);
        return error;
    }
    return reply.ReadInt32();
}

int32_t AbilityManagerProxy::GetMissionIdByToken(const sptr<IRemoteObject> &token)
{
    if (!token) {
        HILOG_ERROR("token is nullptr.");
        return -1;
    }

    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!WriteInterfaceToken(data)) {
        HILOG_ERROR("data interface token failed.");
        return -1;
    }

    if (!data.WriteParcelable(token)) {
        HILOG_ERROR("data write failed.");
        return -1;
    }

    auto error = Remote()->SendRequest(IAbilityManager::GET_MISSION_ID_BY_ABILITY_TOKEN, data, reply, option);
    if (error != NO_ERROR) {
        HILOG_ERROR("Send request error: %{public}d", error);
        return -1;
    }

    return reply.ReadInt32();
}
}  // namespace AAFwk
}  // namespace OHOS
