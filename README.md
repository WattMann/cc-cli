
# cc-cli
Command line application written in c++ that allows you to interact with [CzechCraft](https://czech-craft.eu) API.

## Commands
- info - Shows server info
- votes - Shows server's votes
- topvoters - Shows server's top voters
- playervotes - Shows player's votes
- nextvote - Shows date  when it's going to be possible to vote again

## Dependencies
- [nlohmann/json](https://github.com/nlohmann/json)
- [curl](https://curl.se/libcurl/)
- [fmt](https://github.com/fmtlib/fmt)

## Building on linux
```
$ mkdir build &&cd build
$ cmake --configure ../.
$ cmake --build .
$ cmake --install . --prefix "$HOME/.local"
```

## Examples
```
$ cc-cli
Usage: cc-cli [REQUIRED] (OPTIONAL) <GREEDY>
Arguments:
	help - Displays this dialog.
	info [server slug] - Displays the server information.
	votes [server slug] (count) - Displays last 100 voters, if count is specified it's used instead.
	topvoters [server slug] (count) - Displays top 5 voters, if count is specified it's used instead.
	playervotes [username] [serverslug] (count) - Displays player's votes for specified server, 10 of them, unless count is specified.
	nextvote [username] [server slug] - Displays date and time when it's gonna possible for specified player to vote again.
```

```
$ cc-cli playervotes WattMann warfaremc
Working...
Displaying: 1
Username: WattMann
Vote count: 1
Next vote: 2021-05-11 19:03:44
Votes:
	[2021-05-11 17:03:44] - delivered
```
