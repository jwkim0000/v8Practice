#!/usr/bin/node

var m = require('./test.node');

console.log('----- Test 01 hello -----');
console.log('hello : ' + m.hello());
console.log('-------------------------');
console.log();

console.log('----- Test 02 directCall positive -----');
m.directCall(function(text) {
  console.log('In callback: ' + text);
});
console.log('-------------------------');
console.log();

console.log('----- Test 02 directCall negative -----');
try {
  m.directCall('');
} catch (err) {
  console.error('catch: ' + err);
}
console.log('-------------------------');
console.log();

console.log('----- Test 03 asyncCall positive -----');
m.asyncCall(function(text) {
  console.log('In async callback: ' + text);
});
console.log('-------------------------');
console.log();
