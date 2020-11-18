var express = require('express');
var router = express.Router();

/* GET home page. */
router.get('/', function (req, res, next) {
  console.log(req.body);
  res.send("OKAY");
  // res.status(200);
});

module.exports = router;
