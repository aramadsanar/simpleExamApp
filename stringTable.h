#define getcwd _getcwd

#define SAFE_CRASH printf("This program has crashed, exiting for safety.. Bye..\n");\
                   getchar();\
                   exit(18);

#define EXIT_CRASH NEWLINE\
                   getchar();\
                   CLEAR_SCREEN\
                   exit(255);

#define NEWLINE printf("\n");
#define RETURN_TO_MAIN_MENU main(1);
#define END_EXERCISE getchar();
#define CLEAR_SCREEN system("cls");

#define STANDARD_CLEANUP  NEWLINE\
                          END_EXERCISE\
                          RETURN_TO_MAIN_MENU

#define EXIT_NORMAL NEWLINE\
                    END_EXERCISE\
                    CLEAR_SCREEN\
                    exit(0);

#define GRADE_STRING "Your grade is %c and your score is %.2lf\n"
#define REPEAT_OFFER_STRING "Do you want to repeat this exam (Type 'N' then press enter to exit this program)? "

#define UNKNOWN_ERROR_STRING "We've encountered an unknown error (code 0)."
#define FILE_NOT_PRESENT_ERROR_STRING "The exam files are not present (code 1)."
#define FOPEN_FAILED_ANSWER_KEY_ERROR_STRING "We can't open the answer key file (code 2).\nThe file might not be present or you may input the question numbers incorrectly."
#define FOPEN_FAILED_QUESTION_ERROR_STRING "We can't open the question file (code 3).\nThe file might not be present or you may input the question numbers incorrectly."
#define FGETS_FAILED_ANSWER_KEY_ERROR_STRING "We can't copy the answer key to our cache (code 4)."
#define FGETS_FAILED_QUESTION_ERROR_STRING "We can't copy the question to our cache (code 5)."
#define GRADE_GENERATOR_FAILED_ERROR_STRING "We can't generate the grade for you (code 6)."
#define USER_INPUT_FAILED_ERROR_STRING "We can't get the user input (code 7)."

#define UNKNOWN_ERROR_CODE 0
#define FILE_NOT_PRESENT_ERROR_CODE 1
#define FOPEN_FAILED_ANSWER_KEY_ERROR_CODE 2
#define FOPEN_FAILED_QUESTION_ERROR_CODE 3
#define FGETS_FAILED_ANSWER_KEY_ERROR_CODE 4
#define FGETS_FAILED_QUESTION_ERROR_CODE 5
#define GRADE_GENERATOR_FAILED_ERROR_CODE 6
#define USER_INPUT_FAILED_ERROR_CODE 7

int mainExamLoop(const long int questionNumbers);
int generateResults(char *answerKeys, char *answers, const int questionNumbers);
int checkExamFiles(const long int questionNumbers);
int generateGrade(double score, long int questionNumbers);
void errorHandler(int errorCode);
char parseResponse(char input);
