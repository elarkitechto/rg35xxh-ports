/* Full 3.1.6144; pinned ARM64 ELF and metadata. */
/* Linux has no Android LVL Binder service. Skip its initialization rather
 * than raising a NullReferenceException; no response or purchase state is forged. */
static const struct game_patch game_patches[] = {
  {"LicenseVerification.ValidateLicense.AndroidBinderUnavailable", 0x81244c, {0xf81b0ffe, 0xa90167fa, 0xa9025ff8, 0xa90357f6}, 0, 0},

  {"AndroidHelper.GetAndroidID", 0x7f9154, {0xf81d0ffe, 0xa90157f6, 0xa9024ff4, 0xb00042f6}, 2, (uintptr_t)linux_device_id},
  {"AndroidHelper.GetDeviceModel", 0x7f93b0, {0xa9bd5ffe, 0xa90157f6, 0xa9024ff4, 0xb00042f6}, 2, (uintptr_t)linux_device_model},
  {"AndroidHelper.AllowAutorotation", 0x7f95ac, {0xf81d0ffe, 0xa90157f6, 0xa9024ff4, 0xb00042f6}, 1, 0},
  {"AndroidHelper.SetStatusBarState", 0x7f980c, {0xd10103ff, 0xa9015ffe, 0xa90257f6, 0xa9034ff4}, 0, 0},
  {"AndroidHelper.GetStatusBarHeight", 0x7f9a74, {0xf81d0ffe, 0xa90157f6, 0xa9024ff4, 0xb00042f6}, 1, 0},
  {"AndroidHelper.GetInternalDataPath", 0x7f9cb8, {0xa9bd5ffe, 0xa90157f6, 0xa9024ff4, 0xb00042f4}, 2, (uintptr_t)linux_internal_path},
  {"AndroidHelper.GetScreenDPI", 0x7f9f44, {0xf81d0ffe, 0xa90157f6, 0xa9024ff4, 0xb00042f3}, 1, 160},
  {"AndroidHelper.GetScreenPPI", 0x7fa264, {0xfc1c0fea, 0x6d00a3e9, 0xf9000ffe, 0xa90257f6}, 2, (uintptr_t)linux_screen_ppi},
  {"AndroidHelper.GetMaxTextureSize", 0x7fa5f4, {0xd10183ff, 0xf9000bfe, 0xa90267fa, 0xa9035ff8}, 1, 4096},
  {"AndroidHelper.UpdateHeroWidget", 0x7faea0, {0xf81b0ffe, 0xa90167fa, 0xa9025ff8, 0xa90357f6}, 0, 0},
  {"AndroidHelper.GetHeroWidgetExtras", 0x7fb144, {0xa9be57fe, 0xa9014ff4, 0xf00042d3, 0xf0003d14}, 2, (uintptr_t)linux_empty_string},
  {"AndroidHelper.IsGroot", 0x7fb2a0, {0xf81e0ffe, 0xa9014ff4, 0xf00042d4, 0xf0003d13}, 1, 0},
  {"Initialize.InitSocialPlatform", 0x82a024, {0xa9be57fe, 0xa9014ff4, 0xb0004174, 0x39430288}, 0, 0},
  {"Initialize.CheckAchievements", 0x82a128, {0xd10103ff, 0xa9015ffe, 0xa90257f6, 0xa9034ff4}, 0, 0},
  {"AndroidHelper.GetLocaleData", 0x7fabcc, {0xa9be57fe, 0xa9014ff4, 0x900042f3, 0x90003d34}, 2, (uintptr_t)linux_locale},
  {"GooglePlayData.IsAvailable", 0x7fea48, {0xf81e0ffe, 0xa9014ff4, 0x900042d3, 0xf0003cf4}, 1, 0},
};
