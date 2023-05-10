const pexae = require('./build/Debug/pexae.node')

async function main() {
  var ps = new pexae.PexSearch("client01", "secret01");
  await ps.connect();
  var ft = await ps.fingerprintFile("/Users/stepan/Downloads/pexsearch_query_01.mp3")
  var dump = ft.dump();
  console.log(dump.toString('base64'));
}

main();
