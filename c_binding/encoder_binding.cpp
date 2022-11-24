#include <string>
#include <iostream>
#include <chrono>
#include "glog/logging.h"  // IWYU pragma: keep
#include "iotypes.h"
#include "lyra_encoder.h"
#include "lyra_config.h"

using namespace chromemedia::codec;
using namespace std::chrono;


extern "C"  //Tells the compile to use C-linkage for the next scope.
{
    array_uint8_t* EncodeWav(
        const array_int16_t *wav_array,
        int num_channels,
        int sample_rate_hz, 
        int bitrate,
        bool enable_dtx, 
        const char* model_path_string
    ) {
        
    
        uint64_t start = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

        const ghc::filesystem::path model_path(model_path_string);
        std::vector<int16_t> input_data = to_vector_int16_t(wav_array);
        std::vector<uint8_t> encoded_features;

        // LOG(INFO) << "Input Length: " << input_data.size();

        std::unique_ptr<LyraEncoder> encoder = LyraEncoder::Create(
            /*sample_rate_hz=*/sample_rate_hz,
            /*num_channels=*/num_channels,
            /*bitrate=*/bitrate,
            /*enable_dtx=*/enable_dtx,
            /*model_path=*/model_path);

        
        const auto benchmark_start = absl::Now();



        const int num_samples_per_packet = sample_rate_hz / encoder->frame_rate();
        // Iterate over the wav data until the end of the vector.
        for (int wav_iterator = 0;
            wav_iterator + num_samples_per_packet <= input_data.size();
            wav_iterator += num_samples_per_packet) {
            // Move audio samples from the large in memory wav file frame by frame to
            // the encoder.
            auto encoded = encoder->Encode(absl::MakeConstSpan(
                &input_data.at(wav_iterator), num_samples_per_packet));

            if (!encoded.has_value()) {
                LOG(ERROR) << "Unable to encode features starting at samples at byte "
                            << wav_iterator << ".";
                return 0;
            }
            
            encoded_features.insert(encoded_features.end(), encoded.value().begin(),
                                    encoded.value().end());

        }
        const auto elapsed = absl::Now() - benchmark_start;
        LOG(INFO) << "Elapsed seconds : " << absl::ToDoubleSeconds(elapsed);

        return from_vector_uint8_t(encoded_features);
    }

} //End C linkage scope.