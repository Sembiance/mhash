console.log("Only works if you've done `npm install mhash`");

var hash = require("mhash").hash;

console.log(hash("md2", "testing"));
