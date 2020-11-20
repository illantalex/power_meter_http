var bodyParser = require("body-parser");
var jsonParser = bodyParser.json();
var textParser = bodyParser.text();

module.exports.jsonParser = jsonParser;
module.exports.textParser = textParser;
