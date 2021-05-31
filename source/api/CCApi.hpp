#pragma once

#include <cstdio>
#include <chrono>
#include <stdexcept>
#include <list>
#include <ctime>


#include <nlohmann/json.hpp>
#include <fmt/format.h>
#include <curl/curl.h>

namespace CCApi {

    constexpr const char *CC_URL = "https://czech-craft.eu/api/{}";
    constexpr const char *TIME_FORMAT = "%Y-%m-%d %H:%M:%S";

    struct ServerInfo {
        ServerInfo() : address("N/S"), name("N/S"), position(-1), slug("N/S"), votes(0) {}
        ServerInfo(const std::string &address, const std::string &name, const int position, const std::string &slug,
                   const int votes) : address(address), name(name), position(position), slug(slug), votes(votes) {}

        std::string address;
        std::string name;
        int position;
        std::string slug;
        int votes;
    };

    struct VoterInfo {
        VoterInfo() : username("N/S"), vote_count(0) {}
        VoterInfo(const std::string &username, const int voteCount) : username(username), vote_count(voteCount) {}

        std::string username;
        int vote_count;
    };

    struct Vote {
        Vote() : username("N/S"), date(), delivered(false) {}
        Vote(const std::string &username, const tm &date, const bool delivered) : username(username), date(date), delivered(delivered) {}

        std::string username;
        std::tm date;
        bool delivered;
    };

    struct VoteVector {
        VoteVector() : votes(), vote_count(0) {};
        VoteVector(const std::list<Vote> &votes, const int voteCount) : votes(votes), vote_count(voteCount) {}

        std::list<Vote> votes;
        int vote_count;
    };

    struct PlayerInfo {
        PlayerInfo() : username("N/S"), next_vote(), vote_count(0), votes() {}
        PlayerInfo(const std::string &username, const tm &nextVote, const int voteCount, const std::list<Vote> &votes)
        : username(username), next_vote(nextVote), vote_count(voteCount), votes(votes) {}

        std::string username;
        std::tm next_vote;
        int vote_count;
        std::list<Vote> votes;
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
    std::list<VoterInfo> topVoters(const std::string &slug);

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

    /**
    * Retrieves PlayerInfo containing only next vote and username information.
    *
    * @param username Username of the player
    * @param slug Slug name of the server
    *
    * @throws std::runtime_error Thrown in case something goes wrong, with detailed message about the error
    * @returns Server votes from a month
    */
    PlayerInfo nextVote(const std::string &username, const std::string &slug);
}
