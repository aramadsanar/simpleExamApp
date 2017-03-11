#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <direct.h>
#include "stringTable.h"

int main(int argc, char *argv[])
{
    argc = 2;
    if (argc != 2)
    {
        printf("How to use app:\n\n%s <number_of_questions>", argv[0]);
        getchar();
    }

    else
    {
        argv[1] = "1";
        long int numberInput = strtol(argv[1], NULL, 10);
        int filesPresent = checkExamFiles(numberInput);
        int examIsDone = 0;

        if (filesPresent == 1)
        {
            char *repeatBuffer;
            repeatBuffer = malloc(3 * sizeof(*repeatBuffer));

            do
            {
                CLEAR_SCREEN

                printf("Press enter when you are ready for the exam..\n\n");
                getchar();

                examIsDone = mainExamLoop(numberInput);
                if (examIsDone == 1)
                {
                    printf(REPEAT_OFFER_STRING);
                    fgets(repeatBuffer, sizeof(repeatBuffer), stdin);
                    repeatBuffer[0] = parseResponse(repeatBuffer[0]);
                }
            }while(repeatBuffer[0] != 'N');

            free(repeatBuffer);
        }

        else
        {
            errorHandler(FILE_NOT_PRESENT_ERROR_CODE);
        }
    }

    return 0;
}

int checkExamFiles(const long int questionNumbers)
{
    int questionFilesPresent = 0;
    int answerKeyFilePresent = 0;
    long int iteration;
    char answerKeyFileName[BUFSIZ];
    char temporaryFileName[BUFSIZ];
    char examSetFolder[] = "\\examset";
    char txtTail[] = ".txt";
    char currentDirectory[BUFSIZ];
    FILE *answerKeyFile;

    getcwd(currentDirectory, sizeof(currentDirectory));

    snprintf(answerKeyFileName, sizeof(answerKeyFileName), "%s%s%s", currentDirectory, examSetFolder, "\\answer_key.txt");

    if ((answerKeyFile = fopen(answerKeyFileName, "r")))
    {
        answerKeyFilePresent = 1;
        fclose(answerKeyFile);
    }

    else
    {
        errorHandler(FOPEN_FAILED_ANSWER_KEY_ERROR_CODE);
        answerKeyFilePresent = 0;
    }

    for (iteration = 0; iteration < questionNumbers; iteration++)
    {
        snprintf(temporaryFileName, sizeof(temporaryFileName), "%s%s%s%ld%s", currentDirectory, examSetFolder, "\\question_no_", iteration, txtTail);

        FILE *questionFile;
        if ((questionFile = fopen(temporaryFileName, "r")))
        {
            fclose(questionFile);
        }

        else
        {
            fclose(questionFile);
            errorHandler(FOPEN_FAILED_QUESTION_ERROR_CODE);
        }
    }

    questionFilesPresent = 1;

    if ((questionFilesPresent == 1) && (answerKeyFilePresent == 1))
    {
        return 1;
    }

    else
    {
        errorHandler(FILE_NOT_PRESENT_ERROR_CODE);
        return 0;
    }

    return 0;
}

int mainExamLoop(const long int questionNumbers)
{
    long int iteration;
    int answerKeyCopied = 0;
    int allQuestionsAnswered = 0;
    char answers[questionNumbers+1];
    char answerKeyCached[questionNumbers+1];
    char questionBuffer[BUFSIZ];
    char answerKeyFileName[BUFSIZ];
    char temporaryFileName[BUFSIZ];
    char txtTail[] = ".txt";
    char examSetFolder[] = "\\examset";
    char currentDirectory[BUFSIZ];

    getcwd(currentDirectory, sizeof(currentDirectory));

    snprintf(answerKeyFileName, sizeof(answerKeyFileName), "%s%s%s", currentDirectory, examSetFolder, "\\answer_key.txt");

    FILE *answerKey = fopen(answerKeyFileName, "r");
    if (answerKey != 0)
    {
        //copy the answer key as a whole no matter what
        if (fgets(answerKeyCached, sizeof(answerKeyCached), answerKey) != NULL)
        {
            answerKeyCopied = 1;
            fclose(answerKey);
        }

        else
        {
            fclose(answerKey);
            errorHandler(FGETS_FAILED_ANSWER_KEY_ERROR_CODE);
        }
    }

    else
    {
        errorHandler(FOPEN_FAILED_ANSWER_KEY_ERROR_CODE);
    }

    char *answerHolder;
    answerHolder = malloc(3 * sizeof(char));

    for (iteration = 0; iteration < questionNumbers; iteration++)
    {
        snprintf(temporaryFileName, sizeof(temporaryFileName), "%s%s%s%ld%s", currentDirectory, examSetFolder, "\\question_no_", iteration, txtTail);

        FILE *fp = fopen(temporaryFileName, "r");
        if(fp != 0)
        {
            if (fgets(questionBuffer, sizeof(questionBuffer), fp) != NULL)
            {
                //once copied to buffer, the file is no longer needed!
                fclose(fp);
                CLEAR_SCREEN
                printf("%s\n\nPlease input your answer (A/B/C/D): ", questionBuffer);

                if (fgets(answerHolder, sizeof(answerHolder), stdin) != NULL)
                {
                    answers[iteration] = parseResponse(answerHolder[0]);
                    continue;
                }

                else
                {
                    errorHandler(USER_INPUT_FAILED_ERROR_CODE);
                }
            }

            else
            {
                fclose(fp);
                errorHandler(FGETS_FAILED_QUESTION_ERROR_CODE);
            }
        }

        else
        {
            errorHandler(FOPEN_FAILED_QUESTION_ERROR_CODE);
        }
    }

    free(answerHolder);

    //since all safety measures are in place, it is safe to assume that all questions were answered when it went out of the loop
    allQuestionsAnswered = 1;

    if ((answerKeyCopied == 1) && (allQuestionsAnswered == 1))
    {
        int resultsGenerated = 1;
        resultsGenerated = generateResults(answerKeyCached, answers, questionNumbers);
        if (resultsGenerated == 1)
        {
            return 1;
        }
    }
    //everything fails, go straight through this
    return 0;
}

int generateResults(char answerKeys[], char answers[], const int questionNumbers)
{
    char answerKey[questionNumbers+1];
    char userAnswers[questionNumbers+1];

    strcpy(answerKey, answerKeys);
    strcpy(userAnswers, answers);

    int correctAnswers = 0;
    double score = 0.0;
    int iteration;

    for (iteration = 0; iteration < questionNumbers; iteration++)
    {
        if (userAnswers[iteration] == answerKey[iteration])
        {
            correctAnswers++;
        }

        else
        {
            continue;
            //correctAnswers += 0;
        }
    }

    score = (double)correctAnswers/(double)questionNumbers;
    int gradeGenerated = generateGrade(score * 100, questionNumbers);

    if (gradeGenerated == 1)
    {
        return 1;
    }

    else
    {
        errorHandler(GRADE_GENERATOR_FAILED_ERROR_CODE);
        return 0;
    }

    return 0;   //grades not generated
}

int generateGrade(double score, long int questionNumbers)
{
    CLEAR_SCREEN
    char gradeCode = ' ';

    if (score < 40)
    {
        gradeCode = 'U';
    }

    else if ((score >= 40) && (score < 50))
    {
        gradeCode = 'E';
    }

    else if ((score >= 50) && (score < 60))
    {
        gradeCode = 'D';
    }

    else if ((score >= 60) && (score < 70))
    {
        gradeCode = 'C';
    }

    else if ((score >= 70) && (score < 80))
    {
        gradeCode = 'B';
    }

    else if ((score >= 80) && (score < 90))
    {
        gradeCode = 'A';
    }

    else if ((score >= 90) && (score <= 100))
    {
        gradeCode = 'A';
    }

    else
    {
        errorHandler(GRADE_GENERATOR_FAILED_ERROR_CODE);
    }

    printf(GRADE_STRING, gradeCode, score);
    getchar();
    return 1;
}

void errorHandler(int errorCode)
{
    CLEAR_SCREEN

    switch(errorCode)
    {
    case 0:
        CLEAR_SCREEN
        printf(UNKNOWN_ERROR_STRING);
        EXIT_CRASH
        break;
    case 1:
        CLEAR_SCREEN
        printf(FILE_NOT_PRESENT_ERROR_STRING);
        EXIT_CRASH
        break;
    case 2:
        CLEAR_SCREEN
        printf(FOPEN_FAILED_ANSWER_KEY_ERROR_STRING);
        EXIT_CRASH
        break;
    case 3:
        CLEAR_SCREEN
        printf(FOPEN_FAILED_QUESTION_ERROR_STRING);
        EXIT_CRASH
        break;
    case 4:
        CLEAR_SCREEN
        printf(FGETS_FAILED_ANSWER_KEY_ERROR_STRING);
        EXIT_CRASH
        break;
    case 5:
        CLEAR_SCREEN
        printf(FGETS_FAILED_QUESTION_ERROR_STRING);
        EXIT_CRASH
        break;
    case 6:
        CLEAR_SCREEN
        printf(GRADE_GENERATOR_FAILED_ERROR_STRING);
        EXIT_CRASH
        break;
    case 7:
        CLEAR_SCREEN
        printf(USER_INPUT_FAILED_ERROR_STRING);
        EXIT_CRASH
        break;
    }
}

char parseResponse(char input)
{
    char parsedInput = ' ';

    if (input >= 'A' && input <= 'Z')
    {
        parsedInput = input;
    }

    else if (input >= 'a' && input <= 'z')
    {
        parsedInput = input - 32;
    }

    return parsedInput;
}
