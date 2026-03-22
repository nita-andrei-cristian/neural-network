#ifndef NEURAL_NETWORK_BRAIN_MENU
#define NEURAL_NETWORK_BRAIN_MENU

enum INPUT_TYPE {
	MESSAGE,
	EXPORT,
	TEST0,
	TEST1,
	TEST2,
	TEST3,
	TEST4,
	QUIT,
	INPUT_ERROR,
	CLEAR,
};

void start_menu();

void tick_menu();

void end_menu();


#endif
