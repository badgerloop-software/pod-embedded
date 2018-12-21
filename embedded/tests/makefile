FLAGS=-Wall -o
CAN_PATH=../can/

test_can: $(CAN_PATH)can.c $(CAN_PATH)can.h can_test.c
	gcc $(FLAGS) can_test can_test.c $(CAN_PATH)can.c

