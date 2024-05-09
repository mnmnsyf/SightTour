// Copyright Epic Games, Inc. All Rights Reserved.
#include "Problem_Formula.h"

TArray<TCHAR> FProblem_Formula::Operator = { '+', '-', '*', '/' };

void FProblem_Formula::InitProblem()
{
	bInit = true;

	//分割等式左右两边
	bool VisitEquilSign = false;
	for (int32 i = 0; i < ProblemContent.Len(); ++i)
	{
		//忽略等于号
		if (ProblemContent[i] == '=')
		{
			VisitEquilSign = true;
		}
		else
		{
			// 处理左边的式子
			if (!VisitEquilSign)
			{
				if (FChar::IsDigit(ProblemContent[i]) || Operator.Contains(ProblemContent[i]))
				{
					FString TempString;
					while (i < ProblemContent.Len() && (FChar::IsDigit(ProblemContent[i]) || Operator.Contains(ProblemContent[i])))
					{
						TempString.AppendChar(ProblemContent[i]);
						++i;
					}
					--i;
					LeftExpression.Push(TempString);
				}
				//分隔符内容对应空字符串
				else if (ProblemContent[i] == SplitChar[0])
				{
					do {
						++i;
					} while (i < ProblemContent.Len() && ProblemContent[i] != SplitChar[0]);

					LeftExpression.Push("");
				}
			}
			// 处理右边的式子
			else
			{
				if (FChar::IsDigit(ProblemContent[i]) || Operator.Contains(ProblemContent[i]))
				{
					FString TempString;
					while (i < ProblemContent.Len() && (FChar::IsDigit(ProblemContent[i]) || Operator.Contains(ProblemContent[i])))
					{
						TempString.AppendChar(ProblemContent[i]);
						++i;
					}
					--i;
					RightExpression.Push(TempString);
				}
				//分隔符内容填空的字符串
				else if (ProblemContent[i] == SplitChar[0])
				{
					do {
						++i;
					} while (i < ProblemContent.Len() && ProblemContent[i] != SplitChar[0]);

					RightExpression.Push("");
				}
			}
		}
	}
}

FName FProblem_Formula::GetProblemTypeName()
{
	throw std::logic_error("The method or operation is not implemented.");
}

bool FProblem_Formula::FillProblem(FString AnswerStr)
{
	//判断字符串是否合法(仅包含数字和运算符号)
	for(int32 Index = 0 ; Index < AnswerStr.Len(); ++Index)
	{
		if (!FChar::IsDigit(AnswerStr[Index]) && Operator.Find(AnswerStr[Index]) == INDEX_NONE)
		{
			return false;
		}
	}

	if (!bInit)
	{
		InitProblem();
	}

	int32 EmptyStrIndex = LeftExpression.Find("");
	if (EmptyStrIndex != INDEX_NONE)
	{
		LeftExpression[EmptyStrIndex] = AnswerStr;
	}
	else
	{
		EmptyStrIndex = RightExpression.Find("");
		if (EmptyStrIndex != INDEX_NONE)
		{
			RightExpression[EmptyStrIndex] = AnswerStr;
		}
	}

	return LeftExpression.Find("") == INDEX_NONE && RightExpression.Find("") == INDEX_NONE;
}

bool FProblem_Formula::CheckAllAnswer()
{
	if (!bInit)
	{
		InitProblem();
	}

	if (LeftExpression.IsEmpty() || RightExpression.IsEmpty())
	{
		return false;
	}

	int32 LeftInt = ExpressionEvaluation(LeftExpression);
	int32 RightInt = ExpressionEvaluation(RightExpression);

	return LeftInt == RightInt;
}

void FProblem_Formula::ResetProblem()
{
	InitProblem();
}

TArray<FString> FProblem_Formula::GetUIShowList()
{
	if (!bInit)
	{
		InitProblem();
	}

	TArray<FString> ShowList = LeftExpression;
	ShowList.Add(TEXT("="));
	ShowList.Append(RightExpression);

	return ShowList;
}

int32 FProblem_Formula::ExpressionEvaluation(const TArray<FString>& Expression)
{
	FString Str = "";

	for (FString EachStr : Expression)
	{
		Str.Append(EachStr);
	}

	TArray<TCHAR> OperatorStack;
	TArray<int32> NumberStack;
	TMap<TCHAR, int32> OperatorToPriority{ {'(',0},{')',0},{'+',1},{'-',1},{'*',2},{'/',2} };

	auto Evaluate = [&]() {
		int32 a = NumberStack.Top();
		NumberStack.Pop();
		int32 b = NumberStack.Top();
		NumberStack.Pop();

		TCHAR c = OperatorStack.Top();
		OperatorStack.Pop();

		switch (c)
		{
		case '+':
		{
			NumberStack.Push(a + b);
			break;
		}
		case '-':
		{
			NumberStack.Push(b - a);
			break;
		}
		case '*':
		{
			NumberStack.Push(a * b);
			break;
		}
		case '/':
		{
			if (ensure(a != 0))
			{
				NumberStack.Push(b / a);
			}
			break;
		}
		default:
			check(0);
		}
		};

	for (int32 i = 0; i < Str.Len(); ++i)
	{
		if (FChar::IsDigit(Str[i]))
		{
			int32 Num = 0;
			while (i < Str.Len() && FChar::IsDigit(Str[i]))
			{
				Num = Num * 10 + Str[i] - '0';
				++i;
			}
			NumberStack.Push(Num);
			--i;
		}
		else
		{
			if (Str[i] == '(')
			{
				OperatorStack.Push(Str[i]);
			}
			else if (Str[i] == ')')
			{
				while (OperatorStack.Top() != '(')
				{
					Evaluate();
				}
				OperatorStack.Pop();
			}
			else
			{
				while (OperatorStack.Num() && OperatorToPriority.Contains(Str[i]) && OperatorToPriority[OperatorStack.Top()] >= OperatorToPriority[Str[i]])
				{
					Evaluate();
				}
				OperatorStack.Push(Str[i]);
			}
		}
	}

	while (OperatorStack.Num())
	{
		Evaluate();
	}

	if (ensure(NumberStack.Num()))
	{
		return NumberStack.Top();
	}
	return INDEX_NONE;
}

void FProblem_Text::InitProblem()
{
	bInit = true;
	bAnswerQustion = true;
	for (int32 i = 0; i < ProblemContent.Len(); ++i)
	{
		if (ProblemContent[i] == SplitChar[0])
		{
			ProblemList.Push("");
			i++;
			FString TempString;
			while (i < ProblemContent.Len() && ProblemContent[i] != SplitChar[0])
			{
				TempString.AppendChar(ProblemContent[i]);
				i++;
			}
			AnswerList.Emplace(ProblemList.Num() - 1, TempString);
		}
		else
		{
			FString TempString;
			while (i < ProblemContent.Len() && ProblemContent[i] != SplitChar[0])
			{
				TempString.AppendChar(ProblemContent[i]);
				i++;
			}
			i--;
			ProblemList.Push(TempString);
		}
	}
}

TArray<FString> FProblem_Text::GetUIShowList()
{
	if (!bInit)
	{
		InitProblem();
	}

	return ProblemList;
}

bool FProblem_Text::FillProblem(FString AnswerStr)
{
	if (!bInit)
	{
		InitProblem();
	}
	int32 EmptyStrIndex = ProblemList.Find("");

	if (EmptyStrIndex != INDEX_NONE)
	{
		ProblemList[EmptyStrIndex] = AnswerStr;
		
		if (AnswerList[EmptyStrIndex] != AnswerStr)
		{
			bAnswerQustion = false;
		}
		return ProblemList.Find("") == INDEX_NONE;
	}

	return true;
}

bool FProblem_Text::CheckAllAnswer()
{
	return bAnswerQustion;
}

void FProblem_Text::ResetProblem()
{
	ProblemList.Reset();
	AnswerList.Reset();
}
