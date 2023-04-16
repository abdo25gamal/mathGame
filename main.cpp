#include <iostream>
#include <vector>
using namespace std;

enum enOperationType {Add =1, Sub = 2, Mult = 3, Div = 4, MixOP = 5};

enum enQuestionLevel  {EasyLevel = 1, MidLevel = 2, HardLevel = 3 , Mix = 4};

struct stQuestion
{
       short Number1 = 0;
       short Number2 = 0;
       enOperationType OperationType;
       enQuestionLevel QuestionLevel;
       int CorrectAnswer = 0;
       int PlayerAnswer = 0;
       bool AnswerResult = false;
};

struct stQuizz
{
       vector<stQuestion> vQuestionList;
       short NumberOfQuestions = 0;
       enQuestionLevel QuestionLevel;
       enOperationType OpType;
       short NumberOfWrongAnswers = 0;
       short NumberOfRightAnswers = 0;
       bool isPass = false; 
};

int ReadNumber(string Message)
{
        cout << Message << endl;
        int Number = 0;
        cin >> Number;
        while(cin.fail())
        {
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
                cout << "Invalid Number, " << Message << endl;
                cin >> Number;
        }
        return Number;
}

int ReadNumberInRange(int From, int To,string Message)
{
        int Number = ReadNumber(Message);
        while(Number < From || Number > To)
        {
                Number = ReadNumber(Message);
        }
        return Number;
}

int RandomNumber(int From, int To)
{
        return rand() % (To - From + 1)  + From;
}

void ResetScreen()
{
        system("clear");
        system("color 0F");
}

void SetScreenWinner(bool Right)
{
      if(Right)
      {
        system("color 2F");
      }
      else
      {
        system("color 4F"); 
        cout << "\a"; 
      }   
}

string GetQuestionLevelText(enQuestionLevel QuestionLevel)
{
        string arrQuestionLevelText[] = {"Easy", "Mid", "Hard", "Mix"};
        return arrQuestionLevelText[QuestionLevel - 1]; 
}

int SimpleCalculator(short Number1, short Number2,enOperationType OperationType)
{

        switch (OperationType)
        {
        case enOperationType::Add  : return Number1 + Number2;
        case enOperationType::Sub  : return Number1 - Number2;
        case enOperationType::Mult : return Number1 * Number2;
        case enOperationType::Div  : return Number1 / Number2;

        case enOperationType::MixOP: 
        OperationType = (enOperationType) RandomNumber(1,4);
        return SimpleCalculator(Number1,Number2,OperationType);

        default: return 0; 
        }
        
}

string GetOpTypeSymbol(enOperationType &OpType)
{
        switch (OpType)
        {
        case enOperationType::Add   : return "+";
        case enOperationType::Sub   : return "-";
        case enOperationType::Mult  : return "*";
        case enOperationType::Div   : return "/";
        default : return "Mix";
        }
}

stQuestion GenerateQuestion(enQuestionLevel QuestionLevel,enOperationType OperationType)
{
  stQuestion Question;

  if(QuestionLevel == enQuestionLevel::Mix){QuestionLevel = (enQuestionLevel) RandomNumber(1,3);}
  Question.QuestionLevel = QuestionLevel;

  if(OperationType == enOperationType::MixOP){OperationType = (enOperationType) RandomNumber(1,4);}
  Question.OperationType = OperationType;

  switch (QuestionLevel)
  {
  case enQuestionLevel::EasyLevel: 
  Question.Number1 = RandomNumber(1,10);   
  Question.Number2 = RandomNumber(1,10);
  break;   
  
  case enQuestionLevel::MidLevel: 
  Question.Number1 = RandomNumber(10,50);   
  Question.Number2 = RandomNumber(10,50);
  break;

  case enQuestionLevel::HardLevel: 
  Question.Number1 = RandomNumber(50,100);   
  Question.Number2 = RandomNumber(50,100);
  break;

  case enQuestionLevel::Mix: 
  QuestionLevel = (enQuestionLevel) RandomNumber(1,3);
  return GenerateQuestion(QuestionLevel, OperationType);
  } 
 

  Question.CorrectAnswer = SimpleCalculator(Question.Number1,Question.Number2,OperationType);
  return Question;
  
}

void GenerateQuizzQuestions(stQuizz &Quizz)
{
        for(short i = 0; i < Quizz.NumberOfQuestions; ++i)
        {
           Quizz.vQuestionList.push_back(GenerateQuestion(Quizz.QuestionLevel,Quizz.OpType));     
        }
}

void PrintTheQuestion(stQuizz Quizz,short QuestionNumber)
{
        cout << "\n";
        cout << "Question [" << QuestionNumber + 1 << "/" << Quizz.NumberOfQuestions << "] \n\n";
        cout << Quizz.vQuestionList.at(QuestionNumber).Number1 << endl;
        cout << GetOpTypeSymbol(Quizz.vQuestionList.at(QuestionNumber).OperationType) << endl;
        cout << Quizz.vQuestionList.at(QuestionNumber).Number2 << endl;
        
        cout << "\n__________________" << endl;
}

void CorrectTheQuestionAnswer(stQuizz &Quizz,short QuestionNumber)
{
        if(Quizz.vQuestionList.at(QuestionNumber).PlayerAnswer != Quizz.vQuestionList.at(QuestionNumber).CorrectAnswer)
        {
                Quizz.vQuestionList.at(QuestionNumber).AnswerResult = false;
                Quizz.NumberOfWrongAnswers++;

                cout << "Wrong Answer :-( \n";
                cout << "The Right Answer is :";
                cout << Quizz.vQuestionList.at(QuestionNumber).CorrectAnswer;
                cout << '\n';
        }
        else
        {
                Quizz.vQuestionList.at(QuestionNumber).AnswerResult = true;
                Quizz.NumberOfRightAnswers++;

                cout << "Right Answer :-) \n";
        }

        cout << endl;

        SetScreenWinner(Quizz.vQuestionList.at(QuestionNumber).AnswerResult);
}

void AskAndCorrectQuestionListAnswer(stQuizz &Quizz)
{
        for(short QuestionNumber = 0; QuestionNumber < Quizz.NumberOfQuestions; ++QuestionNumber)
        {
                PrintTheQuestion(Quizz,QuestionNumber);

                Quizz.vQuestionList.at(QuestionNumber).PlayerAnswer = ReadNumber("");
                
                CorrectTheQuestionAnswer(Quizz,QuestionNumber);
        }

        Quizz.isPass = (Quizz.NumberOfRightAnswers >= Quizz.NumberOfWrongAnswers);
}

string GetFinalResultsText(bool Pass)
{
       return (Pass)? "Pass :-)" : "Fail :-(";
}

void PrintQuizzResults(stQuizz Quizz)
{
        cout << "\n";
        cout << "_____________________________\n\n";
        cout << "Final Results is " << GetFinalResultsText(Quizz.isPass);
        cout << "\n___________________________\n\n";
        
        cout << "Number of Questions    : " << Quizz.NumberOfQuestions << endl;
        cout << "Question Level         : " << GetQuestionLevelText(Quizz.QuestionLevel) << endl;
        cout << "OpType                 : " << GetOpTypeSymbol(Quizz.OpType) << endl;
        cout << "Number of Right answers: " << Quizz.NumberOfRightAnswers << endl;
        cout << "Number of Wrong answers: " << Quizz.NumberOfWrongAnswers << endl;
        cout << "_____________________________\n";
}

void PlayMathGame()
{
        stQuizz Quizz;
        Quizz.NumberOfQuestions = ReadNumber("How many Questions do you want to answer? ");
        Quizz.QuestionLevel = (enQuestionLevel) ReadNumberInRange(1,4,"Enter Questions Level [1] Easy, [2] Mid, [3] hard, [4] Mix? ");
        Quizz.OpType = (enOperationType) ReadNumberInRange(1,5,"Enter Operation Type [1] Add,[2] Sub,[3] Mul, [4] Div, [5] Mix? ");

        GenerateQuizzQuestions(Quizz);
        AskAndCorrectQuestionListAnswer(Quizz);
        PrintQuizzResults(Quizz);   
}

void StartGame()
{
     char PlayAgain = 'Y';
      do
      {
        ResetScreen();
        PlayMathGame();

        cout << endl << "Do you want to play again Y/N? ";
        cin >> PlayAgain; 

      } while (PlayAgain == 'Y' || PlayAgain == 'y');
}

int main()
{
        srand((unsigned)time(NULL));
        StartGame();
        return 0; 
}
