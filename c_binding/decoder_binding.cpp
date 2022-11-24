#include <string>
#include <iostream>
#include "absl/types/span.h"
#include "glog/logging.h"  // IWYU pragma: keep
#include "iotypes.h"
#include "lyra_decoder.h"
#include "lyra_config.h"

using namespace chromemedia::codec;


extern "C"  //Tells the compile to use C-linkage for the next scope.
{
    array_int16_t* DecodeWav(
        const array_uint8_t* lyra_data,
        int num_channels,
        int sample_rate,
        int bitrate,
        bool enable_dtx,
        const char* model_path_string
    ) {

        const auto benchmark_start = absl::Now();

        const ghc::filesystem::path model_path(model_path_string);
        const int packet_size = BitrateToPacketSize(bitrate);
        const int num_samples_per_hop = GetNumSamplesPerHop(sample_rate);
        std::vector<uint8_t> input_data = to_vector_uint8_t(lyra_data);

        std::unique_ptr<LyraDecoder> decoder = LyraDecoder::Create(
            sample_rate, 
            num_channels,
            model_path
        );

        std::vector<int16_t> decoded_all;

        for (int idx = 0; idx < input_data.size(); idx += packet_size)
        {
            const absl::Span<const uint8_t> encoded_packet =
                absl::MakeConstSpan(input_data.data() + idx, packet_size);

            // ! Check here for possible errors in receiving data
            decoder->SetEncodedPacket(encoded_packet);

            std::optional<std::vector<int16_t>> decoded = decoder->DecodeSamples(num_samples_per_hop);
            decoded_all.insert(decoded_all.end(), decoded.value().begin(), decoded.value().end());
        }

        const auto elapsed = absl::Now() - benchmark_start;
        LOG(INFO) << "Elapsed seconds : " << absl::ToDoubleSeconds(elapsed);

        return from_vector_int16_t(decoded_all);
    }

} //End C linkage scope.