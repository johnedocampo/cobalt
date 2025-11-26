// Copyright 2025 The Cobalt Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "starboard/android/shared/media_capabilities_cache.h"

#include <memory>
#include <set>
#include <string>
#include <vector>

#include "starboard/media.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace starboard {
namespace {

using ::testing::ByMove;
using ::testing::Return;
using ::testing::SetArgPointee;

typedef std::vector<std::unique_ptr<AudioCodecCapability>>
    AudioCodecCapabilities;
typedef std::vector<std::unique_ptr<VideoCodecCapability>>
    VideoCodecCapabilities;

class MockAudioCodecCapability : public AudioCodecCapability {
 public:
  MockAudioCodecCapability(const std::string& name,
                           const Range& supported_bitrates)
      : AudioCodecCapability(name, supported_bitrates) {}
};

class MockVideoCodecCapability : public VideoCodecCapability {
 public:
  MockVideoCodecCapability(const std::string& name,
                           bool is_secure_required,
                           bool is_secure_supported,
                           bool is_tunnel_mode_required,
                           bool is_tunnel_mode_supported,
                           bool is_software_decoder,
                           bool is_hdr_capable,
                           const Range& supported_widths,
                           const Range& supported_heights,
                           const Range& supported_bitrates,
                           const Range& supported_frame_rates)
      : VideoCodecCapability(name,
                             is_secure_required,
                             is_secure_supported,
                             is_tunnel_mode_required,
                             is_tunnel_mode_supported,
                             is_software_decoder,
                             is_hdr_capable,
                             supported_widths,
                             supported_heights,
                             supported_bitrates,
                             supported_frame_rates) {}
  MOCK_METHOD(bool,
              AreResolutionAndRateSupported,
              (int frame_width, int frame_height, int fps),
              (const, override));
};

class MockMediaCapabilitiesProvider : public MediaCapabilitiesProvider {
 public:
  MOCK_METHOD(bool, GetIsWidevineSupported, (), (override));
  MOCK_METHOD(bool, GetIsCbcsSchemeSupported, (), (override));
  MOCK_METHOD(std::set<SbMediaTransferId>,
              GetSupportedHdrTypes,
              (),
              (override));
  MOCK_METHOD(bool,
              GetIsPassthroughSupported,
              (SbMediaAudioCodec codec),
              (override));
  MOCK_METHOD(bool,
              GetAudioConfiguration,
              (int index, SbMediaAudioConfiguration* configuration),
              (override));
  MOCK_METHOD(std::string,
              FindAudioDecoder,
              (const std::string& mime_type, int bitrate),
              (override));
  MOCK_METHOD(std::string,
              FindVideoDecoder,
              (const std::string& mime_type,
               bool must_support_secure,
               bool must_support_hdr,
               bool require_software_codec,
               bool must_support_tunnel_mode,
               int frame_width,
               int frame_height,
               int bitrate,
               int fps),
              (override));
  MOCK_METHOD(
      void,
      GetCodecCapabilities,
      ((std::map<std::string, AudioCodecCapabilities>)&audio_codec_capabilities,
       (std::map<std::string,
                 VideoCodecCapabilities>)&video_codec_capabilities),
      (override));
};

class MediaCapabilitiesCacheTest : public ::testing::Test {
 protected:
  void SetUp() override {
    auto mock_media_capabilities_provider =
        std::make_unique<MockMediaCapabilitiesProvider>();
    mock_media_capabilities_provider_ = mock_media_capabilities_provider.get();

    cache_ = MediaCapabilitiesCache::CreateForTest(
        std::move(mock_media_capabilities_provider));
    cache_->SetCacheEnabled(true);
  }

  MockMediaCapabilitiesProvider* mock_media_capabilities_provider_;
  std::unique_ptr<MediaCapabilitiesCache> cache_;
};

TEST_F(MediaCapabilitiesCacheTest, IsWidevineSupported_EnabledCache) {
  EXPECT_CALL(*mock_media_capabilities_provider_, GetIsWidevineSupported())
      .Times(1)
      .WillOnce(Return(true));
  EXPECT_TRUE(cache_->IsWidevineSupported());
  EXPECT_TRUE(cache_->IsWidevineSupported());
}

TEST_F(MediaCapabilitiesCacheTest, IsWidevineSupported_DisabledCache) {
  cache_->SetCacheEnabled(false);
  EXPECT_CALL(*mock_media_capabilities_provider_, GetIsWidevineSupported())
      .Times(2)
      .WillOnce(Return(true))
      .WillOnce(Return(false));
  EXPECT_TRUE(cache_->IsWidevineSupported());
  EXPECT_FALSE(cache_->IsWidevineSupported());
}

TEST_F(MediaCapabilitiesCacheTest, IsCbcsSchemeSupported_EnabledCache) {
  EXPECT_CALL(*mock_media_capabilities_provider_, GetIsCbcsSchemeSupported())
      .Times(1)
      .WillOnce(Return(true));
  EXPECT_TRUE(cache_->IsCbcsSchemeSupported());
  EXPECT_TRUE(cache_->IsCbcsSchemeSupported());
}

TEST_F(MediaCapabilitiesCacheTest, IsCbcsSchemeSupported_DisabledCache) {
  cache_->SetCacheEnabled(false);
  EXPECT_CALL(*mock_media_capabilities_provider_, GetIsCbcsSchemeSupported())
      .Times(2)
      .WillOnce(Return(true))
      .WillOnce(Return(false));
  EXPECT_TRUE(cache_->IsCbcsSchemeSupported());
  EXPECT_FALSE(cache_->IsCbcsSchemeSupported());
}

TEST_F(MediaCapabilitiesCacheTest,
       IsHDRTransferCharacteristicsSupported_EnabledCache) {
  std::set<SbMediaTransferId> supported_types = {kSbMediaTransferIdSmpteSt2084};
  EXPECT_CALL(*mock_media_capabilities_provider_, GetSupportedHdrTypes())
      .Times(1)
      .WillOnce(Return(supported_types));

  EXPECT_TRUE(cache_->IsHDRTransferCharacteristicsSupported(
      kSbMediaTransferIdSmpteSt2084));
  EXPECT_FALSE(cache_->IsHDRTransferCharacteristicsSupported(
      kSbMediaTransferIdAribStdB67));
  EXPECT_TRUE(cache_->IsHDRTransferCharacteristicsSupported(
      kSbMediaTransferIdSmpteSt2084));
  EXPECT_FALSE(cache_->IsHDRTransferCharacteristicsSupported(
      kSbMediaTransferIdAribStdB67));
}

TEST_F(MediaCapabilitiesCacheTest,
       IsHDRTransferCharacteristicsSupported_DisabledCache) {
  cache_->SetCacheEnabled(false);
  std::set<SbMediaTransferId> supported_types = {kSbMediaTransferIdSmpteSt2084};
  EXPECT_CALL(*mock_media_capabilities_provider_, GetSupportedHdrTypes())
      .Times(2)
      .WillRepeatedly(Return(supported_types));

  EXPECT_TRUE(cache_->IsHDRTransferCharacteristicsSupported(
      kSbMediaTransferIdSmpteSt2084));
  EXPECT_FALSE(cache_->IsHDRTransferCharacteristicsSupported(
      kSbMediaTransferIdAribStdB67));
}

TEST_F(MediaCapabilitiesCacheTest, IsPassthroughSupported_EnabledCache) {
  EXPECT_CALL(*mock_media_capabilities_provider_,
              GetIsPassthroughSupported(kSbMediaAudioCodecAc3))
      .Times(1)
      .WillOnce(Return(true));
  EXPECT_TRUE(cache_->IsPassthroughSupported(kSbMediaAudioCodecAc3));
  EXPECT_TRUE(cache_->IsPassthroughSupported(kSbMediaAudioCodecAc3));

  EXPECT_CALL(*mock_media_capabilities_provider_,
              GetIsPassthroughSupported(kSbMediaAudioCodecEac3))
      .Times(1)
      .WillOnce(Return(false));
  EXPECT_FALSE(cache_->IsPassthroughSupported(kSbMediaAudioCodecEac3));
  EXPECT_FALSE(cache_->IsPassthroughSupported(kSbMediaAudioCodecEac3));
}

TEST_F(MediaCapabilitiesCacheTest, IsPassthroughSupported_DisabledCache) {
  cache_->SetCacheEnabled(false);
  EXPECT_CALL(*mock_media_capabilities_provider_,
              GetIsPassthroughSupported(kSbMediaAudioCodecAc3))
      .Times(2)
      .WillRepeatedly(Return(true));
  EXPECT_TRUE(cache_->IsPassthroughSupported(kSbMediaAudioCodecAc3));
  EXPECT_TRUE(cache_->IsPassthroughSupported(kSbMediaAudioCodecAc3));

  EXPECT_CALL(*mock_media_capabilities_provider_,
              GetIsPassthroughSupported(kSbMediaAudioCodecEac3))
      .Times(2)
      .WillRepeatedly(Return(false));
  EXPECT_FALSE(cache_->IsPassthroughSupported(kSbMediaAudioCodecEac3));
  EXPECT_FALSE(cache_->IsPassthroughSupported(kSbMediaAudioCodecEac3));
}

TEST_F(MediaCapabilitiesCacheTest, GetAudioConfiguration_EnabledCache) {
  std::vector<SbMediaAudioConfiguration> configs = {
      {kSbMediaAudioConnectorHdmi, 0, kSbMediaAudioCodingTypeAc3, 2},
      {kSbMediaAudioConnectorHdmi, 0, kSbMediaAudioCodingTypeDolbyDigitalPlus,
       6},
  };
  EXPECT_CALL(*mock_media_capabilities_provider_,
              GetAudioConfiguration(0, testing::_))
      .WillOnce(
          testing::DoAll(testing::SetArgPointee<1>(configs[0]), Return(true)));

  EXPECT_CALL(*mock_media_capabilities_provider_,
              GetAudioConfiguration(1, testing::_))
      .WillOnce(
          testing::DoAll(testing::SetArgPointee<1>(configs[1]), Return(true)));

  EXPECT_CALL(*mock_media_capabilities_provider_,
              GetAudioConfiguration(2, testing::_))
      .WillOnce(Return(false));

  SbMediaAudioConfiguration config;
  EXPECT_TRUE(cache_->GetAudioConfiguration(0, &config));
  EXPECT_EQ(config.coding_type, kSbMediaAudioCodingTypeAc3);

  EXPECT_TRUE(cache_->GetAudioConfiguration(1, &config));
  EXPECT_EQ(config.coding_type, kSbMediaAudioCodingTypeDolbyDigitalPlus);

  EXPECT_FALSE(cache_->GetAudioConfiguration(2, &config));
}

TEST_F(MediaCapabilitiesCacheTest, GetAudioConfiguration_DisabledCache) {
  cache_->SetCacheEnabled(false);
  std::vector<SbMediaAudioConfiguration> configs = {
      {kSbMediaAudioConnectorHdmi, 0, kSbMediaAudioCodingTypeAc3, 2},
      {kSbMediaAudioConnectorHdmi, 0, kSbMediaAudioCodingTypeDolbyDigitalPlus,
       6},
  };

  EXPECT_CALL(*mock_media_capabilities_provider_,
              GetAudioConfiguration(0, testing::_))
      .WillOnce(
          testing::DoAll(testing::SetArgPointee<1>(configs[0]), Return(true)));

  EXPECT_CALL(*mock_media_capabilities_provider_,
              GetAudioConfiguration(1, testing::_))
      .WillOnce(
          testing::DoAll(testing::SetArgPointee<1>(configs[1]), Return(true)));

  EXPECT_CALL(*mock_media_capabilities_provider_,
              GetAudioConfiguration(2, testing::_))
      .WillOnce(Return(false));

  SbMediaAudioConfiguration config;
  EXPECT_TRUE(cache_->GetAudioConfiguration(0, &config));
  EXPECT_EQ(config.coding_type, kSbMediaAudioCodingTypeAc3);

  EXPECT_TRUE(cache_->GetAudioConfiguration(1, &config));
  EXPECT_EQ(config.coding_type, kSbMediaAudioCodingTypeDolbyDigitalPlus);

  EXPECT_FALSE(cache_->GetAudioConfiguration(2, &config));
}

TEST_F(MediaCapabilitiesCacheTest, HasAudioDecoderFor_EnabledCache) {
  std::string name = "fake codec";
  Range supported_bitrates = Range(0, 1000);

  std::map<std::string, AudioCodecCapabilities>
      mock_audio_codec_capabilities_map;
  mock_audio_codec_capabilities_map["fake mime_type"].push_back(
      std::make_unique<MockAudioCodecCapability>(name, supported_bitrates));

  EXPECT_CALL(*mock_media_capabilities_provider_,
              GetCodecCapabilities(::testing::_, ::testing::_))
      .WillOnce(
          [&](std::map<std::string, AudioCodecCapabilities>& audio_map_internal,
              std::map<std::string, VideoCodecCapabilities>&
                  video_map_internal) {
            audio_map_internal = std::move(mock_audio_codec_capabilities_map);
          });

  EXPECT_TRUE(cache_->HasAudioDecoderFor("fake mime_type", 500));
  EXPECT_FALSE(cache_->HasAudioDecoderFor("fake mime_type", 2000));
  EXPECT_FALSE(cache_->HasAudioDecoderFor("non-existent mime_type", 500));
}

TEST_F(MediaCapabilitiesCacheTest, HasAudioDecoderFor_DisabledCache) {
  cache_->SetCacheEnabled(false);
  EXPECT_CALL(*mock_media_capabilities_provider_,
              FindAudioDecoder(::testing::_, ::testing::_))
      .Times(2)
      .WillOnce(Return("non-empty string"))
      .WillOnce(Return(""));

  EXPECT_TRUE(cache_->HasAudioDecoderFor("test string", 100));
  EXPECT_FALSE(cache_->HasAudioDecoderFor("test string", 100));
}

TEST_F(MediaCapabilitiesCacheTest, HasVideoDecoderFor_EnabledCache) {
  std::string name = "fake mime_type";
  bool is_secure_supported = true;
  bool is_secure_required = true;

  bool is_tunnel_mode_supported = true;
  bool is_tunnel_mode_required = true;
  bool is_software_decoder = false;
  bool is_hdr_capable = true;
  Range supported_bitrates = Range(0, 1000);
  Range supported_frame_rates = Range(0, 60);
  Range supported_widths = Range(0, 100);
  Range supported_heights = Range(0, 500);

  std::map<std::string, VideoCodecCapabilities>
      mock_video_codec_capabilities_map;
  std::unique_ptr mock_video_capability =
      std::make_unique<MockVideoCodecCapability>(
          name, is_secure_supported, is_secure_required,
          is_tunnel_mode_supported, is_tunnel_mode_required,
          is_software_decoder, is_hdr_capable, supported_widths,
          supported_heights, supported_bitrates, supported_frame_rates);

  // Save the pointer to the mock video capability to set its expected call.
  MockVideoCodecCapability* raw_capability_ptr = mock_video_capability.get();
  mock_video_codec_capabilities_map[name].push_back(
      std::move(mock_video_capability));

  EXPECT_CALL(*mock_media_capabilities_provider_,
              GetCodecCapabilities(::testing::_, ::testing::_))
      .WillOnce(
          [&](std::map<std::string, AudioCodecCapabilities>& audio_map_internal,
              std::map<std::string, VideoCodecCapabilities>&
                  video_map_internal) {
            video_map_internal = std::move(mock_video_codec_capabilities_map);
          });
  EXPECT_CALL(
      *raw_capability_ptr,
      AreResolutionAndRateSupported(::testing::_, ::testing::_, ::testing::_))
      .Times(1)
      .WillOnce(Return(true));

  EXPECT_TRUE(
      cache_->HasVideoDecoderFor(name, is_secure_supported, is_hdr_capable,
                                 is_tunnel_mode_supported, 50, 50, 50, 50));
  EXPECT_FALSE(cache_->HasVideoDecoderFor("fake mime_type", false, false, false,
                                          -1, -1, -1, -1));
  EXPECT_FALSE(cache_->HasVideoDecoderFor("non-existent mime_type", true, true,
                                          true, 50, 50, 50, 50));
}

TEST_F(MediaCapabilitiesCacheTest, HasVideoDecoderFor_DisabledCache) {
  cache_->SetCacheEnabled(false);
  EXPECT_CALL(*mock_media_capabilities_provider_,
              FindVideoDecoder(testing::_, testing::_, testing::_, testing::_,
                               testing::_, testing::_, testing::_, testing::_,
                               testing::_))
      .Times(2)
      .WillOnce(Return("Non-empty string"))
      .WillOnce(Return(""));

  EXPECT_TRUE(cache_->HasVideoDecoderFor("fake mime_type", true, true, true, 50,
                                         50, 50, 50));
  EXPECT_FALSE(cache_->HasVideoDecoderFor("fake mime_type", true, true, true,
                                          50, 50, 50, 50));
}

TEST_F(MediaCapabilitiesCacheTest, ClearCacheClearsAllValues) {
  SbMediaAudioConfiguration config;

  std::string audio_mime = "audio/test";
  std::string video_mime = "video/test";

  // 1. Prepare Audio Mock
  std::map<std::string, AudioCodecCapabilities> mock_audio_map;
  mock_audio_map[audio_mime].push_back(
      std::make_unique<MockAudioCodecCapability>(audio_mime, Range(0, 200000)));

  // 2. Prepare Video Mock
  std::map<std::string, VideoCodecCapabilities> mock_video_map;
  auto mock_video_cap = std::make_unique<MockVideoCodecCapability>(
      video_mime, true, true, true, true, false, true, Range(0, 1920),
      Range(0, 1080), Range(0, 10000), Range(0, 60));

  // Save raw pointer to set expectations on the object inside the map
  MockVideoCodecCapability* raw_video_cap = mock_video_cap.get();
  mock_video_map[video_mime].push_back(std::move(mock_video_cap));

  EXPECT_CALL(*raw_video_cap,
              AreResolutionAndRateSupported(testing::_, testing::_, testing::_))
      .Times(1)  // Should only be called BEFORE the clear
      .WillOnce(Return(true));

  EXPECT_CALL(*mock_media_capabilities_provider_, GetIsWidevineSupported())
      .Times(2)
      .WillOnce(Return(true))
      .WillOnce(Return(false));

  EXPECT_CALL(*mock_media_capabilities_provider_, GetIsCbcsSchemeSupported())
      .Times(2)
      .WillOnce(Return(true))
      .WillOnce(Return(false));

  EXPECT_CALL(*mock_media_capabilities_provider_, GetSupportedHdrTypes())
      .Times(2)
      .WillRepeatedly(
          Return(std::set<SbMediaTransferId>{kSbMediaTransferIdSmpteSt2084}));

  EXPECT_CALL(*mock_media_capabilities_provider_,
              GetIsPassthroughSupported(testing::_))
      .Times(2)
      .WillOnce(Return(true))
      .WillOnce(Return(false));

  EXPECT_CALL(*mock_media_capabilities_provider_,
              GetAudioConfiguration(testing::_, testing::_))
      .Times(4)
      .WillOnce(Return(true))
      .WillOnce(Return(false))
      .WillOnce(Return(true))
      .WillOnce(Return(false));

  EXPECT_CALL(*mock_media_capabilities_provider_,
              GetCodecCapabilities(testing::_, testing::_))
      .Times(2)
      .WillOnce([&](std::map<std::string, AudioCodecCapabilities>& audio_out,
                    std::map<std::string, VideoCodecCapabilities>& video_out) {
        audio_out = std::move(mock_audio_map);
        video_out = std::move(mock_video_map);
      })
      .WillOnce(Return());  // Second call returns nothing/empty

  EXPECT_TRUE(cache_->IsWidevineSupported());
  EXPECT_TRUE(cache_->IsCbcsSchemeSupported());
  EXPECT_TRUE(cache_->IsHDRTransferCharacteristicsSupported(
      kSbMediaTransferIdSmpteSt2084));
  EXPECT_TRUE(cache_->IsPassthroughSupported(kSbMediaAudioCodecAc3));
  EXPECT_TRUE(cache_->GetAudioConfiguration(0, &config));
  EXPECT_FALSE(cache_->HasAudioDecoderFor("audio/mp4", 192000));

  // Call all cache functions again to ensure that the provider functions
  // are not being called to retrieve the values.
  EXPECT_TRUE(cache_->IsWidevineSupported());
  EXPECT_TRUE(cache_->IsCbcsSchemeSupported());
  EXPECT_TRUE(cache_->IsHDRTransferCharacteristicsSupported(
      kSbMediaTransferIdSmpteSt2084));
  EXPECT_TRUE(cache_->IsPassthroughSupported(kSbMediaAudioCodecAc3));
  EXPECT_TRUE(cache_->GetAudioConfiguration(0, &config));
  EXPECT_FALSE(cache_->HasAudioDecoderFor("audio/mp4", 192000));
  EXPECT_TRUE(cache_->HasAudioDecoderFor(audio_mime, 128000));
  EXPECT_TRUE(cache_->HasVideoDecoderFor(video_mime, true, true, true, 1920,
                                         1080, 5000, 30));

  cache_->ClearCache();

  EXPECT_FALSE(cache_->IsWidevineSupported());
  EXPECT_FALSE(cache_->IsCbcsSchemeSupported());
  EXPECT_FALSE(cache_->IsHDRTransferCharacteristicsSupported(
      kSbMediaTransferIdAribStdB67));
  EXPECT_FALSE(cache_->IsPassthroughSupported(kSbMediaAudioCodecEac3));
  EXPECT_FALSE(cache_->GetAudioConfiguration(2, &config));
  EXPECT_FALSE(cache_->HasAudioDecoderFor("audio/mp4", 192000));
  // NEW: Verify Audio/Video Maps were cleared
  // The cache will see it's empty, ask the Provider again (Times(2) above),
  // get empty maps (WillOnce(Return()) above), and result in FALSE.
  EXPECT_FALSE(cache_->HasAudioDecoderFor(audio_mime, 128000));

  // This fails because the provider returned empty maps on the 2nd call,
  // so the mime type "video/test" no longer exists in the cache.
  EXPECT_FALSE(cache_->HasVideoDecoderFor(video_mime, true, true, true, 1920,
                                          1080, 5000, 30));
}
}  // namespace
}  // namespace starboard
