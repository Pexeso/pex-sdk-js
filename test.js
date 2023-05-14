const pexae = require('./build/Release/pexae.node')

async function main() {
  var ps = new pexae.PexSearch("client01", "secret01");
  await ps.connect();
  var ft = await ps.fingerprintFile("/Users/stepan/Downloads/pexsearch_query_01.mp3")
}

main();
