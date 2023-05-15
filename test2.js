const pexae = require('./build/Release/pexae.node')

async function start(ps) {
  await ps.connect();
  let ft = await ps.fingerprintFile("/Users/stepan/Downloads/query03.mp3")
  return ps.startSearch(ft)
}

async function main() {
  let ps = new pexae.PexSearch("client01", "secret01");
  p = start(ps)
  setTimeout(function() {
    p.then(function(res) {
      console.log(JSON.stringify(res, null, 2))
    })
  }, 5000)
}

main();
