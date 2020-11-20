var express = require('express');
var router = express.Router();
var { jsonParser } = require("../middleware/parsers")
/* POST mesurement from device to server. */
router.post('/measurement', jsonParser, function (req, res, next) {
  // console.log(req.headers);
  // console.log(req.body);
  console.log(req.body);
  res.send("OKAY");
});

module.exports = router;
