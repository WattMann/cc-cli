#include "api/CCApi.hpp"
#include "Version.hpp"
#include <fmt/format.h>
#include <chrono>

void displayHelp() {
    fmt::print( "CzechCraft CLI version {}.{}\n"
               "Usage: cc-cli <command> [arguments]\n"
               "Commands:\n"
               " info                       display server information\n"
               " votes                      display server or user votes\n"
               " top  | topvoters           display server top voters\n"
               " next | nextvote            display next vote date\n"
               "Arguments:\n"
               " -h, --help                 display overall help or command specific help\n"
               " -s, --slug [slug]          specify server slug\n"
               " -u, --username [username]  specify player username\n"
               " -y, --year [year]          specify year span\n"
               " -m, --month [year]         specify month span\n",
               CC_CLI_VERSION_MAJOR, CC_CLI_VERSION_MINOR
    );
}

//  1    2     3   4
// exec arg0 arg1 args2
int main(int argc, char **argv) {
    if(argc < 2) {
        displayHelp();
        return 0;
    }
    std::string action { argv[1] };

    struct args {
            std::string username { "N/S" };
            std::string slug { "N/S" };
            int year = -1;
            int month = -1;
            int limit = -1;
            bool help = false;
    };

    auto params = args();

    for (int index = 2; index < argc; ++index) {
        std::string arg {argv[index]};
        if(arg == "--username" || arg == "-u") {
            if(index + 1 >= argc) {
                fmt::print("Username requires an argument\n");
                return 0;
            }
            else
                params.username = argv[++index];
        } else if (arg == "--slug" || arg == "-s") {
            if(index + 1 >= argc) {
                fmt::print("Slug requires an argument\n");
                return 0;
            }
            else
                params.slug = argv[++index];
        } else if (arg == "--year" || arg == "-y") {
            if(index + 1 >= argc) {
                fmt::print("Year requires an argument\n");
                return 0;
            }
            else {
                params.year = atoi(argv[++index]);
                if(params.year == 0) {
                    fmt::print("Bad value for parameter year\n");
                    return 0;
                }
            }
        } else if (arg == "--month" || arg == "-m") {
            if(index + 1 >= argc) {
                fmt::print("Month requires an argument\n");
                return 0;
            }
            else {
                params.month = atoi(argv[++index]);
                if(params.month == 0){
                    fmt::print("Bad value for parameter month\n");
                    return 0;
                }
            }
        }
        else if (arg == "--limit" || arg == "-l") {
            if(index + 1 >= argc) {
                fmt::print("Limit requires an argument\n");
                return 0;
            }
            else {
                params.limit = atoi(argv[++index]);
                if(params.limit == 0) {
                    fmt::print("Bad value for parameter limit\n");
                    return 0;
                }
            }
        } else if (arg == "--help" || arg == "-h") {
            params.help = true;
        }
    }

    if(action == "--help") {
        displayHelp();
        return 0;
    }

    try {
        if(action == "info") {
            if(params.slug == "N/S" || params.help)
                fmt::print("Required parameters: slug\n");
            else {
                auto info = CCApi::serverInfo(params.slug);
                fmt::print("Name: {}\nAddress: {}\nPosition: {}\nVotes: {}\n", info.name, info.address, info.position,
                           info.votes);

            }
            return 0;
        }

        if(action == "votes") {
            if(params.slug == "N/S" || params.help)
                fmt::print("Required parameters: slug\n"
                           "Optional parameters: username, limit\n");
            else if(params.username == "N/S") { // server votes
                auto voteinfo = CCApi::serverVotes(params.slug);
                fmt::print("Total vote count: {}\n", voteinfo.vote_count);

                if (params.limit == -1)
                    params.limit = 100;

                if(params.limit > voteinfo.votes.size())
                    params.limit = voteinfo.votes.size();

                char buffer[32] = {0};
                int index = 0;
                for (const auto &item : voteinfo.votes) {
                    if(index++ >= params.limit)
                        break;
                    std::strftime(buffer, 32, CCApi::TIME_FORMAT, &item.date);
                    fmt::print(" |{}| {} - {}\n", std::string(buffer), item.username,
                               item.delivered ? "Delivered" : "Not delivered");
                }
                return 0;
            } else { // player votes
                auto profile = CCApi::userVotes(params.username, params.slug);
                if (params.limit == -1)
                    params.limit = 10;
                if(params.limit > profile.votes.size())
                    params.limit = profile.votes.size();

                if(profile.votes.empty()) {
                    fmt::print("No votes found for player {}\n", params.username);
                    return 0;
                }

                char buffer[32] = {0};
                std::strftime(buffer, 32, CCApi::TIME_FORMAT, &profile.next_vote);
                fmt::print("Vote count: {}\n"
                           "Next vote: {}\n"
                           "Votes:\n", profile.vote_count, std::string(buffer));

                int index = 0;
                for (const auto &item : profile.votes) {
                    if(index++ >= params.limit)
                        break;
                    std::strftime(buffer, 32, CCApi::TIME_FORMAT, &item.date);
                    fmt::print(" |{}| - {}\n", std::string(buffer), item.delivered ? "Delivered" : "Not delivered");
                }
            }
            return 0;
        }

        if(action == "top" || action == "topvoters"){
            if(params.slug == "N/S" || params.help)
                fmt::print("Required parameters: slug\n"
                           "Optional parameters: limit\n");
            else {
                auto topvoters = CCApi::topVoters(params.slug);
                fmt::print("Total vote count: {}\n", topvoters.size());

                if (params.limit == -1)
                    params.limit = 100;

                if(params.limit > topvoters.size())
                    params.limit = topvoters.size();

                int index = 0;
                for (const auto &item : topvoters) {
                    if(++index > params.limit)
                        break;
                    fmt::print(" {}. {}, votes: {}\n", index, item.username, item.vote_count); //note index fucked prob
                }
            }
            return 0;
        }

        if(action == "nextvote" || action == "next") {
            if(params.slug == "N/S" || params.username == "N/S" || params.help)
                fmt::print("Required parameters: slug, username\n");
            else {
                auto info = CCApi::nextVote(params.username, params.slug);
                char buffer[32] = {0};
                std::strftime(buffer, 32, CCApi::TIME_FORMAT, &info.next_vote);
                fmt::print("Next vote: {}\n", std::string(buffer));
            }
            return 0;
        }
        fmt::print("Unknown command\n");

    } catch (std::exception &ex) {
        fmt::print("Failed to process request. Cause: {}\n", ex.what());
        return 1;
    }
    return 0;
}
