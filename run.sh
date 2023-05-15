#!/bin/bash -e

export AE_SDK_LOG_LEVEL=debug

node-gyp build
node test3.js
