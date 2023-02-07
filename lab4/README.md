# ham-diffie-hellman

My implemntation of the Diffie-Hellman algorithm for public/private key computation written in Go. Calculates g^a mod p, using 512-bit values for a and p.

## Build

Make sure you have Go installed, then run:

```
go build diffie-hellman.go
```

### How to run

Calculate mod-exp (g^a mod p) with random values for "a" and "p":

```
ham-diffie-hellman <g>
```

Calculate mod-exp (g^a mod p):

```
ham-diffie-hellman <g> <a> <p>
```

Calculate mod-exp (g^ab mod p):

```
ham-diffie-hellman <g> <a> <p> <b>
```

