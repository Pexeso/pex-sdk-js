# NodeJS bindings for the Attribution Engine SDK

For more information look at https://docs.ae.pex.com/ae/.

### Installation

To install these bindings you can run the following command:

```
npm install git+ssh://git@github.com:Pexeso/ae-sdk-js.git
```

And once installed you can use it like in the example below.

```js
const ae = require('ae-sdk')

async function main() {
  try {
    let client = new ae.PexSearch("client01", "secret01")
    await client.connect()
    await ae.mock(client)

    ft = await client.fingerprintFile("video.mp4")
    result = await client.startSearch(ft)

    console.log(JSON.stringify(result, null, 2))
  } catch (err) {
    console.log(err)
  }
}

main()
```
