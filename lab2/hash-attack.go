package main

import (
	"crypto/sha1"
	"encoding/binary"
	"fmt"
	"math/rand"
	"os"
	"strconv"
	"time"
)

const HASH_LENGTH_BYTES int = 20

func main() {
	if len(os.Args) != 3 {
		fmt.Println("Missing arguments!")
		usage()
		os.Exit(1)
	}

	var parseErr error
	var truncInt int
	truncInt, parseErr = strconv.Atoi(os.Args[2])
	if parseErr != nil {
		panic(parseErr)
	}

	trunc := uint32(truncInt)
	buf := make([]byte, 64)
	rand.Seed(time.Now().UnixNano())

	if os.Args[1] == "preimage" {
		nonce := rand.Uint64()
		hash := hashNonce(nonce, buf)
		hashTrunced := hashToUint(hash, trunc)
		fmt.Printf("Searching for some preimage bytes whose hash has the same first %d bits as this buf\n\"%x\"\n",
			trunc, nonce)

		matchingNonce, attempts := preimage(hashTrunced, buf, trunc)

		fmt.Printf("Found match in %d attempts!\nMatch: %x\n", attempts, matchingNonce)
		fmt.Printf("Matching hashes:\nGiven: %x\nFound: %x\n", hash, hashNonce(matchingNonce, buf))
	} else if os.Args[1] == "collision" {
		fmt.Printf("Searching for some collision where hashes has the same first %d bits\n", trunc)

		nonce1, nonce2, attempts := collision(buf, trunc)

		fmt.Printf("Found match in %d attempts!\n", attempts)
		fmt.Printf("Input 1: %x\nInput 2: %x\nHash 1: %x\nHash 2: %x\n",
			nonce1, nonce2, hashNonce(nonce1, buf), hashNonce(nonce2, buf))
	} else {
		fmt.Println("Invalid attack! Must be preimage or collision")
		usage()
		os.Exit(1)
	}
}

func usage() {
	fmt.Println("  Usage: ./hash-attack <preimage/collision> <bits-to-truncate>")
}

func hashNonce(nonce uint64, buf []byte) [HASH_LENGTH_BYTES]byte {
	binary.BigEndian.PutUint64(buf, nonce)
	return sha1.Sum(buf)
}

func hashToUint(bytes [HASH_LENGTH_BYTES]byte, trunc uint32) uint64 {
	var result uint64 = 0
	var numBytes uint32 = (trunc / 8)
	if trunc%8 != 0 {
		numBytes++
	}
	var i uint32
	for i = 0; i < numBytes; i++ {
		result = result << 8
		result += uint64(bytes[i])
	}

	if trunc%8 == 0 {
		return result
	}
	return result >> (8 - (trunc % 8))
}

func preimage(hashTrunced uint64, buf []byte, trunc uint32) (uint64, uint64) {
	var attempts uint64 = 0
	nonce := rand.Uint64()
	for hashToUint(hashNonce(nonce, buf), trunc) != hashTrunced {
		nonce = rand.Uint64()
		attempts++
	}

	return nonce, attempts
}

func collision(buf []byte, trunc uint32) (uint64, uint64, uint64) {
	truncHashMap := make(map[uint64]uint64)
	var attempts uint64 = 0
	var exists bool = false
	var nonce1, nonce2 uint64
	var hash uint64
	for !exists {
		nonce1 = rand.Uint64()
		hash = hashToUint(hashNonce(nonce1, buf), trunc)
		nonce2, exists = truncHashMap[hash]
		truncHashMap[hash] = nonce1
		attempts++
	}

	return nonce1, nonce2, attempts
}
