#include "CCApi.hpp"
#include <fmt/format.h>
#include <chrono>

void displayHelp() {
    fmt::print("Usage: cc-cli [COMMAND] [REQUIRED PARAM] (OPTIONAL PARAM=DEF VAL)\n"
               "Commands:\n"
               "\thelp - Displays this dialog.\n"
               "\tinfo [slug] - Displays the server information.\n"
               "\tvotes [slug] (limit=100) - Lists votes.\n"
               "\ttopvoters [slug] (limit=5) - Lists the top voters.\n"
               "\tplayervotes [username] [slug] (limit=10) - Lists player's votes.\n"
               "\tnextvote [username] [slug] - Displays the date when it's going to be possible to vote again.\n"
    );
}

int main(int argc, char **argv) {
    if (argc < 2 || strcmp(argv[1], "help") == 0)
        displayHelp();
    else {
        if (strcmp(argv[1], "info") == 0) {
            if (argc < 3) {
                fmt::print("Argument count too small\n");
                displayHelp();
                return 0;
            }

            fmt::print("Working...\n");
            auto info = CCApi::serverInfo(argv[2]);
            fmt::print("Name: {}\nAddress: {}\nPosition: {}\nVotes: {}\n", info.name, info.address, info.position,
                       info.votes);
            return 0;
        }
        if (strcmp(argv[1], "votes") == 0) {
            if (argc < 3) {
                fmt::print("Argument count too small\n");
                displayHelp();
                return 0;
            }

            fmt::print("Working...\n");
            unsigned long size = 100;
            if (argc >= 4)
                size = std::stoi(argv[3]);

            auto votevector = CCApi::serverVotes(argv[2]);
            fmt::print("Total vote count: {}\n", votevector.vote_count);
            fmt::print("Displaying: {}\n", size);
            if (size == -1 || size > votevector.votes.size())
                size = votevector.votes.size();

            char buffer[32] = {0};
            int index = 0;
            for (const auto &item : votevector.votes) {
                if(index++ >= size)
                    break;
                std::strftime(buffer, 32, CCApi::TIME_FORMAT, &item.date);
                fmt::print("[{}] - {} - {}\n", std::string(buffer), item.username,
                           item.delivered ? "delivered" : "not delivered");
            }

            return 0;
        }
        if (strcmp(argv[1], "topvoters") == 0) {
            if (argc < 3) {
                fmt::print("Argument count too small\n");
                displayHelp();
                return 0;
            }

            unsigned long size = 5;
            if (argc >= 4)
                size = std::stoi(argv[3]);

            fmt::print("Working...\n");
            auto data = CCApi::topVoters(argv[2]);

            if (size == -1 || size > data.size())
                size = data.size();

            fmt::print("Displaying: {}\n", size);
            int place = 1;

            int index = 0;
            for (const auto &item : data) {
                if(index++ >= size)
                    break;
                fmt::print("[{}] - {}, votes: {}\n", place++, item.username, item.vote_count);
            }

            return 0;
        }
        if (strcmp(argv[1], "playervotes") == 0) {
            if (argc < 4) {
                fmt::print("Argument count too small\n");
                displayHelp();
                return 0;
            }
            unsigned long size = 10;
            if (argc >= 5)
                size = std::stoi(argv[4]);

            fmt::print("Working...\n");

            auto profile = CCApi::userVotes(argv[2], argv[3]);
            if (size == -1 || size > profile.votes.size())
                size = profile.votes.size();
            fmt::print("Displaying: {}\n", size);
            char buffer[32] = {0};
            std::strftime(buffer, 32, CCApi::TIME_FORMAT, &profile.next_vote);
            fmt::print("Username: {}\n"
                       "Vote count: {}\n"
                       "Next vote: {}\n"
                       "Votes:\n", profile.username, profile.vote_count, std::string(buffer));

            int index = 0;
            for (const auto &item : profile.votes) {
                if(index++ >= size)
                    break;
                std::strftime(buffer, 32, CCApi::TIME_FORMAT, &item.date);
                fmt::print("\t[{}] - {}\n", std::string(buffer), item.delivered ? "delivered" : "not delivered");
            }

            return 0;
        }

        if (strcmp(argv[1], "nextvote") == 0) {
            if (argc < 4) {
                fmt::print("Argument count too small\n");
                displayHelp();
                return 0;
            }

            fmt::print("Working...\n");
            auto info = CCApi::nextVote(argv[2], argv[3]);
            char buffer[32] = {0};
            std::strftime(buffer, 32, CCApi::TIME_FORMAT, &info.next_vote);
            fmt::print("Username: {}\nNext vote: {}\n", info.username, std::string(buffer));

            return 0;
        }

        fmt::print("Unrecognized command\n");
        displayHelp();

    }
    return 0;
}
