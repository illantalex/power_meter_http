var createError = require('http-errors');
var express = require('express');

var apiRouter = require('./routes/api');
var serviceRouter = require('./routes/service');

var app = express();

// view engine setup


app.use('/', apiRouter);
app.use('/service', serviceRouter);

// catch 404 and forward to error handler
// app.use(function(req, res, next) {
//   next(createError(404));
// });

// // error handler
// app.use(function(err, req, res, next) {
//   // set locals, only providing error in development
//   res.locals.message = err.message;
//   res.locals.error = req.app.get('env') === 'development' ? err : {};

//   // render the error page
//   res.status(err.status || 500);
//   res.render('error');
// });

module.exports = app;
