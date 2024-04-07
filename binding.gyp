{
  "targets": [
    {
      "target_name": "pex",
      "sources": [
        "baseworker.cc",
        "connectworker.cc",
        "defer.cc",
        "fingerprint.cc",
        "fingerprinter.cc",
        "fingerprintworker.cc",
        "init.cc",
        "pexsearch.cc",
        "searchworker.cc",
        "error.cc",
        "mock.cc"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
      ],
      "libraries": [
        "-Wl,-rpath,/usr/local/lib",
        "<!(pkg-config --libs pexsdk)"
      ],
      "defines": [
        "NAPI_DISABLE_CPP_EXCEPTIONS",
        "NAPI_ADDON_API_ENABLE_MAYBE",
      ]
    }
  ]
}
