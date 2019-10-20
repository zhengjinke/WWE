package main

import (
	"testing"
)

var (
	test_exp *TempExpression
)

func Benchmark_Division(b *testing.B) {

	strexp := "Lv+3"
	test_exp, _ = NewExpression(strexp)
	for i := 0; i < b.N; i++ {
		test_exp.CaculateRes(8)
	}
}
