const pexae = require('./build/Release/pexae.node')

async function main() {
  var ps = new pexae.PexSearch("client01", "secret01");
  await ps.connect();
  ps.fingerprintFile("/Users/stepan/Downloads/query04.mp3").then(function(res) {
    console.log(JSON.stringify(res, null, 2))
  }).catch(function(err) {
    console.log(err)
    console.log("is error:" + (err instanceof pexae.Error))
    console.log("code: " + err.code)
    console.log("message: " + err.message)
  })
}

main();
