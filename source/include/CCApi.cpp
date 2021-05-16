#include "CCApi.hpp"

#include <fmt/format.h>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using namespace CCApi;

size_t writer(void *ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
}

[[nodiscard]]
CURL*
common_curl_init(const std::string &context, std::string &response) {
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
common_curl_preform(CURL* handle) {
    auto curl_code = curl_easy_perform(handle);
    if(curl_code != CURLE_OK)
        throw std::runtime_error(fmt::format("CURL request failed with error {}", curl_code));

    long code;
    curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &code);
    if(code != 200)
        throw std::runtime_error(fmt::format("HTTP request failed with code {}", code));
}

ServerInfo
CCApi::serverInfo(const std::string &slug) {
    std::string response;
    auto handle = common_curl_init(fmt::format("server/{}", slug), response);
    try {
        common_curl_preform(handle);

        auto json_object = nlohmann::json::parse(response);
        ServerInfo info {
            .address = json_object["address"],
            .name = json_object["name"],
            .position = json_object["position"].get<int>(),
            .slug = json_object["slug"],
            .votes = json_object["votes"].get<int>()
        };
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
        std::vector<Vote> votes;

        auto obj = nlohmann::json::parse(response);
        auto data = obj["data"];
        for (const auto &datum : data) {
            votes.emplace_back(Vote {
                .username = datum["username"],
                .date = 0, //TODO parse datetime
                .delivered = datum["delivered"].get<bool>(),
            });
        }

        VoteVector vector {
            .votes = votes,
            .vote_count = obj["vote_count"].get<int>()
        };

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
        std::vector<Vote> votes;

        auto obj = nlohmann::json::parse(response);
        auto data = obj["data"];
        for (const auto &datum : data) {
            votes.emplace_back(Vote {
                    .username = datum["username"],
                    .date = 0, //TODO parse datetime
                    .delivered = datum["delivered"].get<bool>(),
            });
        }

        VoteVector vector {
                .votes = votes,
                .vote_count = obj["vote_count"].get<int>()
        };

        return vector;
    } catch (...) {
        curl_easy_cleanup(handle);
        throw;
    }
}

std::vector<VoterInfo>
CCApi::topVoters(const std::string &slug) {
    std::string response;
    auto handle = common_curl_init(fmt::format("server/{}/voters", slug), response);;
    try {
        common_curl_preform(handle);
        std::vector<VoterInfo> votes;

        auto data = nlohmann::json::parse(response)["data"];
        for (const auto &datum : data) {
            votes.emplace_back(VoterInfo {
                    .username = datum["username"],
                    .vote_count = datum["votes"].get<int>()
            });
        }

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
        std::vector<Vote> votes;


        auto obj = nlohmann::json::parse(response);
        auto data = obj["data"];

        for (const auto &datum : data) {
            votes.emplace_back(Vote {
                    .username = datum["username"],
                    .date = 0, //TODO parse datetime
                    .delivered = datum["delivered"].get<bool>()
            });
        }

        PlayerInfo info {
            .username = obj["username"],
            .next_vote = 0, //TODO parse datetime
            .vote_count = obj["vote_count"].get<int>(),
            .votes = votes
        };

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
        std::vector<Vote> votes;

        auto obj = nlohmann::json::parse(response);
        auto data = obj["data"];

        for (const auto &datum : data) {
            votes.emplace_back(Vote {
                    .username = datum["username"],
                    .date = 0, //TODO parse datetime
                    .delivered = datum["delivered"].get<bool>()
            });
        }
        PlayerInfo info {
                .username = username,
                .vote_count = obj["vote_count"].get<int>(),
                .votes = votes
        };

        return info;
    } catch (...) {
        curl_easy_cleanup(handle);
        throw;
    }
}
