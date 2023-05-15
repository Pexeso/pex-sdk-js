const pexae = require('./build/Release/pexae.node')

async function main() {
  var ps = new pexae.PexSearch("client01", "secret01");
  await ps.connect();
  var ft = await ps.fingerprintFile("/Users/stepan/Downloads/query03.mp3")
  res = await ps.startSearch(ft)
  console.log(JSON.stringify(res, null, 2))
}

main();
