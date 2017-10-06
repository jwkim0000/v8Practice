#!/usr/bin/node

var m = require('./test.node');

console.log('hello : ' + m.hello());
console.log('sum1 : ' + m.sum(1));
console.log('sum2 : ' + m.sum(1, 2.2));
console.log('sum3 : ' + m.sum(1, 2.2, 3));

