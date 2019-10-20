package main //package main

import (
	"container/list"
	//	"encoding/binary"
	"fmt"
	"os"
	"runtime/pprof"
	"strconv"
	"strings"
)

type StackElem struct {
	szType string
	Var    interface{}
}

type StackCodeNode struct {
	szId  string      // 模拟寄存器id
	oper  byte        // 二元式运算符
	para1 interface{} // 模拟二元式参数1
	para2 interface{} // 模拟二元式参数2
	type1 int
	type2 int
}

type TempExpression struct {
	buffer  map[string]float64
	tmpCode *list.List
}

type Expression struct {
	stack    [3]*list.List //用于生成逆波兰式的两个栈，和用于生成中间代码的暂存栈
	strExp   string        //原始表达式
	priority map[byte]int  //运算符优先级
}

type FinalExpression struct {
	coe [10]int //最终式的系数
}

func (this *TempExpression) ShowFinalExp() {
	if this.buffer == nil || this.tmpCode == nil {
		err := fmt.Errorf("TempExpression is not initialized")
		fmt.Println(err)
		return
	}
	for iter := this.tmpCode.Front(); iter != nil; iter = iter.Next() {
		elem := iter.Value.(StackCodeNode)
		fmt.Println(elem.szId, elem.oper, elem.para1, elem.para2)
	}
}

func (this *TempExpression) GetFinalExp(Lv float64) {
	if this.buffer == nil || this.tmpCode == nil {
		err := fmt.Errorf("TempExpression is not initialized")
		fmt.Println(err)
		return
	}

	var finalExp [10]FinalExpression
	for i := 0; i < 10; i++ {
		fmt.Println(finalExp[i].coe[0], finalExp[i].coe[1], finalExp[i].coe[2], finalExp[i].coe[3], finalExp[i].coe[4])
	}

	this.buffer["$"] = Lv
	var res float64
	var err error
	for iter := this.tmpCode.Front(); iter != nil; iter = iter.Next() {
		elem := iter.Value.(StackCodeNode)
		//		fmt.Println(elem.szId, elem.oper, elem.para1, elem.para2, elem.type1, elem.type2)
		var para1 float64
		var para2 float64
		if elem.type1 == 1 {
			para1 = elem.para1.(float64)
		} else {
			para1 = this.buffer[elem.para1.(string)]
		}

		if elem.type2 == 1 {
			para2 = elem.para2.(float64)
		} else {
			para2 = this.buffer[elem.para2.(string)]
		}
		res, err = this.Caculate(para1, para2, elem.oper)
		if err != nil {
			fmt.Println(err)
			return
		}
		//		fmt.Printf("Get this.buffer[%s] = %f\n", elem.szId, res)
		this.buffer[elem.szId] = res
	}
	fmt.Printf("Res = %f\n", res)
}

func (this *TempExpression) CaculateRes(Lv float64) {
	if this.buffer == nil || this.tmpCode == nil {
		err := fmt.Errorf("TempExpression is not initialized")
		fmt.Println(err)
		return
	}
	this.buffer["$"] = Lv
	var res float64
	var err error
	for iter := this.tmpCode.Front(); iter != nil; iter = iter.Next() {
		elem := iter.Value.(StackCodeNode)
		//		fmt.Println(elem.szId, elem.oper, elem.para1, elem.para2, elem.type1, elem.type2)
		var para1 float64
		var para2 float64
		if elem.type1 == 1 {
			para1 = elem.para1.(float64)
		} else {
			para1 = this.buffer[elem.para1.(string)]
		}

		if elem.type2 == 1 {
			para2 = elem.para2.(float64)
		} else {
			para2 = this.buffer[elem.para2.(string)]
		}
		res, err = this.Caculate(para1, para2, elem.oper)
		if err != nil {
			fmt.Println(err)
			return
		}
		//		fmt.Printf("Get this.buffer[%s] = %f\n", elem.szId, res)
		this.buffer[elem.szId] = res
	}
	fmt.Printf("Res = %f\n", res)
}
func (this *TempExpression) Caculate(para1 float64, para2 float64, oper byte) (float64, error) {
	var res float64 = 0.0
	switch oper {
	case '+':
		res = para1 + para2
		break
	case '-':
		res = para1 - para2
		break
	case '*':
		res = para1 * para2
		break
	case '/':
		if para2 == 0 {
			err := fmt.Errorf("RPNExpression has error of \"divide by zero\"")
			return 0, err
		}
		res = para1 / para2
		break
	default:
		err := fmt.Errorf("Unexpeted operator has found in RPNExpression")
		return 0, err
		break
	}
	return res, nil
}

func (this *Expression) Caculate(para1 float64, para2 float64, oper byte) (float64, error) {
	var res float64 = 0.0
	switch oper {
	case '+':
		res = para1 + para2
		break
	case '-':
		res = para1 - para2
		break
	case '*':
		res = para1 * para2
		break
	case '/':
		if para2 == 0 {
			err := fmt.Errorf("RPNExpression has error of \"divide by zero\"")
			return 0, err
		}
		res = para1 / para2
		break
	default:
		err := fmt.Errorf("Unexpeted operator has found in RPNExpression")
		return 0, err
		break
	}
	return res, nil
}

func (this *Expression) ParseTempCode(tempExp *TempExpression) error {

	if this.stack[0] == nil {
		err := fmt.Errorf("Expression is not initialized!")
		return err
	}
	if tempExp.tmpCode == nil {
		err := fmt.Errorf("tempExp is not initialized!")
		return err
	}
	nIdx := 1
	exp := this.stack[0]
	for iter := exp.Front(); iter != nil; iter = iter.Next() {
		elem := iter.Value
		if elem.(StackElem).szType == "num" || elem.(StackElem).szType == "arg" {
			// 当前可能是变量或是纯数字
			Stack_Push(elem.(StackElem).Var, elem.(StackElem).szType, this.stack[2])
		} else {
			// 运算符
			elem1 := Stack_Pop(this.stack[2])
			elem2 := Stack_Pop(this.stack[2])
			oper := elem.(StackElem).Var.(byte)
			if elem1 == nil || elem2 == nil {
				err := fmt.Errorf("there are something wrong with Expression")
				return err
			}

			// 判断结束，开搞
			// 首先，如果两个元素里有code
			if elem1.(StackElem).szType == "code" || elem2.(StackElem).szType == "code" || elem1.(StackElem).szType == "arg" || elem2.(StackElem).szType == "arg" {
				var para1 interface{}
				var para2 interface{}
				var type1 int
				var type2 int
				if elem2.(StackElem).szType == "code" {
					para1 = "a" + strconv.Itoa(elem2.(StackElem).Var.(int)) // code节点的本体是整数codeid,转成字符串
					type1 = 2
				} else if elem2.(StackElem).szType == "arg" {
					para1 = string(elem2.(StackElem).Var.(byte))
					type1 = 2
				} else {
					para1 = elem2.(StackElem).Var
					type1 = 1
				}

				if elem1.(StackElem).szType == "code" {
					para2 = "a" + strconv.Itoa(elem1.(StackElem).Var.(int))
					type2 = 2
				} else if elem1.(StackElem).szType == "arg" {
					para2 = string(elem1.(StackElem).Var.(byte))
					type2 = 2
				} else {
					para2 = elem1.(StackElem).Var
					type2 = 1
				}
				//----------------测试用输出----------------
				fmt.Printf("a%d = ", nIdx)

				if elem2.(StackElem).szType == "num" {
					fmt.Printf("%f %s ", elem2.(StackElem).Var, string(elem.(StackElem).Var.(byte)))
				} else if elem2.(StackElem).szType == "code" {
					fmt.Printf("a%d %s ", elem2.(StackElem).Var, string(elem.(StackElem).Var.(byte)))
				} else {
					fmt.Printf("%c %s ", elem2.(StackElem).Var, string(elem.(StackElem).Var.(byte)))
				}

				if elem1.(StackElem).szType == "num" {
					fmt.Printf("%f\n", elem1.(StackElem).Var)
				} else if elem1.(StackElem).szType == "code" {
					fmt.Printf("a%d \n", elem1.(StackElem).Var)
				} else {
					fmt.Printf("%c \n", elem1.(StackElem).Var)
				}
				//----------------测试用输出----------------

				strIdx := "a" + strconv.Itoa(nIdx)
				tempExp.tmpCode.PushBack(StackCodeNode{strIdx, oper, para1, para2, type1, type2})
				Stack_Push(nIdx, "code", this.stack[2])
				nIdx++
				continue
			}

			tmp, err := this.Caculate(elem2.(StackElem).Var.(float64), elem1.(StackElem).Var.(float64), elem.(StackElem).Var.(byte))
			if err != nil {
				return err
			}
			Stack_Push(tmp, "num", this.stack[2])
		}
	}
	if this.stack[2].Len() != 1 {
		err := fmt.Errorf("there are something wrong with RPNExpression")
		return err
	}
	return nil
}

func (this *Expression) ShowSourceExpression() {
	if this.stack[0] == nil {
		fmt.Print(fmt.Errorf("RPNExpression is not initialized!"))
		return
	}
	Stack_ShowAll(this.stack[0])
}

func NewExpression(strExp string) (*TempExpression, error) {
	var exp = new(Expression)
	exp.stack[0] = list.New()
	exp.stack[1] = list.New()
	exp.stack[2] = list.New()

	exp.strExp = exp.ExpressionPretreatment(strExp)
	//	fmt.Println(exp.strExp)
	exp.priority = make(map[byte]int)
	exp.priority['('] = 1
	exp.priority['+'] = 2
	exp.priority['-'] = 2
	exp.priority['*'] = 3
	exp.priority['/'] = 3
	exp.priority[')'] = 4
	err := exp.Process()
	exp.ShowSourceExpression()
	if err != nil {
		// 表达式处理失败
		return nil, err
	}
	tempExp := new(TempExpression)
	tempExp.tmpCode = list.New()
	tempExp.buffer = make(map[string]float64)
	exp.ParseTempCode(tempExp)
	return tempExp, nil
}

func Stack_Push(Input interface{}, szType string, stack *list.List) {
	stack.PushBack(StackElem{szType, Input})
}
func Stack_Top(stack *list.List) interface{} {
	e := stack.Back()
	if e != nil {
		return e.Value
	}
	return nil
}
func Stack_Pop(stack *list.List) interface{} {
	e := stack.Back()
	if e != nil {
		stack.Remove(e)
		return e.Value
	}
	return nil
}

func Stack_ShowAll(stack *list.List) {
	for elem := stack.Front(); elem != nil; elem = elem.Next() {
		if elem.Value.(StackElem).szType == "num" {
			fmt.Printf("%.2f ", elem.Value.(StackElem).Var.(float64))
		} else {
			fmt.Printf("%c ", elem.Value.(StackElem).Var.(byte))
		}
	}
	fmt.Println()
}

func (this *Expression) ExpressionPretreatment(szExp string) string {
	if szExp == "" {
		fmt.Println(fmt.Errorf("expression string error"))
		return ""
	}
	szBuf := ""
	szExp = strings.Replace(szExp, " ", "", -1)   //处理空格
	szExp = strings.Replace(szExp, "Lv", "$", -1) //处理关键字Lv
	if szExp[0] == '-' {
		szBuf = szBuf + "0"
	}
	szBuf = szBuf + string(szExp[0])
	for i := 1; i < len(szExp); i++ {
		if szExp[i] == '-' && (szExp[i-1] < '0' || szExp[i-1] > '9') && szExp[i-1] != '$' {
			szBuf = szBuf + "0-"
		} else {
			szBuf = szBuf + string(szExp[i])
		}
	}
	return szBuf
}

func (this *Expression) GetNumElement(beginPos int) (float64, int) {
	var numBuf = ""
	var strExp = this.strExp
	var jump = 0
	for j := 0; beginPos+j < len(strExp); j++ {
		if (strExp[j+beginPos] < '0' || strExp[j+beginPos] > '9') && strExp[j+beginPos] != '.' {
			break
		}
		numBuf = numBuf + string(strExp[j+beginPos])
		jump++
	}
	var f64Res, _ = strconv.ParseFloat(numBuf, 64)
	return f64Res, jump
}

func (this *Expression) Process() error {
	if this.strExp == "" {
		return fmt.Errorf("ProcessError Expression string is empty")
	}
	var strExp = this.strExp
	for i := 0; i < len(strExp); i++ {
		//		Stack_ShowAll(this.stack[0])
		//		Stack_ShowAll(this.stack[1])
		var jump = 0
		//读数字
		number, jump := this.GetNumElement(i)
		if jump != 0 {
			// jump != 0 说明成功读到了数字
			Stack_Push(number, "num", this.stack[0])
			i = i + jump - 1
			continue
		}
		if strExp[i] == '$' {
			//当前读到的值是变量
			Stack_Push(strExp[i], "arg", this.stack[0])
			continue
		}
		// --------------------------当前读到的是符号---------------------------------
		// 符号错误
		if strExp[i] != '+' && strExp[i] != '-' &&
			strExp[i] != '*' && strExp[i] != '/' &&
			strExp[i] != '(' && strExp[i] != ')' {
			err := fmt.Errorf("unexpected char %c ocurred in expression", strExp[i])
			fmt.Println(err)
			return err
		}
		if strExp[i] == '(' {
			//读取到左括号直接入s2
			Stack_Push(strExp[i], "oper", this.stack[1])
		} else if strExp[i] == ')' {
			//读取到右括号，开始将s2出栈
			elem := Stack_Pop(this.stack[1])
			for elem != nil && elem.(StackElem).Var != '(' {

				if elem.(StackElem).szType == "oper" && elem.(StackElem).Var.(byte) != '(' && elem.(StackElem).Var.(byte) != ')' {
					// 在遇上'('前将s2中的运算符依次导入s1中
					Stack_Push(elem.(StackElem).Var.(byte), "oper", this.stack[0])

				} else if elem.(StackElem).Var.(byte) == '(' {
					break
				}
				elem = Stack_Pop(this.stack[1])

				if elem == nil {
					//在s2栈空之前都没有发现'('判断输入表达式是错误的
					err := fmt.Errorf("expected char '(' before ')' and found nothing")
					fmt.Println(err)
					return err
				}
				if elem.(StackElem).Var.(byte) == '(' {
					break
				}
			}
		} else {
			// 读取到字符串中的其它运算符
			elem := Stack_Top(this.stack[1])

			if elem == nil {
				Stack_Push(strExp[i], "oper", this.stack[1])
			} else if this.priority[strExp[i]] > this.priority[elem.(StackElem).Var.(byte)] {
				Stack_Push(strExp[i], "oper", this.stack[1])
			} else {
				// 读到的运算符优先级小于栈顶的，开始出栈，直到当前运算符大于栈顶或栈空
				for true {
					elem = Stack_Pop(this.stack[1])
					if elem == nil {
						break
					}
					if this.priority[strExp[i]] > this.priority[elem.(StackElem).Var.(byte)] {
						Stack_Push(elem.(StackElem).Var.(byte), "oper", this.stack[1])
						break
					}
					Stack_Push(elem.(StackElem).Var.(byte), "oper", this.stack[0])
				}
				Stack_Push(strExp[i], "oper", this.stack[1])
			}
		}

	}
	for {
		elem := Stack_Pop(this.stack[1])
		if elem == nil {
			break
		}
		Stack_Push(elem.(StackElem).Var.(byte), "oper", this.stack[0])
	}
	return nil
}

func main() {
	f, _ := os.Create("profile_file")
	pprof.StartCPUProfile(f)
	defer pprof.StopCPUProfile()
	strexp := "-Lv-(5-3+34)*Lv"

	test_exp1, _ := NewExpression(strexp)
	test_exp2, _ := NewExpression("3.14159*Lv")
	//	test_exp.ShowFinalExp()
	test_exp1.CaculateRes(8)
	test_exp2.CaculateRes(1)
	//	test_exp.ShowSourceExpression()
	//	res, _ := test_exp.CaculateResault(8)
	//	fmt.Println(res)
	//	for i := 0; i < 1000000; i++ {
	//		test_exp.CaculateResault(8)
	//	}
}
