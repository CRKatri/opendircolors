# Moved to https://git.cameronkatri.com/CRKatri/opendircolors

# opendircolors
A BSD-licensed replacement for GNU dircolors(1)

FreeBSD's (and MacOS') `ls(1)` supports colors using the `LSCOLORS` variable, where as GNU's uses the more customizable `LS_COLORS` which is generated with the command `dircolors(1)` in coreutils. This repo contains two different utilites, `opendircolors(1)` and `dirconvert(1)`.

### `opendircolors(1) [-bch] file`
A near drop in replacement for GNU's `dircolors(1)`, lacking only built in defaults and `-p`. This will read from `file` and prints the commands needed to set both `LSCOLORS` and `LS_COLORS`. The typical usage is to add ``eval "`opendircolors ~/.dir_colors`"`` to your `.*shrc`.

### `dirconvert(1) LSCOLORS | LS_COLORS`
Will read the inputed string and convert it to the other format, ignoring anything that can't be converted.
