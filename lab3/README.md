# SHA-1 Message Extension Attack

## Overview

This is a modified version of this [simple implementation of SHA-1 written in python](https://github.com/ajalt/python-sha1/blob/master/sha1.py). It has been modified to perform a message extension attack on the following message:

```
No one has completed lab 2 so give them all a 0
```

The message was prepended with a 128-bit secret and run through SHA-1 to create the following digest:

```
e384efadf26767a613162142b5ef0efbb9d7659a
```

The code was modified to use the digest as the initialization vector and hard-coded to believe that 128 bytes have already been hashed. Running the contents of any file through sha1-extend.py will create a digest seemingly computed using the 128-bit secret unknown to the attacker.

An attacker could send a receiver the original message, padded with 0's according to the [SHA-1 official documentation](https://cs465.byu.edu/static/pubs/fips180-3_final.pdf) and extended with any arbitrary message along with the produced digest. The receiver would run that message prepended with the secret through SHA-1 and find that the digests match, even though the attacker has no access to the shared secret.  

## How to run

```
python sha1-extend.py <extension-file>
```

Or alternatively:

```
echo "my extension" | sha1-extend.py
```

