#include<iostream>
#include<sstream>

#include<curlpp/cURLpp.hpp>
#include<curlpp/Easy.hpp>
#include<curlpp/Options.hpp>
#include<curlpp/Infos.hpp>

#include<nlohmann/json.hpp>

int main() {
    long response_code;
    std::stringstream stream;
    try {
        curlpp::Easy request;
        request.setOpt<curlpp::options::Url>("https://api.dictionaryapi.dev/api/v2/entries/en/hello");
        request.setOpt(new curlpp::options::WriteStream(&stream));
        request.perform();

        response_code = curlpp::infos::ResponseCode::get(request);
    }
    catch(curlpp::RuntimeError & e) {
        std::cerr << e.what() << std::endl;
    }
    catch(curlpp::LogicError & e) {
        std::cerr << e.what() << std::endl;
    }
    if (response_code != 200) {
        std::cerr << "failed to contact api with response " << response_code << std::endl;
        return 1;
    } else {
        nlohmann::json data = nlohmann::json::parse(stream);
        
        try {
            auto first_result = data[0];

            for (const auto& meaning : first_result["meanings"]) {
                std::cout << "word type: " << meaning["partOfSpeech"] << std::endl;
                for (const auto& definitions : meaning["definitions"]) {
                    std::string def = definitions["definition"];
                    std::cout << "definition: " << def << std::endl;
                }
            }
        }
        catch (nlohmann::json::exception & e) {
            std::cerr << "parse error " << e.what() << " raw dump of response " << stream.str() << std::endl;
        }
    }
    return 0;
}