#!/bin/bash -e

export PEX_SDK_LOG_LEVEL=debug

node-gyp build
node test.js
