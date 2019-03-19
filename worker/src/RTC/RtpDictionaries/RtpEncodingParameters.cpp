#define MS_CLASS "RTC::RtpEncodingParameters"
// #define MS_LOG_DEV

#include "Logger.hpp"
#include "MediaSoupErrors.hpp"
#include "RTC/RtpDictionaries.hpp"

namespace RTC
{
	/* Instance methods. */

	RtpEncodingParameters::RtpEncodingParameters(json& data)
	{
		MS_TRACE();

		if (!data.is_object())
			MS_THROW_TYPE_ERROR("data is not an object");

		auto jsonSsrcIt             = data.find("ssrc");
		auto jsonRidIt              = data.find("rid");
		auto jsonCodecPayloadTypeIt = data.find("codecPayloadType");
		auto jsonRtxIt              = data.find("rtx");
		auto jsonMaxBitrateIt       = data.find("maxBitrate");
		auto jsonMaxFramerateIt     = data.find("maxFramerate");

		// ssrc is optional.
		if (jsonSsrcIt != data.end() && jsonSsrcIt->is_number_unsigned())
			this->ssrc = jsonSsrcIt->get<uint32_t>();

		// rid is optional.
		if (jsonRidIt != data.end() && jsonRidIt->is_string())
			this->rid = jsonRidIt->get<std::string>();

		// codecPayloadType is optional.
		if (jsonCodecPayloadTypeIt != data.end() && jsonCodecPayloadTypeIt->is_number_unsigned())
		{
			this->codecPayloadType    = jsonCodecPayloadTypeIt->get<uint8_t>();
			this->hasCodecPayloadType = true;
		}

		// rtx is optional.
		// This may throw.
		if (jsonRtxIt != data.end() && jsonRtxIt->is_object())
		{
			this->rtx    = RtpRtxParameters(*jsonRtxIt);
			this->hasRtx = true;
		}

		// maxBitrate is optional.
		if (jsonMaxBitrateIt != data.end() && jsonMaxBitrateIt->is_number_unsigned())
			this->maxBitrate = jsonMaxBitrateIt->get<uint32_t>();

		// maxFramerate is optional.
		if (jsonMaxFramerateIt != data.end() && jsonMaxFramerateIt->is_number())
			this->maxFramerate = jsonMaxFramerateIt->get<double>();
	}

	void RtpEncodingParameters::FillJson(json& jsonObject) const
	{
		MS_TRACE();

		// Force it to be an object even if no key/values are added below.
		jsonObject = json::object();

		// Add ssrc.
		if (this->ssrc != 0u)
			jsonObject["ssrc"] = this->ssrc;

		// Add rid.
		if (!this->rid.empty())
			jsonObject["rid"] = this->rid;

		// Add codecPayloadType.
		if (this->hasCodecPayloadType)
			jsonObject["codecPayloadType"] = this->codecPayloadType;

		// Add rtx.
		if (this->hasRtx)
			this->rtx.FillJson(jsonObject["rtx"]);

		// Add maxBitrate.
		if (this->maxBitrate != 0u)
			jsonObject["maxBitrate"] = this->maxBitrate;

		// Add maxFramerate.
		if (this->maxFramerate > 0)
			jsonObject["maxFramerate"] = this->maxFramerate;
	}
} // namespace RTC
