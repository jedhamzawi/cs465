package main

import (
	"crypto/rand"
	"fmt"
	"math/big"
	"os"
)

var DEBUG bool = false

func main() {
	const BITSIZE int = 512
	var argOffset int = 0
	if len(os.Args) < 2 {
		fmt.Println("No arguments given!")
		usage()
		os.Exit(1)
	}

	if os.Args[1] == "-d" ||
		os.Args[1] == "-D" ||
		os.Args[1] == "-debug" ||
		os.Args[1] == "-DEBUG" {
		DEBUG = true
		argOffset++
	}

	if len(os.Args) == 3+argOffset || len(os.Args) > 5+argOffset {
		fmt.Println("Invalid arguments!")
		usage();
		os.Exit(1)
	}


	var aBig *big.Int = new(big.Int)
	var pBig *big.Int = new(big.Int)
	var gBig *big.Int = new(big.Int)

	gBig.SetString(os.Args[1+argOffset], 10)

	if len(os.Args) >= 4+argOffset {
		aBig.SetString(os.Args[2+argOffset], 10)
		pBig.SetString(os.Args[3+argOffset], 10)
	} else if len(os.Args) == 2+argOffset {
		aBig = randBigInt(int64(BITSIZE))
		pBig = safePrime(BITSIZE)
	}
	printDebug("a: %s\n", aBig.String())
	printDebug("p: %s\n", pBig.String())

	if len(os.Args) != 5+argOffset {
		ga := modExp(gBig, aBig, pBig)
		printDebug("g^a mod p: %s\n", ga.String())
		println(ga.String())
	} else {
		var bBig *big.Int = new(big.Int)
		bBig.SetString(os.Args[4], 10)

		printDebug("b: %s\n", bBig.String())

		gab := modExp(bBig, aBig, pBig)
		printDebug("g^ab mod p: %s\n", gab.String())
		println(gab.String())
	}
}

func usage() {
	fmt.Println("Usage:")
	fmt.Println("  Calculate mod-exp (g^a mod p) with random values for \"a\" and \"p\"")
	fmt.Println("    ham-diffie-hellman <g>")
	fmt.Println("  Calculate mod-exp (g^a mod p)")
	fmt.Println("    ham-diffie-hellman <g> <a> <p>")
	fmt.Println("  Calculate mod-exp (g^[a*b] mod p) ")
	fmt.Println("    ham-diffie-hellman <g> <a> <p> <b>")
	fmt.Println("  Debug mode")
	fmt.Println("    ham-diffie-hellman <-d/-D/-debug/-DEBUG> <...args>")
}

func println(text string) {
	if !DEBUG {
		fmt.Println(text)
	}
}

func printDebug(format string, args ...any) {
	if DEBUG {
		fmt.Printf(format, args...)
	}
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
