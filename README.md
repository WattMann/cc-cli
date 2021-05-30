![Latest tag](https://img.shields.io/github/v/tag/WattMann/cc-cli)
![License](https://img.shields.io/badge/license-MIT-blue)

# CzechCraft CLI Application
<b>This an an unofficial application. I am not associated with CzechCraft at all.</b><br><br>
Command line application written in c++ that allows you to interact with the [CzechCraft](https://czech-craft.eu) API.<br>
Tested on linux `5.11.0-17`, compiled with `GNU GCC 10.3.0`.


## Usage
```
Usage: cc-cli <command> [arguments]
Commands:
 info                       display server information
 votes                      display server or user votes
 top | topvoters            display server top voters
 next | nextvote            display next vote date
Arguments:
 -h, --help                 display overall help or command specific help
 -s, --slug [slug]          specify server slug
 -u, --username [username]  specify player username
 -y, --year [year]          specify year span
 -m, --month [year]         specify month span

```

## Dependencies
Dependencies are handled with conan.
- [nlohmann/json](https://github.com/nlohmann/json)
- [curl](https://curl.se/libcurl/)
- [fmt](https://github.com/fmtlib/fmt)

## Building on linux
```
$ mkdir build &&cd build
$ cmake --configure .. -DCMAKE_BUILD_TYPE=<Release/Debug>
$ cmake --build . && cmake --install . --prefix <PATH>
```

## Building on windows
```
$ mkdir build && cd build
$ cmake .. -DCMAKE_BUILD_TYPE=<Release/Debug>
$ cmake --build .
```


## Examples
```
$ cc-cli top --slug warfaremc --limit 5
Total vote count: 25
 1. henten, votes: 279
 2. aplayer, votes: 269
 3. pathetic, votes: 256
 4. creature, votes: 237
 5. anorange_, votes: 235
```
```
$ cc-cli votes --slug warfaremc --username WattMann --limit 5
Vote count: 2
Next vote: 2021-05-28 18:50:35
Votes:
 |2021-05-28 16:50:35| - Delivered
 |2021-05-11 17:03:44| - Delivered
```

I personally recommend piping the output trough lolcat for immersive experience.