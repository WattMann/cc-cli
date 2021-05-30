#include "CCApi.hpp"

#include <ctime>
#include <iomanip>
#include <sstream>

using namespace CCApi;

extern "C" char* strptime(const char* s,
                          const char* f,
                          struct tm* tm) {
    std::istringstream input(s);
    input.imbue(std::locale(setlocale(LC_ALL, nullptr)));
    input >> std::get_time(tm, f);
    if (input.fail()) {
        return nullptr;
    }
    return (char*)(s + input.tellg());
}

void parse_time(char* str, const char* format, std::tm& tm){
    strptime(str, format, &tm);
}

size_t writer(void *ptr, size_t size, size_t nmemb, std::string *data){
    data->append((char *) ptr, size * nmemb);
    return size * nmemb;
}

[[nodiscard]]
CURL *
common_curl_init(const std::string &context, std::string &response) {
	 curl_global_init(CURL_GLOBAL_WIN32);
    auto handle = curl_easy_init();
    if (!handle)
        throw std::runtime_error("Failed to initialize CURL handle");

    curl_easy_setopt(handle, CURLOPT_URL, fmt::format(CC_URL, context).c_str());
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, writer);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, true);

    return handle;
}

void
common_curl_preform(CURL *handle) {
    auto curl_code = curl_easy_perform(handle);
    if (curl_code != CURLE_OK)
        throw std::runtime_error(fmt::format("CURL request failed with error {}", curl_code));

    long code;
    curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &code);
    if (code != 200)
        throw std::runtime_error(fmt::format("HTTP request failed with code {}", code));
}

ServerInfo
CCApi::serverInfo(const std::string &slug) {
    std::string response;
    auto handle = common_curl_init(fmt::format("server/{}", slug), response);
  	printf("%ud\n", handle);
    try {
        common_curl_preform(handle);
        auto json_object = nlohmann::json::parse(response);
        
        ServerInfo info{
                json_object["address"],
                json_object["name"],
                json_object["position"].get<int>(),
                json_object["slug"],
                json_object["votes"].get<int>()
        };
        printf("obj done\n");
        curl_easy_cleanup(handle);
        return info;
    } catch (...) {
        curl_easy_cleanup(handle);
        throw;
    }
}

VoteVector
CCApi::serverVotes(const std::string &slug) {
    std::string response;
    auto handle = common_curl_init(fmt::format("server/{}/votes", slug), response);;
    try {
        common_curl_preform(handle);
        std::list<Vote> votes;

        auto obj = nlohmann::json::parse(response);
        auto data = obj["data"];
        for (const auto &datum : data) {
            std::tm tm {};
            parse_time((char*)datum["datetime"].get<std::string>().c_str(), TIME_FORMAT, tm);

            votes.emplace_back(Vote{datum["username"], tm, datum["delivered"].get<bool>()});
        }

        VoteVector vector {
                votes,
                obj["vote_count"].get<int>()
        };

        curl_easy_cleanup(handle);
        return vector;
    } catch (...) {
        curl_easy_cleanup(handle);
        throw;
    }
}

VoteVector
CCApi::serverVotes(const std::string &slug, const int &month, const int &year) {
    std::string response;
    auto handle = common_curl_init(fmt::format("server/{}/votes/{}/{}", slug, year, month), response);;
    try {
        common_curl_preform(handle);
        std::list<Vote> votes;

        auto obj = nlohmann::json::parse(response);
        auto data = obj["data"];
        for (const auto &datum : data) {
            std::tm tm{};
            parse_time((char*)datum["datetime"].get<std::string>().c_str(), TIME_FORMAT, tm);

            votes.push_front(Vote{
                    datum["username"],
                    tm,
                    datum["delivered"].get<bool>(),
            });
        }

        VoteVector vector{
                votes,
                obj["vote_count"].get<int>()
        };

        curl_easy_cleanup(handle);
        return vector;
    } catch (...) {
        curl_easy_cleanup(handle);
        throw;
    }
}

std::list<VoterInfo>
CCApi::topVoters(const std::string &slug) {
    std::string response;
    auto handle = common_curl_init(fmt::format("server/{}/voters", slug), response);;
    try {
        common_curl_preform(handle);
        std::list<VoterInfo> votes;

        auto data = nlohmann::json::parse(response)["data"];
        for (const auto &datum : data) {
            votes.push_back(VoterInfo{
                    datum["username"],
                    datum["votes"].get<int>()
            });
        }
        curl_easy_cleanup(handle);
        return votes;
    } catch (...) {
        curl_easy_cleanup(handle);
        throw;
    }
}

PlayerInfo
CCApi::userVotes(const std::string &username, const std::string &slug) {
    std::string response;
    auto handle = common_curl_init(fmt::format("server/{}/player/{}", slug, username), response);;
    try {
        common_curl_preform(handle);
        std::list<Vote> votes;

        auto obj = nlohmann::json::parse(response);
        auto data = obj["data"];

        for (const auto &datum : data) {
            std::tm tm{};
            parse_time((char*) datum["datetime"].get<std::string>().c_str(), TIME_FORMAT, tm);
            votes.push_front(Vote{
                    datum["username"],
                    tm,
                    datum["delivered"].get<bool>()
            });
        }

        std::tm tm {};
        parse_time((char*)obj["next_vote"].get<std::string>().c_str(), TIME_FORMAT, tm);
        PlayerInfo info {
                obj["username"],
                tm,
                obj["vote_count"].get<int>(),
                votes
        };
        
        curl_easy_cleanup(handle);
        return info;
    } catch (...) {
        curl_easy_cleanup(handle);
        throw;
    }
}

PlayerInfo
CCApi::userVotes(const std::string &username, const std::string &slug, const int &month, const int &year) {
    std::string response;
    auto handle = common_curl_init(fmt::format("server/{}/player/{}/{}/{}", slug, username, year, month), response);
    try {
        common_curl_preform(handle);
        std::list<Vote> votes;

        auto obj = nlohmann::json::parse(response);
        auto data = obj["data"];

        for (const auto &datum : data) {
            std::tm tm{};
            parse_time((char*)datum["datetime"].get<std::string>().c_str(), TIME_FORMAT, tm);
            votes.push_front(Vote{
                    datum["username"],
                    tm,
                    datum["delivered"].get<bool>()
            });
        }

        PlayerInfo info {
                username,
                std::tm{},
                obj["vote_count"].get<int>(),
                votes
        };

        curl_easy_cleanup(handle);
        return info;
    } catch (...) {
        curl_easy_cleanup(handle);
        throw;
    }
}

PlayerInfo
CCApi::nextVote(const std::string &username, const std::string &slug) {
    std::string response;
    auto handle = common_curl_init(fmt::format("server/{}/player/{}/next_vote", slug, username), response);
    try {
        common_curl_preform(handle);

        auto obj = nlohmann::json::parse(response);
        std::tm tm {};
        parse_time((char*)obj["next_vote"].get<std::string>().c_str(), TIME_FORMAT, tm);

        PlayerInfo info {
                obj["username"],
                tm
        };
        curl_easy_cleanup(handle);
        return info;
    } catch (...) {
        curl_easy_cleanup(handle);
        throw;
    }
}
