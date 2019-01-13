
int init() {
	// 1. init all drivers
	
	// 2. init all peripherals
	
}

int main() {
	/* Create the big data structures to house pod data */
	Bms bms; 
	Rms rms;
	
	if (init() == 1) {
		printf("Error in initialization! Exiting...\r\n");
		exit();
	}
	
	while(1) {
	
		// Control Loop!		

	}

}
