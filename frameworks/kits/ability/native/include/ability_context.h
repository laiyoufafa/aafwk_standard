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

#ifndef FOUNDATION_APPEXECFWK_OHOS_ABILITY_CONTEXT_H
#define FOUNDATION_APPEXECFWK_OHOS_ABILITY_CONTEXT_H

#include <map>

#include "context_container.h"
#include "data_ability_helper.h"

namespace OHOS {
namespace DataShare {
class DataShareHelper;
}
namespace AppExecFwk {
class AbilityContext : public ContextContainer {
public:
    AbilityContext() = default;
    virtual ~AbilityContext() = default;

    /**
     * Attaches a Context object to the current ability.
     * Generally, this method is called after Ability is loaded to provide the application context for the current
     * ability.
     *
     * @param base Indicates a Context object.
     */
    void AttachBaseContext(const std::shared_ptr<Context> &base);

    /**
     * @brief Obtains the absolute path to the application-specific cache directory
     * on the primary external or shared storage device.
     *
     * @return Returns the absolute path to the application-specific cache directory on the external or
     * shared storage device; returns null if the external or shared storage device is temporarily unavailable.
     */
    std::string GetExternalCacheDir() override;

    /**
     * @brief Obtains the absolute path to the directory for storing files for the application on the
     * primary external or shared storage device.
     *
     * @param type Indicates the type of the file directory to return
     *
     * @return Returns the absolute path to the application file directory on the external or shared storage
     * device; returns null if the external or shared storage device is temporarily unavailable.
     */
    std::string GetExternalFilesDir(std::string &type) override;

    /**
     * @brief Obtains the directory for storing files for the application on the device's internal storage.
     *
     * @return Returns the application file directory.
     */
    std::string GetFilesDir() override;

    /**
     * @brief IsUpdatingConfigurations
     *
     * @return true  or false
     */
    bool IsUpdatingConfigurations() override;

    /**
     * @brief PrintDrawnCompleted
     *
     * @return true  or false
     */
    bool PrintDrawnCompleted() override;
    /**
     * @brief Obtains the absolute path which app created and will be excluded from automatic backup to remote storage.
     * The returned path maybe changed if the application is moved to an adopted storage device.
     *
     * @return The path of the directory holding application files that will not be automatically backed up to remote
     * storage.
     */
    std::string GetNoBackupFilesDir() override;

    /**
     * @brief Remove permissions for all users who have access to specific permissions
     *
     * @param permission Indicates the permission to unauth. This parameter cannot be null.
     * @param uri Indicates the URI to unauth. This parameter cannot be null.
     * @param uid Indicates the UID of the unauth to check.
     *
     */
    void UnauthUriPermission(const std::string &permission, const Uri &uri, int uid) override;

    /**
     * @brief Obtains the distributed file path.
     * If the distributed file path does not exist, the system creates one and returns the created path. This method is
     * applicable only to the context of an ability rather than that of an application.
     *
     * @return Returns the distributed file.
     */
    std::string GetDistributedDir() override;

    /**
     * @brief Sets the pattern of this Context based on the specified pattern ID.
     *
     * @param patternId Indicates the resource ID of the pattern to set.
     */
    void SetPattern(int patternId) override;

    /**
     * @brief Obtains the Context object of this ability.
     *
     * @return Returns the Context object of this ability.
     */
    std::shared_ptr<Context> GetAbilityPackageContext() override;

    /**
     * @brief Obtains the name of the current process.
     *
     * @return Returns the current process name.
     */
    std::string GetProcessName() override;

    /**
     * @brief InitResourceManager
     *
     * @param bundleInfo  BundleInfo
     */
    void InitResourceManager(BundleInfo &bundleInfo, std::shared_ptr<ContextDeal> &deal);

    /**
     * @brief Starts a new ability.
     * An ability using the AbilityInfo.AbilityType.SERVICE or AbilityInfo.AbilityType.PAGE template uses this method
     * to start a specific ability. The system locates the target ability from installed abilities based on the value
     * of the want parameter and then starts it. You can specify the ability to start using the want parameter.
     *
     * @param want Indicates the Want containing information about the target ability to start.
     *
     * @param requestCode Indicates the request code returned after the ability using the AbilityInfo.AbilityType.PAGE
     * template is started. You can define the request code to identify the results returned by abilities. The value
     * ranges from 0 to 65535. This parameter takes effect only on abilities using the AbilityInfo.AbilityType.PAGE
     * template.
     *
     * @return errCode ERR_OK on success, others on failure.
     */
    using ContextContainer::StartAbility;
    ErrCode StartAbility(const AAFwk::Want &Want, int requestCode) override;

    /**
     * @brief Starts a new ability with special ability start setting.
     *
     * @param want Indicates the Want containing information about the target ability to start.
     * @param requestCode Indicates the request code returned after the ability is started. You can define the request
     * code to identify the results returned by abilities. The value ranges from 0 to 65535.
     * @param abilityStartSetting Indicates the special start setting used in starting ability.
     *
     * @return errCode ERR_OK on success, others on failure.
     */
    ErrCode StartAbility(const Want &want, int requestCode, const AbilityStartSetting &abilityStartSetting) override;

    /**
     * @brief Destroys another ability you had previously started by calling Ability.startAbilityForResult
     * (ohos.aafwk.content.Want, int, ohos.aafwk.ability.startsetting.AbilityStartSetting) with the same requestCode
     * passed.
     *
     * @param requestCode Indicates the request code passed for starting the ability.
     *
     * @return errCode ERR_OK on success, others on failure.
     */
    ErrCode TerminateAbility(int requestCode) override;

    /**
     * @brief Destroys the current ability.
     *
     * @return errCode ERR_OK on success, others on failure.
     */
    ErrCode TerminateAbility() override;

    /**
     * @brief Obtains the bundle name of the ability that called the current ability.
     * You can use the obtained bundle name to check whether the calling ability is allowed to receive the data you will
     * send. If you did not use Ability.startAbilityForResult(ohos.aafwk.content.Want, int,
     * ohos.aafwk.ability.startsetting.AbilityStartSetting) to start the calling ability, null is returned.
     *
     * @return Returns the bundle name of the calling ability; returns null if no calling ability is available.
     */
    std::string GetCallingBundle() override;

    /**
     * @brief Obtains the ohos.bundle.ElementName object of the current ability.
     *
     * @return Returns the ohos.bundle.ElementName object of the current ability.
     */
    std::shared_ptr<ElementName> GetElementName();

    /**
     * @brief Obtains the ElementName of the ability that called the current ability.
     *
     * @return Returns the ElementName of the calling ability; returns null if no calling ability is available.
     */
    std::shared_ptr<ElementName> GetCallingAbility();

    /**
     * @brief Connects the current ability to an ability using the AbilityInfo.AbilityType.SERVICE template.
     *
     * @param want Indicates the want containing information about the ability to connect
     *
     * @param conn Indicates the callback object when the target ability is connected.
     *
     * @return True means success and false means failure
     */
    bool ConnectAbility(const Want &want, const sptr<AAFwk::IAbilityConnection> &conn) override;

    /**
     * @brief Disconnects the current ability from an ability
     *
     * @param conn Indicates the IAbilityConnection callback object passed by connectAbility after the connection
     *              is set up. The IAbilityConnection object uniquely identifies a connection between two abilities.
     *
     * @return errCode ERR_OK on success, others on failure.
     */
    ErrCode DisconnectAbility(const sptr<AAFwk::IAbilityConnection> &conn) override;

    /**
     * @brief Destroys another ability that uses the AbilityInfo.AbilityType.SERVICE template.
     * The current ability using either the AbilityInfo.AbilityType.SERVICE or AbilityInfo.AbilityType.PAGE
     * template can call this method to destroy another ability that uses the AbilityInfo.AbilityType.SERVICE
     * template. The current ability itself can be destroyed by calling the terminateAbility() method.
     *
     * @param want Indicates the Want containing information about the ability to destroy.
     *
     * @return Returns true if the ability is destroyed successfully; returns false otherwise.
     */
    virtual bool StopAbility(const AAFwk::Want &want) override;

    /**
     * @brief Obtains information about the current application. The returned application information includes basic
     * information such as the application name and application permissions.
     *
     * @return Returns the ApplicationInfo for the current application.
     */
    std::shared_ptr<ApplicationInfo> GetApplicationInfo() const override;

    /**
     * @brief Obtains the application-specific cache directory on the device's internal storage. The system
     * automatically deletes files from the cache directory if disk space is required elsewhere on the device.
     * Older files are always deleted first.
     *
     * @return Returns the application-specific cache directory.
     */
    std::string GetCacheDir() override;

    /**
     * @brief Obtains the application-specific code-cache directory on the device's internal storage.
     * The system will delete any files stored in this location both when your specific application is upgraded,
     * and when the entire platform is upgraded.
     *
     * @return Returns the application-specific code-cache directory.
     */
    std::string GetCodeCacheDir() override;

    /**
     * @brief Obtains the local database path.
     * If the local database path does not exist, the system creates one and returns the created path.
     *
     * @return Returns the local database file.
     */
    std::string GetDatabaseDir() override;

    /**
     * @brief Obtains the absolute path where all private data files of this application are stored.
     *
     * @return Returns the absolute path storing all private data files of this application.
     */
    std::string GetDataDir() override;

    /**
     * @brief Obtains the directory for storing custom data files of the application.
     * You can use the returned File object to create and access files in this directory. The files
     * can be accessible only by the current application.
     *
     * @param name Indicates the name of the directory to retrieve. This directory is created as part
     * of your application data.
     * @param mode Indicates the file operating mode. The value can be 0 or a combination of MODE_PRIVATE.
     *
     * @return Returns a File object for the requested directory.
     */
    std::string GetDir(const std::string &name, int mode) override;

    /**
     * @brief Obtains an IBundleMgr instance.
     * You can use this instance to obtain information about the application bundle.
     *
     * @return Returns an IBundleMgr instance.
     */
    sptr<IBundleMgr> GetBundleManager() const override;

    /**
     * @brief Obtains the path of the package containing the current ability. The returned path contains the resources,
     *  source code, and configuration files of a module.
     *
     * @return Returns the path of the package file.
     */
    std::string GetBundleCodePath() override;

    /**
     * @brief Obtains the bundle name of the current ability.
     *
     * @return Returns the bundle name of the current ability.
     */
    std::string GetBundleName() override;

    /**
     * @brief Obtains the path of the OHOS Ability Package (HAP} containing this ability.
     *
     * @return Returns the path of the HAP containing this ability.
     */
    std::string GetBundleResourcePath() override;

    /**
     * @brief Obtains the Context object of the application.
     *
     * @return Returns the Context object of the application.
     */
    std::shared_ptr<Context> GetApplicationContext() const override;

    /**
     * @brief Obtains the Context object of the application.
     *
     * @return Returns the Context object of the application.
     */
    std::shared_ptr<Context> GetContext() override;

    /**
     * @brief Obtains an ability manager.
     * The ability manager provides information about running processes and memory usage of an application.
     *
     * @return Returns an IAbilityManager instance.
     */
    sptr<AAFwk::IAbilityManager> GetAbilityManager() override;

    /**
     * Called when getting the ProcessInfo
     *
     * @return ProcessInfo
     */
    std::shared_ptr<ProcessInfo> GetProcessInfo() const override;

    /**
     * @brief Obtains the type of this application.
     *
     * @return Returns system if this application is a system application;
     * returns normal if it is released in official AppGallery;
     * returns other if it is released by a third-party vendor;
     * returns an empty string if the query fails.
     */
    std::string GetAppType() override;

    /**
     * @brief Obtains information about the current ability.
     * The returned information includes the class name, bundle name, and other information about the current ability.
     *
     * @return Returns the AbilityInfo object for the current ability.
     */
    const std::shared_ptr<AbilityInfo> GetAbilityInfo() override;

    /**
     * @brief Obtains the HapModuleInfo object of the application.
     *
     * @return Returns the HapModuleInfo object of the application.
     */
    std::shared_ptr<HapModuleInfo> GetHapModuleInfo() override;

    /**
     * @brief Creates a Context object for an application with the given bundle name.
     *
     * @param bundleName Indicates the bundle name of the application.
     * @param flag  Indicates the flag for creating a Context object. It can be 0, any of
     * the following values, or any combination of the following values: CONTEXT_IGNORE_SECURITY,
     * CONTEXT_INCLUDE_CODE, and CONTEXT_RESTRICTED. The value 0 indicates that there is no restriction
     * on creating contexts for applications.
     * @param accountId Indicates the account Id.
     *
     * @return Returns a Context object created for the specified application.
     */
    std::shared_ptr<Context> CreateBundleContext(std::string bundleName, int flag, int accountId = DEFAULT_ACCOUNT_ID);

    /**
     * @brief Obtains a resource manager.
     *
     * @return Returns a ResourceManager object.
     */
    std::shared_ptr<Global::Resource::ResourceManager> GetResourceManager() const override;

    /**
     * @brief Query whether the application of the specified PID and UID has been granted a certain permission
     *
     * @param permissions Indicates the list of permissions to be requested. This parameter cannot be null.
     * @param pid Process id
     * @param uid
     * @return Returns 0 (IBundleManager.PERMISSION_GRANTED) if the current process has the permission;
     * returns -1 (IBundleManager.PERMISSION_DENIED) otherwise.
     */
    virtual int VerifyPermission(const std::string &permission, int pid, int uid) override;

    /**
     * @brief Requests certain permissions from the system.
     * This method is called for permission request. This is an asynchronous method. When it is executed,
     * the Ability.onRequestPermissionsFromUserResult(int, String[], int[]) method will be called back.
     *
     * @param permissions Indicates the list of permissions to be requested. This parameter cannot be null.
     * @param permissionsState Indicates the list of permissions' state to be requested. This parameter cannot be null.
     * @param requestCode Indicates the request code to be passed to the Ability.onRequestPermissionsFromUserResult(int,
     * String[], int[]) callback method. This code cannot be a negative number.
     */
    virtual void RequestPermissionsFromUser(std::vector<std::string> &permissions, std::vector<int> &permissionsState,
        int requestCode) override;

    /**
     * @brief Deletes the specified private file associated with the application.
     *
     * @param fileName Indicates the name of the file to delete. The file name cannot contain path separators.
     *
     * @return Returns true if the file is deleted successfully; returns false otherwise.
     */
    bool DeleteFile(const std::string &fileName) override;

    /**
     * @brief Set deviceId/bundleName/abilityName of the calling ability
     *
     * @param deviceId deviceId of the calling ability
     *
     * @param bundleName bundleName of the calling ability
     *
     * @param abilityName abilityName of the calling ability
     */
    void SetCallingContext(const std::string &deviceId, const std::string &bundleName, const std::string &abilityName);

    /**
     * @brief Obtains information about the caller of this ability.
     *
     * @return Returns the caller information.
     */
    Uri GetCaller() override;

    /**
     * @brief Get the string of this Context based on the specified resource ID.
     *
     * @param resId Indicates the resource ID of the string to get.
     *
     * @return Returns the string of this Context.
     */
    std::string GetString(int resId) override;

    /**
     * @brief Get the string array of this Context based on the specified resource ID.
     *
     * @param resId Indicates the resource ID of the string array to get.
     *
     * @return Returns the string array of this Context.
     */
    std::vector<std::string> GetStringArray(int resId) override;

    /**
     * @brief Get the integer array of this Context based on the specified resource ID.
     *
     * @param resId Indicates the resource ID of the integer array to get.
     *
     * @return Returns the integer array of this Context.
     */
    std::vector<int> GetIntArray(int resId) override;

    /**
     * @brief Obtains the theme of this Context.
     *
     * @return theme Returns the theme of this Context.
     */
    std::map<std::string, std::string> GetTheme() override;

    /**
     * @brief Sets the theme of this Context based on the specified theme ID.
     *
     * @param themeId Indicates the resource ID of the theme to set.
     */
    void SetTheme(int themeId) override;

    /**
     * @brief Obtains the pattern of this Context.
     *
     * @return getPattern in interface Context
     */
    std::map<std::string, std::string> GetPattern() override;

    /**
     * @brief Get the color of this Context based on the specified resource ID.
     *
     * @param resId Indicates the resource ID of the color to get.
     *
     * @return Returns the color value of this Context.
     */
    int GetColor(int resId) override;

    /**
     * @brief Obtains the theme id of this {@code Context}.
     *
     * @return int Returns the theme id of this {@code Context}.
     */
    int GetThemeId() override;

    /**
     * @brief
     * Destroys this Service ability if the number of times it has been started equals the number represented by the
     * given {@code startId}. This method is the same as calling {@link #terminateAbility} to destroy this Service
     * ability, except that this method helps you avoid destroying it if a client has requested a Service
     * ability startup in {@link ohos.aafwk.ability.Ability#onCommand} but you are unaware of it.
     *
     * @param startId Indicates the number of startup times of this Service ability passed to
     *                {@link ohos.aafwk.ability.Ability#onCommand}. The {@code startId} is
     *                incremented by 1 every time this ability is started. For example,
     *                if this ability has been started for six times, the value of {@code startId} is {@code 6}.
     *
     * @return Returns {@code true} if the {@code startId} matches the number of startup times
     *         and this Service ability will be destroyed; returns {@code false} otherwise.
     */
    bool TerminateAbilityResult(int startId) override;

    /**
     * @brief Obtains the current display orientation of this ability.
     *
     * @return Returns the current display orientation.
     */
    int GetDisplayOrientation() override;

    /**
     * @brief Obtains the path storing the preference file of the application.
     *        If the preference file path does not exist, the system creates one and returns the created path.
     *
     * @return Returns the preference file path .
     */
    std::string GetPreferencesDir() override;

    /**
     * @brief Set color mode
     *
     * @param the value of color mode.
     */
    void SetColorMode(int mode) override;

    /**
     * @brief Obtains color mode.
     *
     * @return Returns the color mode value.
     */
    int GetColorMode() override;

    /**
     * @brief Obtains the unique ID of the mission containing this ability.
     *
     * @return Returns the unique mission ID.
     */
    int GetMissionId() override;

    /**
     * @brief Starts multiple abilities.
     *
     * @param wants Indicates the Want containing information array about the target ability to start.
     */
    void StartAbilities(const std::vector<AAFwk::Want> &wants) override;

    friend DataAbilityHelper;
    friend OHOS::DataShare::DataShareHelper;
    static int ABILITY_CONTEXT_DEFAULT_REQUEST_CODE;

protected:
    sptr<IRemoteObject> GetToken() override;
    sptr<IRemoteObject> token_;
    AAFwk::Want resultWant_;
    int resultCode_ = -1;
    std::string callingDeviceId_;
    std::string callingBundleName_;
    std::string callingAbilityName_;
    std::map<sptr<AAFwk::IAbilityConnection>, sptr<IRemoteObject>> abilityConnectionMap_;

private:
    /**
     * @brief Get Current Ability Type
     *
     * @return Current Ability Type
     */
    AppExecFwk::AbilityType GetAbilityInfoType();
    void GetPermissionDes(const std::string &permissionName, std::string &des);
};
}  // namespace AppExecFwk
}  // namespace OHOS
#endif  // FOUNDATION_APPEXECFWK_OHOS_ABILITY_CONTEXT_H
