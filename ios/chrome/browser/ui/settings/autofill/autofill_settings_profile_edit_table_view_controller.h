// Copyright 2023 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IOS_CHROME_BROWSER_UI_SETTINGS_AUTOFILL_AUTOFILL_SETTINGS_PROFILE_EDIT_TABLE_VIEW_CONTROLLER_H_
#define IOS_CHROME_BROWSER_UI_SETTINGS_AUTOFILL_AUTOFILL_SETTINGS_PROFILE_EDIT_TABLE_VIEW_CONTROLLER_H_

#import "ios/chrome/browser/ui/autofill/autofill_profile_edit_handler.h"
#import "ios/chrome/browser/ui/settings/autofill/autofill_edit_table_view_controller.h"

@protocol AutofillProfileEditTableViewController;

// The table view for the Autofill profile edit settings.
@interface AutofillSettingsProfileEditTableViewController
    : AutofillEditTableViewController

@property(nonatomic, weak) id<AutofillProfileEditHandler> handler;

@end

#endif  // IOS_CHROME_BROWSER_UI_SETTINGS_AUTOFILL_AUTOFILL_SETTINGS_PROFILE_EDIT_TABLE_VIEW_CONTROLLER_H_
