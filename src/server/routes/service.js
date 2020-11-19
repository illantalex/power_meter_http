var express = require('express');
var router = express.Router();

/* GET users listing. */
router.get('/time', function(req, res, next) {
  res.send(`${Math.floor((new Date()).getTime() / 1000)}`);
});

module.exports = router;
