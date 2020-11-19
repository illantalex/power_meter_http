var express = require('express');
var router = express.Router();

/* POST mesurement from device to server. */
router.get('/measurement', function (req, res, next) {
  console.log(req.body);
  // res.status(200);
});

module.exports = router;
