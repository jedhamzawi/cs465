package main

import (
	"crypto/rand"
	"fmt"
	"math/big"
	"os"
	"strconv"
)

func main() {
	const BITSIZE int = 512

	if len(os.Args) < 2 || len(os.Args) == 3 || len(os.Args) > 5 {
		fmt.Println("Invalid arguments!")
		usage()
		os.Exit(1)
	}

	g, parseErr := strconv.Atoi(os.Args[1])
	if parseErr != nil {
		panic(parseErr)
	}

	var aBig *big.Int = new(big.Int)
	var pBig *big.Int = new(big.Int)
	var gBig *big.Int = big.NewInt(int64(g))

	if len(os.Args) >= 4 {
		aBig.SetString(os.Args[2], 10)
		pBig.SetString(os.Args[3], 10)
	} else if len(os.Args) == 2 {
		aBig = randBigInt(int64(BITSIZE))
		pBig = safePrime(BITSIZE)
	}
	fmt.Printf("a: %s\n", aBig.String())
	fmt.Printf("p: %s\n", pBig.String())

	if len(os.Args) != 5 {
		ga := modExp(gBig, aBig, pBig)
		fmt.Printf("g^a mod p: %s\n", ga.String())
	} else {
		var bBig *big.Int = new(big.Int)
		bBig.SetString(os.Args[4], 10)

		fmt.Printf("b: %s\n", bBig.String())

		gab := modExp(bBig, aBig, pBig)
		fmt.Printf("g^ab mod p: %s\n", gab.String())
	}
}

func usage() {
	fmt.Println("Usage:")
	fmt.Println("  Calculate mod-exp (g^a mod p) with random values for \"a\" and \"p\"")
	fmt.Println("    diffie-hellman <g>")
	fmt.Println("  Calculate mod-exp (g^a mod p)")
	fmt.Println("    diffie-hellman <g> <a> <p>")
	fmt.Println("  Calculate mod-exp (g^[a*b] mod p) ")
	fmt.Println("    diffie-hellman <g> <a> <p> <b>")
}

// Generate safe prime where p is prime and p/2-1 is also prime
func safePrime(bitsize int) *big.Int {
	pBig, err := rand.Prime(rand.Reader, bitsize)
	one := big.NewInt(1)
	if err != nil {
		panic(err)
	}
	for pBig.Sub(pBig.Div(pBig, big.NewInt(2)), one).ProbablyPrime(20) {
		pBig, err = rand.Prime(rand.Reader, bitsize)
		if err != nil {
			panic(err)
		}
	}
	return pBig
}

// Generate cryptographically safe number of given size
func randBigInt(bitsize int64) *big.Int {
	max := new(big.Int)
	max.Exp(big.NewInt(2), big.NewInt(bitsize), nil).Sub(max, big.NewInt(1))
	aBig, err := rand.Int(rand.Reader, max)
	if err != nil {
		panic(err)
	}
	return aBig
}

// Calculate g^a mod p
func modExp(g *big.Int, a *big.Int, p *big.Int) *big.Int {
	var product *big.Int = new(big.Int)
	var shifted *big.Int = new(big.Int)
	var and *big.Int = new(big.Int)
	var one *big.Int = big.NewInt(1)
	var zero *big.Int = big.NewInt(0)
	product.SetString(g.String(), 10)
	shifted.SetString(a.String(), 10)

	var total *big.Int = new(big.Int)
	if and.And(shifted, one).Cmp(zero) != 0 {
		total.SetString(g.String(), 10)
	} else {
		total.SetInt64(1)
	}
	shifted.Rsh(shifted, 1)

	numBits := a.BitLen()
	var i int
	for i = 1; i < numBits; i++ {
		product.Mod(product.Mul(product, product), p)
		if and.And(shifted, one).Cmp(zero) != 0 {
			total.Mod(total.Mul(total, product), p)
		}
		shifted.Rsh(shifted, 1)
	}

	return total
}
