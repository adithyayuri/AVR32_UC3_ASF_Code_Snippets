// Function to read the serial number of UC3C series of MCUs

unsigned int UniqID[4];

void getID(unsigned int *ID)
{
	volatile unsigned int *ptr_ID = 0x8080010C;

	for (int i=0; i<4;i++)
	{
		ID[i] = *ptr_ID;
		ptr_ID++;
	}
	ID[3] = ID[3]>>16;
}
