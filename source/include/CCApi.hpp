#ifndef CC_CLI_CCAPI_HPP
#define CC_CLI_CCAPI_HPP

#include <stdexcept>
#include <vector>
#include <ctime>

namespace CCApi {
    constexpr const char* CC_URL = "https://czech-craft.eu/api/{}";

    struct Response {
        const int code;
        const std::string message;
    };

    struct ServerInfo {
        const std::string address;
        const std::string name;
        const int position;
        const std::string slug;
        const int votes;
    };

    struct VoterInfo  {
        const std::string username;
        const int vote_count;
    };

    struct Vote {
        const std::string username;
        const std::time_t date;
        const bool delivered;
    };

    struct VoteVector {
        std::vector<Vote> votes;
        const int vote_count;
    };

    struct PlayerInfo {
        const std::string username;
        const std::time_t next_vote;
        const int vote_count;
        const std::vector<Vote> votes;
    };

    /**
     * Retrieves server information.
     *
     * @param slug Slug name of the server
     *
     * @throws std::runtime_error Thrown in case something goes wrong, with detailed message about the error
     * @returns Server information
     */
    ServerInfo serverInfo(const std::string &slug);

    /**
    * Retrieves all server votes. (Literally all of them)
    *
    * @param slug Slug name of the server
     *
    * @throws std::runtime_error Thrown in case something goes wrong, with detailed message about the error
    * @returns Server votes (A lot of them)
    */
    VoteVector serverVotes(const std::string &slug);

    /**
    * Retrieves all server votes from a specified month and a year.
    *
    * @param slug Slug name of the server
    * @param month
    * @param year
     *
    * @throws std::runtime_error Thrown in case something goes wrong, with detailed message about the error
    * @returns Server votes from a month
    */
    VoteVector serverVotes(const std::string &slug, const int &month, const int &year);

    /**
    * Retrieves server's top voters.
    *
    * @param slug Slug name of the server
    * @param month
    * @param year
     *
    * @throws std::runtime_error Thrown in case something goes wrong, with detailed message about the error
    * @returns Top voters profiles
    */
    std::vector<VoterInfo> topVoters(const std::string &slug);

    /**
    * Retrieves all user votes.
     *
    * @param username Username of the player
    * @param slug Slug name of the server
     *
    * @throws std::runtime_error Thrown in case something goes wrong, with detailed message about the error
    * @returns Server votes from a month
    */
    PlayerInfo userVotes(const std::string &username, const std::string &slug);

    /**
    * Retrieves all user votes from a specified month and a year.
     *
    * @param username Username of the player
    * @param slug Slug name of the server
    * @param month
    * @param year
     *
    * @throws std::runtime_error Thrown in case something goes wrong, with detailed message about the error
    * @returns Server votes from a month
    */
    PlayerInfo userVotes(const std::string &username, const std::string &slug, const int &month, const int &year);

}
#endif //CC_CLI_CCAPI_HPP
