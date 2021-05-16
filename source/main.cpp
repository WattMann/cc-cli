#include <fmt/format.h>
#include "CCApi.hpp"

void displayHelp() {
    fmt::print("Usage: cc-cli [REQUIRED] (OPTIONAL) <GREEDY>\n"
               "Arguments:\n"
               "\thelp - Displays this dialog.\n"
               "\tinfo [server slug] - Displays the server information.\n"
               "\tvotes [server slug] (count) - Displays last 100 voters, if count is specified count it's used instead.\n"
               "\ttopvoters [server slug] (count) - Displays top 5 voters, if count is specified it's used instead.\n"
               "\tplayervotes [server slug] [username] (count) - Displays player's votes for specified server, 10 of them, unless count is specified.\n"
               "\tnextvote [server slug] [username] - Displays date and time when it's possible for specified player to vote again.\n"
    );
}
int main(int argc, char** argv) {
    if(argc < 2 || strcmp(argv[1], "help") == 0)
      displayHelp();
    else {
        if(strcmp(argv[1], "info") == 0) {
            if(argc < 3){
                fmt::print("Argument count too small\n");
                return 0;
            }

            fmt::print("Working...\n");
            auto info = CCApi::serverInfo(argv[2]);
            fmt::print("Name: {}\nAddress: {}\nPosition: {}\nVotes: {}\n", info.name, info.address, info.position, info.votes);

        }
        if(strcmp(argv[1], "votes") == 0) {
            if(argc < 3){
                fmt::print("Argument count too small\n");
                displayHelp();
                return 0;
            }

            fmt::print("Working...\n");
            unsigned long size = 100;
            if(argc >= 4)
                size = std::stoi(argv[3]);

            auto votevector = CCApi::serverVotes(argv[2]);
            fmt::print("Total vote count: {}\n", votevector.vote_count);
            fmt::print("Displaying: {}\n", size);
            if(size == -1 || size > votevector.votes.size())
                size = votevector.votes.size();

            for (int i = 0; i < size; ++i) {
                auto ctx = votevector.votes.at(i);
                fmt::print("[{}] - {} - {}\n", ctx.date, ctx.username, ctx.delivered ? "delivered" : "not delivered");
            }

        }
        if(strcmp(argv[1], "topvoters") == 0) {
            if(argc < 3){
                fmt::print("Argument count too small\n");
                return 0;
            }

            unsigned long size = 5;
            if(argc >= 4)
                size = std::stoi(argv[3]);

            fmt::print("Working...\n");
            auto data = CCApi::topVoters(argv[2]);

            if(size == -1 || size > data.size())
                size = data.size();

            for (int i = 0; i < size; ++i) {
                auto ctx = data.at(i);
                fmt::print("[{}] - {}, votes: {}\n", i + 1, ctx.username, ctx.vote_count);
            }
        }
        if(strcmp(argv[1], "playervotes") == 0) {
            if(argc < 4) {
                fmt::print("Argument count too small\n");
                return 0;
            }
            unsigned long size = 10;
            if(argc >= 5)
                size = std::stoi(argv[4]);

            fmt::print("Working...\n");

            auto profile = CCApi::userVotes(argv[3], argv[2]);
            if(size == -1 || size > profile.votes.size())
                size = profile.votes.size();
            fmt::print("Username: {}\n"
                       "Vote count: {}\n"
                       "Next vote: {}\n"
                       "Votes:\n", profile.username, profile.vote_count, profile.next_vote);
            for (int i = 0; i < size; ++i) {
                auto ctx = profile.votes.at(i);
                fmt::print("\t[{}] - {}\n", ctx.date, ctx.delivered ? "delivered" : "not delivered");
            }
        }

        if(strcmp(argv[1], "nextvote") == 0) {

        }
    }
    return 0;
}
