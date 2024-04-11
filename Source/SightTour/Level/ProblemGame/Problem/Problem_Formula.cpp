// Copyright Epic Games, Inc. All Rights Reserved.
#include "Problem_Formula.h"

void FProblem_Formula::InitProblem()
{
	bInit = true;

	TArray<TCHAR> Operator = {'+', '-', '*', '/'};

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
				if (ProblemContent[i] >= '0' && ProblemContent[i] <= '9' || Operator.Find(ProblemContent[i]))
				{
					FString TempString;
					while (ProblemContent[i] >= '0' && ProblemContent[i] <= '9' || Operator.Find(ProblemContent[i]))
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
					} while (ProblemContent[i] != SplitChar[0]);

					LeftExpression.Push("");
				}
			}
			// 处理右边的式子
			else
			{
				if (ProblemContent[i] >= '0' && ProblemContent[i] <= '9' || Operator.Find(ProblemContent[i]))
				{
					FString TempString;
					while (ProblemContent[i] >= '0' && ProblemContent[i] <= '9' || Operator.Find(ProblemContent[i]))
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
					} while (ProblemContent[i] != SplitChar[0]);

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
	if (!bInit)
	{
		InitProblem();
	}

	int32 EmptyStrIndex = LeftExpression.Find("");

	if (EmptyStrIndex != INDEX_NONE)
	{
		LeftExpression[EmptyStrIndex] = AnswerStr;
		return true;
	}

	EmptyStrIndex = RightExpression.Find("");
	if (EmptyStrIndex != INDEX_NONE)
	{
		RightExpression[EmptyStrIndex] = AnswerStr;
		return true;
	}

	return false;
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
	TMap<TCHAR, int32> m{ {'(',0},{')',0},{'+',1},{'-',1},{'*',2},{'/',2} };

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
		if (Str[i] >= '0' && Str[i] <= '9')
		{
			int32 Num = 0;
			while (Str[i] >= '0' && Str[i] <= '9')
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
				while (OperatorStack.Num() && m[OperatorStack.Top()] >= m[Str[i]])
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


	return NumberStack.Top();
}