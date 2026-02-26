#include<iostream>
#include<sstream>
#include<string>

#include<curlpp/cURLpp.hpp>
#include<curlpp/Easy.hpp>
#include<curlpp/Options.hpp>
#include<curlpp/Infos.hpp>

#include<nlohmann/json.hpp>

int full(char *word, int is_simple) {
    long response_code;
    std::stringstream stream;
    std::string request_url;
    try {
        curlpp::Easy request;
        request_url = "https://api.dictionaryapi.dev/api/v2/entries/en/" + std::string(word);
        request.setOpt<curlpp::options::Url>(request_url);
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
        std::cerr << "failed to contact api with response " << response_code << " requested url: " << request_url << std::endl;
        return 1;
    } else {
        nlohmann::json data = nlohmann::json::parse(stream);
        
        try {
            auto first_result = data[0];

            for (const auto& meaning : first_result["meanings"]) {
                if (is_simple == 0) {
                    std::cout << "word type: " << meaning["partOfSpeech"] << std::endl;
                }
                for (const auto& definitions : meaning["definitions"]) {
                    std::string def = definitions["definition"];
                    if (is_simple == 0) {
                        std::cout << "definition: " << def << std::endl;
                    } else {
                        std::cout << def << std::endl;
                        return 0;
                    }
                }
            }
        }
        catch (nlohmann::json::exception & e) {
            std::cerr << "parse error " << e.what() << " raw dump of response " << stream.str() << std::endl;
        }
    }
    return 0;
}

int simple(char *word) {
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "no arguments passed, see --help for more information" << std::endl;
        return 1;
    }
    std::string argument_one = argv[1];
    if (argument_one == "--help") {
        std::cout << "define - a terminal tool to look up definitions of words" << std::endl;
        std::cout << "--help - display help menu" << std::endl;
        std::cout << "--full - display full definition and word types of a word" << std::endl;
        std::cout << "if no other arguments are passed, it will display the definition in simple mode" << std::endl;
        std::cout << "usage: define [OPTION] [WORD]" << std::endl;
    } else if (argument_one == "--full") {
        full(argv[2], 0);
    } else {
        full(argv[1], 1);
    }
    return 0;
}