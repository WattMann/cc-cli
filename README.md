![Latest tag](https://img.shields.io/github/v/tag/WattMann/cc-cli)
![License](https://img.shields.io/badge/license-MIT-blue)

# CzechCraft CLI Application
<b>This an an unofficial application. I am not associated with CzechCraft at all.</b><br><br>
Command line application written in c++ that allows you to interact with the [CzechCraft](https://czech-craft.eu) API.<br>
Tested on linux `5.11.0-17`, compiled with `GNU GCC 10.3.0`.


## Usage
```
Usage: cc-cli [COMMAND] [REQUIRED PARAM] (OPTIONAL PARAM=DEF VAL)
Commands:
	help - Displays this dialog.
	info [slug] - Displays the server information.
	votes [slug] (limit=100) - Lists votes.
	topvoters [slug] (limit=5) - Lists the top voters.
	playervotes [username] [slug] (limit=10) - Lists player's votes.
	nextvote [username] [slug] - Displays the date when it's going to be possible to vote again.
```

## Dependencies
Dependencies are handled with conan.
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

## Building on windows
```
$ mkdir build && cd build
$ cmake .. -DCMAKE_BUILD_TYPE=<Release/Debug>
$ cmake --build .
```


## Examples
```
$ cc-cli topvoters warfaremc
Working...
Displaying: 5
[1] - Name, votes: 86
[2] - DeadOne, votes: 86
[3] - OIrange, votes: 89
[4] - Henten, votes: 91
[5] - Lulw, votes: 92
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

