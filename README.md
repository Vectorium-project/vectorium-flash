This version of Vectorium cryptocurrency is based on Zcash 2.0.2 source code, introducing overwinter and sapling support keeping Vectorium PoW algorithm and consensus rules.

Vectorium flash's source code goals. 

- Make the blockchain as distributed as possible (minable blocks will be distributed in the most decentralized way possible, via secured solo pools);


- Greater security, in fact a 51% attacks is very hard to do, the blockchain is stable even with only a graphic card;


- Greater advantage for small miner compared to mining farms;


- The blockchain is private and belong to Vectorium company; Anyway the mining is free and anyone with a graphics card can participate in mining;


- When the hashrate will be high, for more blocks distribution the miners will be paused and in the meantime they'll able to undermine other coins.


Building
-----------------

### Install dependencies

On Ubuntu/Debian-based systems:

```
$ sudo apt-get install \
      build-essential pkg-config libc6-dev m4 g++-multilib autoconf libtool ncurses-dev unzip git python python-zmq zlib1g-dev wget bsdmainutils automake curl
```

On Fedora-based systems:

```
$ sudo dnf install git pkgconfig automake autoconf ncurses-devel python python-zmq wget gtest-devel gcc gcc-c++ libtool patch curl
```

Windows:
```
sudo apt-get install build-essential pkg-config libc6-dev m4 g++-multilib autoconf libtool ncurses-dev unzip git python zlib1g-dev wget bsdmainutils automake mingw-w64
```

On Mac systems:

```
brew tap discoteq/discoteq; brew install flock
brew install autoconf autogen automake
brew install gcc5
brew install binutils
brew install protobuf
brew install coreutils
brew install wget llvm
```

### Check GCC version

gcc/g++ 4.9 or later is required. SnowGem has been successfully built using gcc/g++ versions 4.9 to 7.x inclusive. Use ```g++ --version``` to check which version you have.

On Ubuntu Trusty, if your version is too old then you can install gcc/g++ 4.9 as follows:

```
$ sudo add-apt-repository ppa:ubuntu-toolchain-r/test
$ sudo apt-get update
$ sudo apt-get install g++-4.9
```

### Fetch the software and parameter files

Fetch our repository with git and run ```fetch-params.sh``` like so:
```
$ ./zcutil/fetch-params.sh
```

### Build Linux/MAC

Ensure you have successfully installed all system package dependencies as described above. Then run the build, e.g.:
```
$ git clone https://github.com/Vectorium-project/vectorium-flash
$ cd vectorium-flash/
$ chmod +x src/leveldb/build_detect_platform zcutil/build.sh zcutil/fetch-params.sh depends/cargo-checksum.sh depends/config.guess depends/config.sub autogen.sh share/genbuild.sh
$ ./zcutil/build.sh --disable-tests
```

This should compile our dependencies and build snowgemd

### Build Windows

Ensure you have successfully installed all system package dependencies as described above. Then run the build, e.g.:
```
$ git clone https://github.com/Vectorium-project/vectorium-flash
$ cd  vectorium-flash/
$ chmod +x src/leveldb/build_detect_platform zcutil/build.sh zcutil/fetch-params.sh depends/cargo-checksum.sh depends/config.guess depends/config.sub autogen.sh share/genbuild.sh
HOST=x86_64-w64-mingw32 ./zcutil/build-win.sh
```

--------