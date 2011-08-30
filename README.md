Description
===========

Provides several hashing algorithms for [node.js](http://nodejs.org/) by binding to mhash.


Usage
=====
<pre>
var hash = require("mhash").hash;

console.log(hash("md2", "testing"));
</pre>


Supported Hashing Algorithms
============================
* crc32

* crc32b

* md2

* md4

* md5

* haval128

* haval160

* haval192

* haval224

* haval256

* sha1

* sha224

* sha256

* sha384

* sha512

* ripemd128

* ripemd160

* ripemd256

* ripemd320

* tiger128

* tiger160

* tiger192

* gost

* whirlpool

* adler32

* snefru128

* snefru256


Installation with NPM
=====================

npm install mhash


Manual Installation
===================

    node-waf configure build

