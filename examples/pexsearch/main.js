const pex = require("pex-sdk");

const CLIENT_ID = "#YOUR_CLIENT_ID_HERE";
const CLIENT_SECRET = "#YOUR_CLIENT_SECRET_HERE";
const INPUT_FILE = "/path/to/file.mp3";

async function main() {
  try {
    // First step is to create a new PexSearch client.
    let client = new pex.PexSearchClient(CLIENT_ID, CLIENT_SECRET)

    // Connect to the PexSearch server. This operation is asynchronous and
    // returns a promise, just like most operations in this package.
    await client.connect()

    // Mock the client. This will make the client talk to the local testing
    // server instead of production. This must be ommited when deploying to
    // production.
    await pex.mock(client)

    // Fingerprint the input file. We're requesting only an audio fingerprint.
    // To get only a melody fingerprint:
    //   let ft = await client.fingerprintFile(INPUT_FILE, [pex.MELODY])
    // To get both:
    //   let ft = await client.fingerprintFile(INPUT_FILE, [pex.AUDIO, pex.MELODY])
    //   or:
    //   let ft = await client.fingerprintFile(INPUT_FILE)
    let ft = await client.fingerprintFile(INPUT_FILE, [pex.AUDIO])

    // Start the search and await the result.
    let result = await client.startSearch({
      fingerprint: ft,           // Fingerprint is required.
      type: pex.IDENTIFY_MUSIC,  // This can also be pex.FIND_MATCHES.
                                 // If not specified, defaults to pex.IDENTIFY_MUSIC.
    })

    // Pretty print the result.
    console.log(JSON.stringify(result, null, 2))
  } catch (err) {
    // If any of the operations above fail, this block will be executed.
    console.log(err)
  }
}

main()
