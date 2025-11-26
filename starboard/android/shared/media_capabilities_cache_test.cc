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

using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgPointee;

// --- Constants to avoid Magic Strings ---
constexpr char kAudioMime[] = "audio/test_codec";
constexpr char kVideoMime[] = "video/test_codec";
constexpr int kDefaultBitrate = 200000;

typedef std::vector<std::unique_ptr<AudioCodecCapability>>
    AudioCodecCapabilities;
typedef std::vector<std::unique_ptr<VideoCodecCapability>>
    VideoCodecCapabilities;

// --- Mock Classes ---
class MockAudioCodecCapability : public AudioCodecCapability {
 public:
  MockAudioCodecCapability(const std::string& name,
                           const Range& supported_bitrates)
      : AudioCodecCapability(name, supported_bitrates) {}
};

class MockVideoCodecCapability : public VideoCodecCapability {
 public:
  // Helper to allow simple construction in tests
  MockVideoCodecCapability(const std::string& name)
      : VideoCodecCapability(name,
                             true,            /* is_secure_required */
                             true,            /* is_secure_supported */
                             true,            /* is_tunnel_mode_required */
                             true,            /* is_tunnel_mode_supported */
                             false,           /* is_software_decoder */
                             true,            /* is_hdr_capable */
                             Range(0, 3840),  /* width */
                             Range(0, 2160),  /* height */
                             Range(0, 10000), /* bitrate */
                             Range(0, 60)) /* fps */ {}

  // Full constructor for granular control if needed
  MockVideoCodecCapability(const std::string& name,
                           bool secure_req,
                           bool secure_sup,
                           bool tunnel_req,
                           bool tunnel_sup,
                           bool software,
                           bool hdr,
                           const Range& w,
                           const Range& h,
                           const Range& br,
                           const Range& fps)
      : VideoCodecCapability(name,
                             secure_req,
                             secure_sup,
                             tunnel_req,
                             tunnel_sup,
                             software,
                             hdr,
                             w,
                             h,
                             br,
                             fps) {}

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
              (const std::string&, bool, bool, bool, bool, int, int, int, int),
              (override));
  MOCK_METHOD(void,
              GetCodecCapabilities,
              ((std::map<std::string, AudioCodecCapabilities>)&,
               (std::map<std::string, VideoCodecCapabilities>)&),
              (override));
};

// --- Test Fixture ---
class MediaCapabilitiesCacheTest : public ::testing::Test {
 protected:
  void SetUp() override {
    auto provider = std::make_unique<MockMediaCapabilitiesProvider>();
    mock_media_capabilities_provider_ = provider.get();
    cache_ = MediaCapabilitiesCache::CreateForTest(std::move(provider));
    cache_->SetCacheEnabled(true);
  }

  // Helper: Factory for Video Mock to reduce boolean soup in tests
  std::unique_ptr<MockVideoCodecCapability> CreateDefaultVideoMock(
      const std::string& name) {
    return std::make_unique<MockVideoCodecCapability>(name);
  }

  // Helper: Factory for Audio Mock
  std::unique_ptr<MockAudioCodecCapability> CreateDefaultAudioMock(
      const std::string& name) {
    return std::make_unique<MockAudioCodecCapability>(
        name, Range(0, kDefaultBitrate));
  }

  // Helper: Sets up the Provider to return "True" or "Supported" for all simple
  // queries. This declutters the ClearCache test significantly.
  void ExpectProviderSuccess(int times) {
    EXPECT_CALL(*mock_media_capabilities_provider_, GetIsWidevineSupported())
        .Times(times)
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*mock_media_capabilities_provider_, GetIsCbcsSchemeSupported())
        .Times(times)
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*mock_media_capabilities_provider_, GetSupportedHdrTypes())
        .Times(times)
        .WillRepeatedly(
            Return(std::set<SbMediaTransferId>{kSbMediaTransferIdSmpteSt2084}));
    EXPECT_CALL(*mock_media_capabilities_provider_,
                GetIsPassthroughSupported(_))
        .Times(times)
        .WillRepeatedly(Return(true));

    // Setup a dummy config for GetAudioConfiguration
    static SbMediaAudioConfiguration kDummyConfig = {
        kSbMediaAudioConnectorHdmi, 0, kSbMediaAudioCodingTypeAc3, 2};
    EXPECT_CALL(*mock_media_capabilities_provider_, GetAudioConfiguration(0, _))
        .Times(times)
        .WillRepeatedly(DoAll(SetArgPointee<1>(kDummyConfig), Return(true)));

    // 2. Index 1: Return false (The loop ends)
    // You need this expectation because the cache will always check the next
    // index
    EXPECT_CALL(*mock_media_capabilities_provider_, GetAudioConfiguration(1, _))
        .Times(times)
        .WillRepeatedly(Return(false));
  }

  MockMediaCapabilitiesProvider* mock_media_capabilities_provider_;
  std::unique_ptr<MediaCapabilitiesCache> cache_;
};

// --- Tests ---

TEST_F(MediaCapabilitiesCacheTest, IsWidevineSupported_EnabledCache) {
  EXPECT_CALL(*mock_media_capabilities_provider_, GetIsWidevineSupported())
      .WillOnce(Return(true));
  EXPECT_TRUE(cache_->IsWidevineSupported());
  EXPECT_TRUE(cache_->IsWidevineSupported());  // Cached
}

TEST_F(MediaCapabilitiesCacheTest, IsWidevineSupported_DisabledCache) {
  cache_->SetCacheEnabled(false);
  EXPECT_CALL(*mock_media_capabilities_provider_, GetIsWidevineSupported())
      .Times(2)
      .WillOnce(Return(true))
      .WillOnce(Return(false));
  EXPECT_TRUE(cache_->IsWidevineSupported());
  EXPECT_FALSE(cache_->IsWidevineSupported());  // Not Cached
}

TEST_F(MediaCapabilitiesCacheTest, IsCbcsSchemeSupported_EnabledCache) {
  EXPECT_CALL(*mock_media_capabilities_provider_, GetIsCbcsSchemeSupported())
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
  std::map<std::string, VideoCodecCapabilities> video_map;

  auto mock_video = CreateDefaultVideoMock(kVideoMime);
  MockVideoCodecCapability* raw_video_ptr =
      mock_video.get();  // Observer pointer
  video_map[kVideoMime].push_back(std::move(mock_video));

  // 1. Setup Provider to return the map
  EXPECT_CALL(*mock_media_capabilities_provider_, GetCodecCapabilities(_, _))
      .WillOnce(
          [&](auto&, auto& video_out) { video_out = std::move(video_map); });

  // 2. Setup Expectation on the item *inside* the map
  EXPECT_CALL(*raw_video_ptr, AreResolutionAndRateSupported(1920, 1080, 30))
      .WillOnce(Return(true));

  // 3. Execute
  EXPECT_TRUE(cache_->HasVideoDecoderFor(kVideoMime, true, true, true, 1920,
                                         1080, 0, 30));

  // 4. Verify negative case (mime not in map)
  EXPECT_FALSE(cache_->HasVideoDecoderFor("unknown_mime", true, true, true,
                                          1920, 1080, 0, 30));
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
  // --- 1. Setup Data ---
  std::map<std::string, AudioCodecCapabilities> audio_map;
  audio_map[kAudioMime].push_back(CreateDefaultAudioMock(kAudioMime));

  std::map<std::string, VideoCodecCapabilities> video_map;
  auto mock_video = CreateDefaultVideoMock(kVideoMime);
  MockVideoCodecCapability* raw_video_ptr = mock_video.get();  // Observer
  video_map[kVideoMime].push_back(std::move(mock_video));

  // --- 2. Setup Expectations (Pre-Clear) ---

  // Setup standard provider calls (Widevine, HDR, etc) to always return success
  // We expect them called twice: Once before clear, once after clear.
  ExpectProviderSuccess(2);

  // Setup the complex Codec Map call
  // Call 1: Return the maps we built above.
  // Call 2: Return empty maps (simulating empty provider).
  EXPECT_CALL(*mock_media_capabilities_provider_, GetCodecCapabilities(_, _))
      .Times(2)
      .WillOnce([&](auto& audio_out, auto& video_out) {
        audio_out = std::move(audio_map);
        video_out = std::move(video_map);
      })
      .WillOnce(Return());

  // Expect the specific video capability to be checked exactly once (Pre-Clear)
  EXPECT_CALL(*raw_video_ptr, AreResolutionAndRateSupported(_, _, _))
      .WillOnce(Return(true));

  // --- 3. Verify Pre-Clear (Populated) State ---
  SbMediaAudioConfiguration config;
  EXPECT_TRUE(cache_->IsWidevineSupported());
  EXPECT_TRUE(cache_->IsCbcsSchemeSupported());
  EXPECT_TRUE(cache_->IsHDRTransferCharacteristicsSupported(
      kSbMediaTransferIdSmpteSt2084));
  EXPECT_TRUE(cache_->IsPassthroughSupported(kSbMediaAudioCodecAc3));
  EXPECT_TRUE(cache_->GetAudioConfiguration(0, &config));

  // Verify Maps populated
  EXPECT_TRUE(cache_->HasAudioDecoderFor(kAudioMime, 100));
  EXPECT_TRUE(cache_->HasVideoDecoderFor(kVideoMime, true, true, true, 1920,
                                         1080, 0, 60));

  // --- 4. ACTION: Clear Cache ---
  cache_->ClearCache();

  // --- 5. Verify Post-Clear (Empty/Refresh) State ---

  // These calls trigger the Provider again (Satisfying the Times(2)
  // expectation)
  EXPECT_TRUE(cache_->IsWidevineSupported());
  EXPECT_TRUE(cache_->IsCbcsSchemeSupported());
  EXPECT_TRUE(cache_->IsHDRTransferCharacteristicsSupported(
      kSbMediaTransferIdSmpteSt2084));
  EXPECT_TRUE(cache_->IsPassthroughSupported(kSbMediaAudioCodecAc3));

  EXPECT_TRUE(cache_->GetAudioConfiguration(0, &config));

  // These calls trigger Provider again, but Provider now returns Empty Maps.
  // Therefore, the cache should return FALSE.
  EXPECT_FALSE(cache_->HasAudioDecoderFor(kAudioMime, 100));
  EXPECT_FALSE(cache_->HasVideoDecoderFor(kVideoMime, true, true, true, 1920,
                                          1080, 0, 60));
}
}  // namespace
}  // namespace starboard
