# NodeJS bindings for the Attribution Engine SDK

For more information look at https://docs.search.pex.com/.

### Installation

**Important!** This software requires Node.js version 18.0.0 or later.

This package is written in C++ using https://github.com/nodejs/node-addon-api.
As such it requires some extra dependencies to build. You can install them like this:

##### Ubuntu/Debian

```
sudo apt-get install -y python3 pkg-config make g++
```

##### macOS

```
brew install pkg-config
```

Once you have the dependencies installed you can install the NodeJS language bindings like this:

```
npm install git+ssh://git@github.com:Pexeso/pex-sdk-js.git
```

### Usage examples

You can find usage examples in the [examples directory](examples).

.
