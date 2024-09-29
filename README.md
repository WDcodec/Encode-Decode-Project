# Encoder Decoder System
The wd_codec library is an Encoder-Decoder System for protecting and recovering data using Reed-Solomon error correction.

## Installation

Open the cmd in your local folder for install the code 

```bash
git clone https://github.com/WDcodec/Encode-Decode-Project.git
```

## Usage
The parameters of both the Reed-Solomon algorithm and the finite field definition can be manually adjusted in setup_utilities.h, where they are currently set to the CCSDS standard by default.

so, Each usage must have these declarations:
```main
#include "setup_utilities.h"
int main()
{   
    wd_codec::setup();

    const wd_codec::Encoder encoder(wd_codec::field, wd_codec::generator_polynomial);
    const wd_codec::Decoder decoder(wd_codec::field, wd_codec::generator_polynomial_index);

    wd_codec::file_encoder_t file_encoder(encoder);
    wd_codec::file_decoder_t file_decoder(decoder);

```
Chosing type of input:

### Primitive Input
```
std::string message ;
std::cin >> message ;
message.resize(wd_codec::code_length, 0x00);
if (!encoder.encode(wd_codec::block, message)){
     return 1;
   }
if (!decoder.decode(wd_codec::block)) {
     return 1;
   }
```
### File
```
const std::string input_file_name = "input.dat";//your file
const std::string rsencoded_output_file_name = "output.rsenc";
const std::string rsdecoded_file_name = "output.rsdec";
       
if (!file_encoder.encode(input_file_name, rsencoded_output_file_name)) {
            return 1;
}

//inject random errors      
wd_codec::error_injection::inject_random_errors<wd_codec::code_length, wd_codec::fec_length>(rsencoded_output_file_name);
       
if (!file_decoder.decode(rsencoded_output_file_name, rsdecoded_file_name)) {
            wd_codec::Logger::log(wd_codec::INFO, "File Decoder: Decoder failed " );
            wd_codec::close();
            return 1;
 }

//number of blocks
std::cout << "num blocks " << file_decoder.getCurrentBlockIndex();

```

### Photo: BMP Format
[convert photo from other format:](https://convertfree.com/he/converter/audio/opus)
```
const std::string input_file_name = "picture.bmp";//your photo path
const std::string rsencoded_output_file_name = "output.rsenc";
const std::string rsdecoded_file_name = "output.rsdec";
const std::string binaryFilePath = "binary_image_data.bin";

wd_codec::fileio::convert_image_to_binary(input_file_name, binaryFilePath);

if (!file_encoder.encode_image(input_file_name, rsencoded_output_file_name)) {
    std::cout << "Encoding failed." << std::endl;
    wd_codec::close();
    return 1;
}
//inject random errors  
wd_codec::error_injection::inject_random_errors_for_image<wd_codec::code_length,wd_codec::fec_length>(rsencoded_output_file_name, "binary_image_encoded.bmp");

if (!file_decoder.decode_image(rsencoded_output_file_name, rsdecoded_file_name)) {
    return 1;
}
```
### Audio: OPUS Format

[convert audio from other format :](https://he.onlineconvert.com/mp4-to-opus)

```
const std::string input_file_name = "audio.opus";//your audio path
const std::string rsencoded_output_file_name = "output.rsenc";
const std::string rsdecoded_file_name = "output.rsdec";
const std::string binaryFilePath = "binary_audio_data.bin";

wd_codec::fileio::converAudioToBinary(input_file_name, binaryFilePath);

if (!file_encoder.encode_audio(input_file_name, rsencoded_output_file_name)) {
    return 1;
}
//inject random errors  
wd_codec::error_injection::inject_random_errors_for_audio<wd_codec::code_length, wd_codec::fec_length>(rsencoded_output_file_name, "binary_audio_encoded.opus");

if (!file_decoder.decode_audio(rsencoded_output_file_name, rsdecoded_file_name)) {
    return 1;
}
```

### Additional : Mixing solution for Burst Errors:

```
const std::string input_file_name = "input.txt";//your file path
const std::string rsencoded_output_file_name = "output.rsenc";
const std::string mixing_output_file_name = "mix.rsenc";
const std::string remixing_output_file_name = "remix.rsenc";
const std::string rsdecoded_file_name = "output.rsdec";
// Call the encode function
if (!file_encoder.encode(input_file_name, rsencoded_output_file_name)) {
            return 1;
}
// add a mixing layer between encode - decode for burst errors
wd_codec::transpose_mix<wd_codec::code_length, wd_codec::fec_length, wd_codec::data_length>(rsencoded_output_file_name, mixing_output_file_name, file_encoder.get_blocks_number(), wd_codec::fileio::file_size(rsencoded_output_file_name));

long burst_start_position = 1000;  // Example start position
long burst_length = 100;  // Example burst length
wd_codec::error_injection::corrupt_file_with_burst_errors(mixing_output_file_name, burst_start_position, burst_length);
wd_codec::transpose_remix2<wd_codec::code_length, wd_codec::fec_length, wd_codec::data_length>(mixing_output_file_name, remixing_output_file_name, file_encoder.get_blocks_number(), wd_codec::fileio::file_size(mixing_output_file_name));

if (!file_decoder.decode(remixing_output_file_name, rsdecoded_file_name)) 
            return 1;
}
```

### Polynomial Support
support of Polynomial data structure 

An example of multiplying 2 polynomials:
```
include "setup_utilities.h"
// P(x) = x^2 + x + 1
wd_codec::galois::Field_Element P_elements[] = {
    wd_codec::galois::Field_Element(wd_codec::field, 1),
    wd_codec::galois::Field_Element(wd_codec::field, 1),
    wd_codec::galois::Field_Element(wd_codec::field, 1)
};

// Q(x) = 2x + 2
wd_codec::galois::Field_Element Q_elements[] = {
    wd_codec::galois::Field_Element(wd_codec::field, 2),
    wd_codec::galois::Field_Element(wd_codec::field, 2)
};

wd_codec::galois::Polynomial P(wd_codec::field, 2, P_elements);
wd_codec::galois::Polynomial Q(wd_codec::field, 1, Q_elements);

//P(X) = 2 x^3 + 2
P *= Q;
```

## Test

Set properties in Test project:

![Set properties](https://github.com/user-attachments/assets/9b36fba8-8043-4d92-8a23-c7e918dd3fd8)

Chose the Test mode for run

![mode for run](https://github.com/user-attachments/assets/33138b8a-457e-4017-a7a6-942f97e3913d)

Unmarked the lines in Logger.h:

![Unmarked the lines](https://github.com/user-attachments/assets/4d3fe26a-0936-4db0-8f73-aa79227302f5)

Run 

you should see: 9/9 tests passed 

you can write more tests acroding your goals

##
#### Run Time: O(n^2)
#### Space comlexity:O(n)


 #### You are welcome to read our [design document](https://docs.google.com/document/d/15HjDrKq-bXMuxdPeO_hRMhoan5Ts4Vxn/edit?usp=sharing&ouid=103102751553159913006&rtpof=true&sd=true) and see our [presentation](https://docs.google.com/presentation/d/1ogAyA55F_DTyFQMUE5UGRZJBpf7j_wuBfhxTugHZ9KU/edit?usp=sharing)

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

We look forward to your feedback to help us improve this project.

### Team Members:

- [Efrat Eisenbach](https://github.com/efratizen)
- [Ayala Elhadad](https://github.com/AyalaElha)
- [Tamar Godinger](https://github.com/tamargodin)

