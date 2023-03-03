package main

import (
	"crypto/rand"
	"fmt"
	"math/big"
)

var zero *big.Int = big.NewInt(0)
var one *big.Int = big.NewInt(1)

func main() {
	const e int64 = 65537
	const bitsize int = 512

	eBig := big.NewInt(e)
	pBig, qBig := generatePQ(bitsize)
	phiBig := calcPhiN(pBig, qBig)
	for gcd(phiBig, eBig).Cmp(one) != 0 {
		pBig, qBig = generatePQ(bitsize)
		phiBig = calcPhiN(pBig, qBig)
	}
	fmt.Printf("p: %s\n", pBig.String())
	fmt.Printf("q: %s\n", qBig.String())
	fmt.Printf("phiN: %s\n", phiBig.String())
	dBig := extendEuclid(eBig, phiBig)
	if dBig.Sign() < 0 {
		dBig.Add(dBig, phiBig)
	}
	fmt.Printf("d: %s\n", dBig.String())

	nBig := new(big.Int)
	nBig = nBig.Mul(pBig, qBig)
	fmt.Printf("n: %s\n", nBig.String())
	if !verify(eBig, nBig, dBig, 20) {
		panic("(m^e mod n)^d mod n != m")
	}
}

func calcPhiN(p, q *big.Int) *big.Int {
	pSub1 := new(big.Int)
	qSub1 := new(big.Int)
	pSub1.Sub(p, one)
	qSub1.Sub(q, one)
	return new(big.Int).Mul(pSub1, qSub1)
}

func generatePrime(bitsize int) *big.Int {
	pBig, err := rand.Prime(rand.Reader, bitsize)
	if err != nil {
		panic(err)
	}

	return pBig
}

func generatePQ(bitsize int) (*big.Int, *big.Int) {
	pBig := generatePrime(bitsize)
	qBig := generatePrime(bitsize)

	return pBig, qBig
}

func gcd(x, y *big.Int) *big.Int {
	if x.Cmp(y) <= 0 {
		return big.NewInt(0)
	}
	if y.Cmp(big.NewInt(0)) == 0 {
		return x
	} else if y.Cmp(big.NewInt(1)) == 0 {
		return big.NewInt(1)
	}
	return gcd(y, new(big.Int).Mod(x, y))
}

func extendEuclid(a, b *big.Int) *big.Int {
	aVal := new(big.Int)
	bVal := new(big.Int)
	aVal.SetBytes(a.Bytes())
	bVal.SetBytes(b.Bytes())
	if bVal.Cmp(zero) == 0 {
		return big.NewInt(1)
	}
	x := new(big.Int)
	y := new(big.Int)
	q := new(big.Int)
	r := new(big.Int)
	x1 := big.NewInt(0)
	x2 := big.NewInt(1)
	y1 := big.NewInt(1)
	y2 := big.NewInt(0)
	for bVal.Cmp(zero) > 0 {
		q.Div(aVal, bVal)
		r.Sub(aVal, r.Mul(q, bVal))
		x.Sub(x2, x.Mul(q, x1))
		y.Sub(y2, y.Mul(q, y1))
		aVal.SetString(bVal.String(), 10)
		bVal.SetString(r.String(), 10)
		x2.SetString(x1.String(), 10)
		x1.SetString(x.String(), 10)
		y2.SetString(y1.String(), 10)
		y1.SetString(y.String(), 10)
	}
	return x2
}

func verify(eBig, nBig, dBig *big.Int, iters int) bool {
	mBig := new(big.Int)
	var err error

	for i := 0; i < iters; i++ {
		mBig, err = rand.Int(rand.Reader, nBig)
		if err != nil {
			panic(err)
		}
		// Calculate (m^e mod n)^d mod n
		temp := modExp(modExp(mBig, eBig, nBig), dBig, nBig)
		if mBig.Cmp(temp) != 0 {
			return false
		}
	}
	return true
}

// Calculate g^a mod p
func modExp(g *big.Int, a *big.Int, p *big.Int) *big.Int {
	var product *big.Int = new(big.Int)
	var shifted *big.Int = new(big.Int)
	var and *big.Int = new(big.Int)
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
