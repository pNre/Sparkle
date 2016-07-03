//
//  SUUpdaterDelegate.h
//  Sparkle
//
//  Created by Mayur Pawashe on 3/12/16.
//  Copyright © 2016 Sparkle Project. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SUExport.h"

@protocol SUVersionComparison;
@class SUUpdater, SUAppcast, SUAppcastItem;

// -----------------------------------------------------------------------------
// SUUpdater Notifications for events that might be interesting to more than just the delegate
// The updater will be the notification object
// -----------------------------------------------------------------------------
SU_EXPORT extern NSString *const SUUpdaterDidFinishLoadingAppCastNotification;
SU_EXPORT extern NSString *const SUUpdaterDidFindValidUpdateNotification;
SU_EXPORT extern NSString *const SUUpdaterDidNotFindUpdateNotification;
SU_EXPORT extern NSString *const SUUpdaterWillRestartNotification;
#define SUUpdaterWillRelaunchApplicationNotification SUUpdaterWillRestartNotification;
#define SUUpdaterWillInstallUpdateNotification SUUpdaterWillRestartNotification;

// Key for the SUAppcastItem object in the SUUpdaterDidFindValidUpdateNotification userInfo
SU_EXPORT extern NSString *const SUUpdaterAppcastItemNotificationKey;
// Key for the SUAppcast object in the SUUpdaterDidFinishLoadingAppCastNotification userInfo
SU_EXPORT extern NSString *const SUUpdaterAppcastNotificationKey;

// -----------------------------------------------------------------------------
//	SUUpdater Delegate:
// -----------------------------------------------------------------------------

/*!
 Provides methods to control the behavior of an SUUpdater object.
 */
@protocol SUUpdaterDelegate <NSObject>
@optional

/*!
 Returns whether to allow Sparkle to pop up.
 
 For example, this may be used to prevent Sparkle from interrupting a setup assistant.
 
 \param updater The SUUpdater instance.
 */
- (BOOL)updaterMayCheckForUpdates:(SUUpdater *)updater;

/*!
 Returns additional parameters to append to the appcast URL's query string.
 
 This is potentially based on whether or not Sparkle will also be sending along the system profile.
 
 \param updater The SUUpdater instance.
 \param sendingProfile Whether the system profile will also be sent.
 
 \return An array of dictionaries with keys: "key", "value", "displayKey", "displayValue", the latter two being specifically for display to the user.
 */
- (NSArray *)feedParametersForUpdater:(SUUpdater *)updater sendingSystemProfile:(BOOL)sendingProfile;

/*!
 Returns a custom appcast URL.
 
 Override this to dynamically specify the entire URL.
 
 \param updater The SUUpdater instance.
 */
- (NSString *)feedURLStringForUpdater:(SUUpdater *)updater;

/*!
 Returns whether Sparkle should prompt the user about automatic update checks.
 
 Use this to override the default behavior.
 
 \param updater The SUUpdater instance.
 */
- (BOOL)updaterShouldPromptForPermissionToCheckForUpdates:(SUUpdater *)updater;

/*!
 Called after Sparkle has downloaded the appcast from the remote server.
 
 Implement this if you want to do some special handling with the appcast once it finishes loading.
 
 \param updater The SUUpdater instance.
 \param appcast The appcast that was downloaded from the remote server.
 */
- (void)updater:(SUUpdater *)updater didFinishLoadingAppcast:(SUAppcast *)appcast;

/*!
 Returns the item in the appcast corresponding to the update that should be installed.
 
 If you're using special logic or extensions in your appcast,
 implement this to use your own logic for finding a valid update, if any,
 in the given appcast.
 
 \param appcast The appcast that was downloaded from the remote server.
 \param updater The SUUpdater instance.
 */
- (SUAppcastItem *)bestValidUpdateInAppcast:(SUAppcast *)appcast forUpdater:(SUUpdater *)updater;

/*!
 Called when a valid update is found by the update driver.
 
 \param updater The SUUpdater instance.
 \param item The appcast item corresponding to the update that is proposed to be installed.
 */
- (void)updater:(SUUpdater *)updater didFindValidUpdate:(SUAppcastItem *)item;

/*!
 Called when a valid update is not found.
 
 \param updater The SUUpdater instance.
 */
- (void)updaterDidNotFindUpdate:(SUUpdater *)updater;

/*!
 Returns whether the release notes (if available) should be downloaded after an update is found and shown.
 
 This is specifically for the releaseNotesLink element in the appcast.
 
 \param updater The SUUpdater instance.
 
 \return \c YES to download and show the release notes if available
 */
- (BOOL)updaterShouldDownloadReleaseNotes:(SUUpdater *)updater;

/*!
 Called immediately before downloading the specified update.
 
 \param updater The SUUpdater instance.
 \param item The appcast item corresponding to the update that is proposed to be downloaded.
 \param request The mutable URL request that will be used to download the update.
 */
- (void)updater:(SUUpdater *)updater willDownloadUpdate:(SUAppcastItem *)item withRequest:(NSMutableURLRequest *)request;

/*!
 Called after the specified update failed to download.
 
 \param updater The SUUpdater instance.
 \param item The appcast item corresponding to the update that failed to download.
 \param error The error generated by the failed download.
 */
- (void)updater:(SUUpdater *)updater failedToDownloadUpdate:(SUAppcastItem *)item error:(NSError *)error;

/*!
 Called when the user clicks the cancel button while and update is being downloaded.
 
 \param updater The SUUpdater instance.
 */
- (void)userDidCancelDownload:(SUUpdater *)updater;

/*!
 Called immediately before installing the specified update.
 
 \param updater The SUUpdater instance.
 \param item The appcast item corresponding to the update that is proposed to be installed.
 */
- (void)updater:(SUUpdater *)updater willInstallUpdate:(SUAppcastItem *)item;

/*!
 \deprecated Use -updater:shouldPostponeRelaunchForUpdate:untilInvokingBlock: instead
 Returns whether the relaunch should be delayed in order to perform other tasks.
 
 This is not called if the user didn't relaunch on the previous update,
 in that case it will immediately restart.
 
 This may also not be called if the application is not going to relaunch after it terminates.
 
 \param updater The SUUpdater instance.
 \param item The appcast item corresponding to the update that is proposed to be installed.
 \param invocation The invocation that must be completed before continuing with the relaunch.
 
 \return \c YES to delay the relaunch until \p invocation is invoked.
 */
- (BOOL)updater:(SUUpdater *)updater shouldPostponeRelaunchForUpdate:(SUAppcastItem *)item untilInvoking:(NSInvocation *)invocation __deprecated;

/*!
 Returns whether the relaunch should be delayed in order to perform other tasks.
 
 This is not called if the user didn't relaunch on the previous update,
 in that case it will immediately restart.
 
 This may also not be called if the application is not going to relaunch after it terminates.
 
 \param updater The SUUpdater instance.
 \param item The appcast item corresponding to the update that is proposed to be installed.
 \param installHandler The install handler that must be completed before continuing with the relaunch.
 
 \return \c YES to delay the relaunch until \p installHandler is invoked.
 */
- (BOOL)updater:(SUUpdater *)updater shouldPostponeRelaunchForUpdate:(SUAppcastItem *)item untilInvokingBlock:(void (^)(void))installHandler;

/*!
 Returns whether the application should be relaunched at all.
 
 Some apps \b cannot be relaunched under certain circumstances.
 This method can be used to explicitly prevent a relaunch.
 
 \param updater The SUUpdater instance.
 */
- (BOOL)updaterShouldRelaunchApplication:(SUUpdater *)updater;

/*!
 Called immediately before relaunching.
 
 \param updater The SUUpdater instance.
 */
- (void)updaterWillRelaunchApplication:(SUUpdater *)updater;

/*!
 Returns an object that compares version numbers to determine their arithmetic relation to each other.
 
 This method allows you to provide a custom version comparator.
 If you don't implement this method or return \c nil,
 the standard version comparator will be used. Note that the
 standard version comparator may be used during installation for preventing
 a downgrade, even if you provide a custom comparator here.
 
 \sa SUStandardVersionComparator
 
 \param updater The SUUpdater instance.
 */
- (id<SUVersionComparison>)versionComparatorForUpdater:(SUUpdater *)updater;

/*!
 @deprecated This method is no longer used. See SUStandardUserUpdaterDriver delegate instead.
 */
// Don't specify SUVersionDisplay in the return type, otherwise we'd have to forward declare a protocol that isn't used here anymore
- (id)versionDisplayerForUpdater:(SUUpdater *)updater __deprecated;

/*!
 Returns the path to the application which is used to relaunch after the update is installed.
 
 The installer also waits for the termination of the application at this path.
 
 The default is the path of the host bundle.
 
 \param updater The SUUpdater instance.
 */
- (NSString *)pathToRelaunchForUpdater:(SUUpdater *)updater;

/*!
 Returns whether or not the updater should inherit its parent's privileges during installation
 
 Use this to override the default behavior which is to not inherit privileges.
 
 If privileges are inherited, then no authorization prompt will show up, and
 the installation will succeed only if the process has sufficient privileges.
 
 On the other hand, if privileges are not inherited (the default), the installer may
 request an authorization prompt to obtain additional privileges.
 
 \param updater The SUUpdater instance.
 */
- (BOOL)updaterShouldInheritInstallPrivileges:(SUUpdater *)updater;

/*!
 Returns the decryption password (if any) which is used to extract the update archive DMG.
 
 Return nil if no password should be used.
 
 \param updater The SUUpdater instance.
 */
- (NSString *)decryptionPasswordForUpdater:(SUUpdater *)updater;

/*!
 Called before an updater shows a modal alert window,
 to give the host the opportunity to hide attached windows that may get in the way.
 
 \param updater The SUUpdater instance.
 
 @deprecated See SUStandardUserUpdaterDriver delegate instead. This method is no longer invoked.
 */
- (void)updaterWillShowModalAlert:(SUUpdater *)updater __deprecated;

/*!
 Called after an updater shows a modal alert window,
 to give the host the opportunity to hide attached windows that may get in the way.
 
 \param updater The SUUpdater instance.
 
 @deprecated See SUStandardUserUpdaterDriver delegate instead. This method is no longer invoked.
 */
- (void)updaterDidShowModalAlert:(SUUpdater *)updater __deprecated;

/*!
 Called when an update is scheduled to be silently installed on quit.
 
 \param updater The SUUpdater instance.
 \param item The appcast item corresponding to the update that is proposed to be installed.
 \param invocation Can be used to trigger an immediate silent install and relaunch.
 */
- (void)updater:(SUUpdater *)updater willInstallUpdateOnQuit:(SUAppcastItem *)item immediateInstallationInvocation:(NSInvocation *)invocation;

/*!
 Calls after an update that was scheduled to be silently installed on quit has been canceled.
 
 \param updater The SUUpdater instance.
 \param item The appcast item corresponding to the update that was proposed to be installed.
 
  \deprecated This method is no longer invoked. The installer will try to its best ability to install the update.
 */
- (void)updater:(SUUpdater *)updater didCancelInstallUpdateOnQuit:(SUAppcastItem *)item __deprecated;

/*!
 Called after an update is aborted due to an error.
 
 \param updater The SUUpdater instance.
 \param error The error that caused the abort
 */
- (void)updater:(SUUpdater *)updater didAbortWithError:(NSError *)error;

@end
